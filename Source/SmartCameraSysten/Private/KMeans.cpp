#include "KMeans.h"

//Other
#include <cstdlib>
#include <ctime>
#include <limits>
#include <random>
#include <algorithm>

//Engine includes
#include "Kismet/GameplayStatics.h"
#include "ProfilingDebugging/CookStats.h"
#include "EngineUtils.h"
#include "Engine/World.h"
//#include "Algo/RemoveIf.h"
/*
*	TODO
*	Implement cook stats, look into it
*/

KMeans::KMeans(int32 n_clusters, int32 max_iter, int32 random_state) :
	m_NumberClusters(n_clusters),
	m_MaxIter(max_iter),
	m_RandomState(random_state)
{

}

std::vector<FPoint> KMeans::InitializeCentroids(const std::vector<FPoint>& points)
{
	std::vector<FPoint> centroids(m_NumberClusters);

	static std::random_device seed;
	static std::mt19937 random_number_generator(seed());
	std::uniform_int_distribution<size_t> indices(0, points.size() - 1);

	for (int32 clusterId = 0; clusterId < m_NumberClusters; clusterId++)
	{
		centroids[clusterId] = points[indices(random_number_generator)];
		//centroids[clusterId].cluster = clusterId;
	}

	return centroids;
}

std::vector<FPoint> KMeans::ComputeCentroids(const std::vector<FPoint>& points, const std::vector<FPoint>& distance)
{
	std::vector<FPoint> newMeans(m_NumberClusters);
	std::vector<int32> counts(m_NumberClusters, 0);
	std::vector<FPoint> newCentroids(m_NumberClusters);

	//Counts points for each centroid
	for (int32 pointId = 0; pointId < points.size(); pointId++)
	{
		const FPoint& compDistance = distance[pointId];
		newMeans[compDistance.cluster].location.X += points[pointId].location.X;
		newMeans[compDistance.cluster].location.Y += points[pointId].location.Y;
		counts[compDistance.cluster] += 1;
	}

	//Get new centroids
	for (int32 clusterId = 0; clusterId < m_NumberClusters; clusterId++)
	{
		int32 count = std::max<int32>(1, counts[clusterId]);
		newCentroids[clusterId].location.X = newMeans[clusterId].location.X / count;
		newCentroids[clusterId].location.Y = newMeans[clusterId].location.Y / count;
		newCentroids[clusterId].cluster = clusterId;
	}

	return newCentroids;
}

std::vector<FPoint> KMeans::ComputeDistance(const std::vector<FPoint>& points, const std::vector<FPoint>& centroids)
{
	std::vector<FPoint> assignement(points.size());

	for (int32 pointId = 0; pointId < points.size(); pointId++)
	{
		const FPoint& point = points[pointId];
		float bestDistance = std::numeric_limits<float>::max();
		int32 bestCluster = 0;

		for (int32 clusterId = 0; clusterId < m_NumberClusters; clusterId++)
		{
			float distance = point.Distance(centroids[clusterId]);

			if (distance < bestDistance)
			{
				bestDistance = distance;
				bestCluster = clusterId;
			}
		}

		assignement[pointId] = point;

		FPoint& pointComp = assignement[pointId];
		pointComp.cluster = bestCluster;
		pointComp.minDist = bestDistance;
	}

	return assignement;
}

void KMeans::Fit(const std::vector<FPoint>& data)
{
	m_Centroids = InitializeCentroids(data);

	for (int32 i = 0; i < m_MaxIter; i++)
	{
		//Save
		std::vector<FPoint> oldCentroids = m_Centroids;

		std::vector<FPoint> distance = ComputeDistance(data, m_Centroids);
		m_Centroids = ComputeCentroids(data, distance);

		//Check if they are the same and we should stop it
		//Not now.....
	}
}

std::vector<FPoint> KMeans::Predict(const std::vector<FPoint>& data)
{
	return  ComputeDistance(data, m_Centroids);
}


