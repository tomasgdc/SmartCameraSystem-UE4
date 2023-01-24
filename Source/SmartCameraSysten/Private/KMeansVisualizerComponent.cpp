// Copyright 2019 SIE-CCG


#include "KMeansVisualizerComponent.h"

// Sets default values for this component's properties
UKMeansVisualizerComponent::UKMeansVisualizerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UKMeansVisualizerComponent::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined;
	
	for (const FKMeansVisualizerConfig& config : KMeansVisualizerClusterSettings)
	{
		ACentroidObject* centroidObject = GetWorld()->SpawnActor<ACentroidObject>(CentroidObject, ObjectSpawnLocation, ObjectSpawnRotation, Params);
		check(IsValid(centroidObject));

		centroidObject->SetColour(config.Colour);
		m_Centroids.Add(config.ClusterId, centroidObject);
	}

	for (int32 i = 0; i < MaxNumberOfClusterObjects; i++)
	{
		AClusterObject* clusterObject = GetWorld()->SpawnActor<AClusterObject>(ClusterObject, ObjectSpawnLocation, ObjectSpawnRotation, Params);
		check(IsValid(clusterObject));

		m_Clusters.Add(clusterObject);
	}
}

void UKMeansVisualizerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	m_Centroids.Empty();
	m_Clusters.Empty();
}

// Called every frame
void UKMeansVisualizerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Centroids
	for (const TPair<int32, ACentroidObject*>& pair : m_Centroids)
	{
		ACentroidObject* centroidObject = pair.Value;
		FVector previousDesiredLocation = centroidObject->GetActorLocation();
		FVector newDesiredLocation = centroidObject->GetDesiredLocation();

		FVector desiredLoc = FMath::VInterpTo(previousDesiredLocation, newDesiredLocation, DeltaTime, InterpolateSpeed);
		centroidObject->SetActorLocation(desiredLoc);

		FVector previousScale = centroidObject->GetActorScale3D();

		//Calculated difference between debug and visual circle actor and divided by that difference.. Taaddaa. Magicccc
		float magicNUmber = 49.0f;
		FVector newScale(centroidObject->GetRadius() / magicNUmber);

		FVector desiredScale = FMath::VInterpTo(previousScale, newScale, DeltaTime, InterpolateSpeed);
		centroidObject->SetActorScale3D(desiredScale);
	}

	//Clusters
	for (AClusterObject* clusterObject : m_Clusters)
	{
		FVector previousDesiredLocation = clusterObject->GetActorLocation();
		FVector newDesiredLocation = clusterObject->GetDesiredLocation();

		FVector desiredLoc = FMath::VInterpTo(previousDesiredLocation, newDesiredLocation, DeltaTime, InterpolateSpeed);
		clusterObject->SetActorLocation(desiredLoc);
	}

	//m_IsClusterPendingUpdate = false;
}


void UKMeansVisualizerComponent::UpdateClusterObjects(const TArray<FPoint>& centroids, const TArray<FPoint>& clusters)
{
	UpdateCentroidsObjects(centroids);
	UpdateClustersObjects(clusters);

	//m_IsClusterPendingUpdate = true;
}

void UKMeansVisualizerComponent::UpdateCentroidsObjects(const TArray<FPoint>& centroids)
{
	for(int32 clusterId = 0; clusterId < centroids.Num(); clusterId++)
	{
		const FPoint& point = centroids[clusterId];

		ACentroidObject** centroidOjbect = m_Centroids.Find(clusterId);
		if (centroidOjbect != nullptr && IsValid((*centroidOjbect)))
		{
			(*centroidOjbect)->SetRadius(point.radius);
			(*centroidOjbect)->SetDesiredLocation(point.location);
		}
	}
}

void UKMeansVisualizerComponent::UpdateClustersObjects(const TArray<FPoint>& clusters)
{
	FLinearColor colour = FLinearColor::White;

	for (int32 i = 0; i < clusters.Num(); i++)
	{
		const FPoint& point = clusters[i];

		ACentroidObject** centroidOjbect = m_Centroids.Find(point.cluster);
		if (centroidOjbect != nullptr && IsValid((*centroidOjbect)))
		{
			colour = (*centroidOjbect)->GetColour();
		}

		AClusterObject* clusterObject = m_Clusters[i];
		clusterObject->SetDesiredLocation(point.location);
		clusterObject->SetColour(colour);
	}
}


TArray<ACentroidObject*> UKMeansVisualizerComponent::GetCentroids() const
{
	TArray< ACentroidObject*> centroids;
	m_Centroids.GenerateValueArray(centroids);

	return centroids;
}

TArray<AClusterObject*> UKMeansVisualizerComponent::GetClusters() const
{
	return m_Clusters;
}