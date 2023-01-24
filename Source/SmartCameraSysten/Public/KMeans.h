#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Async/AsyncWork.h"
#include "Misc/ScopeLock.h"
#include <vector>
#include <atomic>

#include "KMeans.generated.h"

USTRUCT(BlueprintType)
struct SMARTCAMERASYSTEN_API FPoint
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "ClusterGenerator")
	FVector location;

	UPROPERTY(BlueprintReadWrite, Category = "ClusterGenerator")
	FRotator rotation;

	UPROPERTY(BlueprintReadWrite, Category = "ClusterGenerator")
	int32 cluster = -1;

	UPROPERTY(BlueprintReadWrite, Category = "ClusterGenerator")
	float minDist = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "ClusterGenerator")
	float radius = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "ClusterGenerator")
	FString guid;

	UPROPERTY(BlueprintReadWrite, Category = "ClusterGenerator")
	FName name;

	FPoint() = default;


	explicit FPoint(const FPoint& inCopy) :
		location(inCopy.location),
		rotation(inCopy.rotation),
		cluster(inCopy.cluster),
		minDist(inCopy.minDist),
		radius(inCopy.radius),
		guid(inCopy.guid),
		name(inCopy.name)
	{

	}

	explicit FPoint(FVector inLocation) :
		location(inLocation)
	{

	}

	float Distance(const FPoint& p) const
	{
		return sqrtf((p.location.X - location.X) * (p.location.X - location.X) + (p.location.Y - location.Y) * (p.location.Y - location.Y) + (p.location.Z - location.Z) * (p.location.Z - location.Z));
	}

};

class SMARTCAMERASYSTEN_API KMeans
{
private:
	std::vector<FPoint> m_Centroids;

	int32 m_NumberClusters = 0;
	int32 m_MaxIter = 0;
	int32 m_RandomState = 0;

public:
	KMeans() = default;

	/*
	*
	*/
	KMeans(int32 n_clusters, int32 max_iter = 100, int32 random_state = 123);

	/*
	*
	*/
	void Fit(const std::vector<FPoint>& data);

	/*
	*
	*/
	std::vector<FPoint> Predict(const std::vector<FPoint>& data);

	/*
	*
	*/
	const std::vector<FPoint>& GetCentroids() const { return m_Centroids; }

protected:
	/*
	*
	*/
	std::vector<FPoint> InitializeCentroids(const std::vector<FPoint>& points);

	/*
	*
	*/
	std::vector<FPoint>  ComputeCentroids(const std::vector<FPoint>& points, const std::vector<FPoint>& distance);

	/*
	*
	*/
	std::vector<FPoint>  ComputeDistance(const std::vector<FPoint>& points, const std::vector<FPoint>& centroids);
};

class SMARTCAMERASYSTEN_API FClusterGenerationAsyncTask : public FNonAbandonableTask
{
public:

	enum EWorkerState : uint32
	{
		WorkerStateNone = 0,
		WorkerStateRunning = 1 << 0,
		WorkerStateFinished = 1 << 1,
		WorkerStateDestroyed = 1 << 2,
	};

	FClusterGenerationAsyncTask(int32 NumberOfClusters, int32 MaxIterations, TSubclassOf<AActor> ClassTypeToCluster,  UWorld* World, const TMap<FString, AActor*>& ActorMap);
	virtual ~FClusterGenerationAsyncTask();

	void DoWork();

	TArray<FPoint> GetGeneratedClusters();
	TArray<FPoint> GetGeneratedCentroids();
	void SetActorMap(const TMap<FString, AActor*>& actorMap);

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FClusterGenerationAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	FORCEINLINE bool WasDataBuilt() const
	{
		return m_bDataWasBuilt;
	}

protected:
	void RecordTaskStarted();
	void RecordTaskFinished();
	void RecordTaskDestroyed();

private:
	std::atomic<uint32>		m_WorkerState{ WorkerStateNone };

	int32					m_NumberOfClusters;
	int32					m_MaxIterations;

	/** true if we had to build the data */
	bool					m_bDataWasBuilt = false;

	TSubclassOf<AActor>		m_ClassTypeToCluster;

	UWorld*					m_UWorldRef;

	TMap<FString, AActor*>	m_ActorMap;
	TArray<FPoint>			m_GeneratedCluster;
	TArray<FPoint>			m_GeneratedCentroids;
};

UCLASS(ClassGroup = (Gameplay), Meta = (BlueprintSpawnableComponent, DisplayName = "ClusterGeneratorComponent"))
class SMARTCAMERASYSTEN_API UClusterGeneratorComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Config | ClusterGenerator")
	int32 NumberOfClusters = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Config | ClusterGenerator")
	int32 MaxIterations = 2;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config | ClusterGenerator")
	TSubclassOf<AActor> ClassTypeToCluster;

public:
	UClusterGeneratorComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*
	*	
	*/
	//UFUNCTION(BlueprintCallable, Category = "SmartCameraSystem | ClusterGenerator | Functions")
	uint32 GenerateAsynchronous();

	/*
	*	
	*/
	bool GetAsynchronousResults(uint32 Handle, TArray<FPoint>& generatedCluster, TArray<FPoint>& generatedCentroids);

	/*
	* 
	*/
	bool PollAsynchronousCompletion(uint32 Handle);

	/*
	*
	*/
	void WaitAsynchronousCompletion(uint32 Handle);

	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystem | ClusterGenerator | Functions")
	void GenerateSynchronous(TArray<FPoint>& generatedCluster, TArray<FPoint>& generatedCentroids);

	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystem | ClusterGenerator | Functions")
	void UpdateClustersPosition(const TArray<FPoint>& inGeneratedCluster, TArray<FPoint>& outUpdatedGenertaedCluster);

	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystem | ClusterGenerator | Functions")
	void UpdateCentroids(const TArray<FPoint>& inGeneratedCluster, const TArray<FPoint>& inGeneratedCentroids, TArray<FPoint>& outUpdatedCentroids);

	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystem | ClusterGenerator | Functions")
	void GetCentroidWithMostClusters(const TArray<FPoint>& generatedCluster, const TArray<FPoint>& generatedCentroids, FPoint& outCentroid);

protected:
	uint32 GetNextHandle();

	/*
	*	@param SpawnedActor
	*/
	void OnActorSpawned(AActor* SpawnedActor);

	/*
	* @param Actor
	*/
	void AddToActorList(AActor* Actor);

private:
	TMap<FString, AActor*>	m_ActorMap;
	FCriticalSection		m_CriticalSection;

	/** Delegate thats holds delegate handle  **/
	FDelegateHandle			m_ActorSpawnedDelegateHandle;

	/** Counter used to produce unique handles **/
	FThreadSafeCounter			m_CurrentHandle;

	/** Object used for synchronization via a scoped lock **/
	FCriticalSection			m_SynchronizationObject;

	/** Map of handle to pending task **/
	TMap<uint32, FAsyncTask<FClusterGenerationAsyncTask>*>	m_PendingTasks;
};