UClusterGeneratorComponent::UClusterGeneratorComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UClusterGeneratorComponent::BeginPlay()
{
	Super::BeginPlay();

	// set a callback so we consider new actors for our filter
	if (UWorld* MyWorld = GetWorld())
	{
		FOnActorSpawned::FDelegate ActorSpawnedDelegate = FOnActorSpawned::FDelegate::CreateUObject(this, &UClusterGeneratorComponent::OnActorSpawned);
		m_ActorSpawnedDelegateHandle = MyWorld->AddOnActorSpawnedHandler(ActorSpawnedDelegate);
	}

	// but we still need to gather all the existing actors in the world
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AddToActorList(*ActorItr);
	}
}

void UClusterGeneratorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// being polite and tidying up
	if (UWorld* MyWorld = GetWorld())
	{
		MyWorld->RemoveOnActorSpawnedHandler(m_ActorSpawnedDelegateHandle);
	}

	Super::EndPlay(EndPlayReason);
}

void UClusterGeneratorComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Remove old objects
	for(TMap<FString, AActor*>::TIterator ItRemove = m_ActorMap.CreateIterator(); ItRemove; ++ItRemove)
	{
		if (ItRemove->Value == nullptr || ItRemove->Value->IsPendingKillPending())
		{
			ItRemove.RemoveCurrent();
		}		
	}
}

uint32 UClusterGeneratorComponent::GenerateAsynchronous()
{
	FAsyncTask<FClusterGenerationAsyncTask>*  AsyncTask = new FAsyncTask<FClusterGenerationAsyncTask>(NumberOfClusters, MaxIterations, ClassTypeToCluster, GetWorld(), m_ActorMap);

	const uint32 Handle = GetNextHandle();
	check(!m_PendingTasks.Contains(Handle));
	m_PendingTasks.Add(Handle, AsyncTask);

	AsyncTask->StartSynchronousTask();

	return Handle;
}

bool UClusterGeneratorComponent::GetAsynchronousResults(uint32 Handle, TArray<FPoint>& generatedCluster, TArray<FPoint>& generatedCentroids)
{
	FAsyncTask<FClusterGenerationAsyncTask>* AsyncTask = NULL;
	{
		FScopeLock ScopeLock(&m_SynchronizationObject);
		m_PendingTasks.RemoveAndCopyValue(Handle, AsyncTask);
	}

	check(AsyncTask);
	const bool bDataWasBuilt = AsyncTask->GetTask().WasDataBuilt();

	if (!bDataWasBuilt)
	{
		delete AsyncTask;
		return false;
	}

	generatedCluster = AsyncTask->GetTask().GetGeneratedClusters();
	generatedCentroids = AsyncTask->GetTask().GetGeneratedCentroids();

	//Update actor map
	//m_ActorMap = AsyncTask->GetTask().GetActorMap();

	delete AsyncTask;

	check(generatedCluster.Num());
	check(generatedCentroids.Num());
	check(m_ActorMap.Num());

	return true;
}

bool UClusterGeneratorComponent::PollAsynchronousCompletion(uint32 Handle)
{
	FAsyncTask<FClusterGenerationAsyncTask>* AsyncTask = nullptr;
	{
		FScopeLock ScopeLock(&m_SynchronizationObject);
		AsyncTask = m_PendingTasks.FindRef(Handle);
	}

	check(AsyncTask);
	return AsyncTask->IsDone();
}

void UClusterGeneratorComponent::WaitAsynchronousCompletion(uint32 Handle)
{
	FAsyncTask<FClusterGenerationAsyncTask>* AsyncTask = nullptr;
	{
		FScopeLock ScopeLock(&m_SynchronizationObject);
		AsyncTask = m_PendingTasks.FindRef(Handle);
	}

	check(AsyncTask);
	AsyncTask->EnsureCompletion();
}

void UClusterGeneratorComponent::GenerateSynchronous(TArray<FPoint>& generatedCluster, TArray<FPoint>& generatedCentroids)
{
	FAsyncTask<FClusterGenerationAsyncTask> PendingTask(NumberOfClusters, MaxIterations, ClassTypeToCluster, GetWorld(), m_ActorMap);
	PendingTask.StartSynchronousTask();

	generatedCluster = PendingTask.GetTask().GetGeneratedClusters();
	generatedCentroids = PendingTask.GetTask().GetGeneratedCentroids();

	//Update actor map
	//m_ActorMap = PendingTask.GetTask().GetActorMap();
}

void UClusterGeneratorComponent::UpdateClustersPosition(const TArray<FPoint>& inGeneratedCluster, TArray<FPoint>& outUpdatedGenertaedCluster)
{
	//Make sure map is not changed while we are searching for saved actor references
	FScopeLock lock(&m_CriticalSection);

	//Copy
	//outUpdatedGenertaedCluster = inGeneratedCluster;

	//Get new locations
	for (const FPoint& point : inGeneratedCluster)
	{
		AActor** actor = m_ActorMap.Find(point.guid);
		if (actor != nullptr && IsValid(*actor) == true)
		{
			FPoint copyPoint(point);
			copyPoint.location = (*actor)->GetActorLocation();
			copyPoint.rotation = (*actor)->GetActorRotation();

			outUpdatedGenertaedCluster.Add(MoveTemp(copyPoint));
		}
	}
}

void UClusterGeneratorComponent::UpdateCentroids(const TArray<FPoint>& inGeneratedCluster, const TArray<FPoint>& inGeneratedCentroids, TArray<FPoint>& outUpdatedCentroids)
{
	//Copy
	outUpdatedCentroids = inGeneratedCentroids;

	/*
		looping over clusters and calculate Euclidian distance of
		each point within that cluster from its centroid and
		pick the maximum which is the radius of that cluster
	*/
	for (const auto& point : inGeneratedCluster)
	{
		FPoint& centroid = outUpdatedCentroids[point.cluster];
		centroid.radius = std::max(centroid.radius, centroid.Distance(point));
	}
}

void UClusterGeneratorComponent::GetCentroidWithMostClusters(const TArray<FPoint>& generatedCluster, const TArray<FPoint>& generatedCentroids, FPoint& outCentroid)
{
	TArray<int32> clusterElems;
	clusterElems.SetNum(generatedCentroids.Num());

	//Calculate how many cluster elements centroids has
	for (const FPoint& point : generatedCluster)
	{
		clusterElems[point.cluster] += 1;
	}

	//Get largest centroid id
	int32 clusterMaxElems = -1;
	int32 clusterId = -1;

	for (int32 i = 0; i < clusterElems.Num(); i++)
	{	
		if (clusterMaxElems < clusterElems[i])
		{
			clusterMaxElems = clusterElems[i];
			clusterId = i;
		}
	}

	//Check bounds
	if (generatedCentroids.Num() >= clusterId && clusterId != -1)
	{
		outCentroid = generatedCentroids[clusterId];
	}
}

uint32 UClusterGeneratorComponent::GetNextHandle()
{
	return (uint32)m_CurrentHandle.Increment();
}

void UClusterGeneratorComponent::OnActorSpawned(AActor* SpawnedActor)
{
	AddToActorList(SpawnedActor);
}


void UClusterGeneratorComponent::AddToActorList(AActor* Actor)
{
	if (Actor != nullptr)
	{
		if (const auto* ActorClass = Actor->GetClass())
		{
			if (ActorClass->IsChildOf(ClassTypeToCluster))
			{
				m_ActorMap.Add(Actor->GetActorLabel(), Actor);
			}
		}
	}
}

FClusterGenerationAsyncTask::FClusterGenerationAsyncTask(int32 NumberOfClusters, int32 MaxIterations, TSubclassOf<AActor> ClassTypeToCluster, UWorld* World, const TMap<FString, AActor*>& ActorMap)
	:	m_NumberOfClusters(NumberOfClusters), 
		m_MaxIterations(MaxIterations), 
		m_ClassTypeToCluster(ClassTypeToCluster),
		m_UWorldRef(World),
		m_ActorMap(ActorMap)
{

}


FClusterGenerationAsyncTask::~FClusterGenerationAsyncTask()
{
	RecordTaskDestroyed();
}

void FClusterGenerationAsyncTask::DoWork()
{
	RecordTaskStarted();
	std::vector<FPoint> dataPoints;

	//Fil data points with active actors that will be passed to KMeans algorithm
	for (TMap<FString, AActor*>::TIterator ItRemove = m_ActorMap.CreateIterator(); ItRemove; ++ItRemove)
	{
		if (ItRemove->Value != nullptr || IsValid(ItRemove->Value) == true)
		{
			FPoint point;
			AActor* actorTarget = ItRemove->Value;

			point.location = actorTarget->GetActorLocation();
			point.rotation = actorTarget->GetActorRotation();
			point.guid = actorTarget->GetActorLabel();
			point.name = actorTarget->GetFName();

			dataPoints.emplace_back(MoveTemp(point));
		}
	}

	//If its empty. Don't do anything
	if (dataPoints.empty())
	{
		m_bDataWasBuilt = false;
		RecordTaskFinished();

		return;
	}

	//Init
	KMeans kMeans(m_NumberOfClusters, m_MaxIterations);

	//Fit model
	kMeans.Fit(dataPoints);

	//Assign cluster to each example
	std::vector<FPoint> predData = kMeans.Predict(dataPoints);

	m_GeneratedCluster.Empty();
	m_GeneratedCluster.Append(predData.data(), predData.size());

	m_GeneratedCentroids.Empty();
	m_GeneratedCentroids.Append(kMeans.GetCentroids().data(), kMeans.GetCentroids().size());

	//Sort by cluster id
	m_GeneratedCluster.Sort([](const FPoint& p1, const FPoint& p2)
	{
		return p1.cluster < p2.cluster;
	});

	/*
		looping over clusters and calculate Euclidian distance of
		each point within that cluster from its centroid and
		pick the maximum which is the radius of that cluster
	*/
	for (const auto& point : m_GeneratedCluster)
	{
		FPoint& centroid = m_GeneratedCentroids[point.cluster];
		centroid.radius = std::max(centroid.radius, centroid.Distance(point));
	}

	m_bDataWasBuilt = true;

	RecordTaskFinished();
}

TArray<FPoint> FClusterGenerationAsyncTask::GetGeneratedClusters()
{ 
	return m_GeneratedCluster;
}

TArray<FPoint> FClusterGenerationAsyncTask::GetGeneratedCentroids()
{ 
	return m_GeneratedCentroids; 
}

void FClusterGenerationAsyncTask::SetActorMap(const TMap<FString, AActor*>& actorMap)
{
	m_ActorMap = actorMap;
}

void FClusterGenerationAsyncTask::RecordTaskStarted()
{
	// Record that the task is running and check that it was not running, finished, or destroyed previously.
	const uint32 PreviousState = m_WorkerState.fetch_or(WorkerStateRunning, std::memory_order_relaxed);
	checkf(!(PreviousState & WorkerStateRunning), TEXT("Starting ClusterGeneration worker that is already running!"));
	checkf(!(PreviousState & WorkerStateFinished), TEXT("Starting ClusterGeneration worker that is already finished!"));
	checkf(!(PreviousState & WorkerStateDestroyed), TEXT("Starting ClusterGeneration worker that has been destroyed!"));
}

void FClusterGenerationAsyncTask::RecordTaskFinished()
{
	// Record that the task is finished and check that it was running and not finished or destroyed previously.
	const uint32 PreviousState = m_WorkerState.fetch_xor(WorkerStateRunning | WorkerStateFinished, std::memory_order_relaxed);
	checkf((PreviousState & WorkerStateRunning), TEXT("Finishing ClusterGeneration worker that was not running!"));
	checkf(!(PreviousState & WorkerStateFinished), TEXT("Finishing ClusterGeneration worker that is already finished!"));
	checkf(!(PreviousState & WorkerStateDestroyed), TEXT("Finishing ClusterGeneration worker that has been destroyed!"));
}

void FClusterGenerationAsyncTask::RecordTaskDestroyed()
{
	const uint32 PreviousState = m_WorkerState.fetch_or(WorkerStateDestroyed, std::memory_order_relaxed);
	checkf(!(PreviousState & WorkerStateRunning), TEXT("Destroying ClusterGeneration worker that is still running!"));
	checkf(!(PreviousState & WorkerStateDestroyed), TEXT("Destroying ClusterGeneration worker that has been destroyed previously!"));
}