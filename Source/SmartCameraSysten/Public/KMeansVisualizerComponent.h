// Copyright 2019 SIE-CCG

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HAL/ThreadSafeBool.h"
#include "KMeans.h"
#include "CentroidObject.h"
#include "ClusterObject.h"
#include "KMeansVisualizerComponent.generated.h"


USTRUCT(BlueprintType)
struct FKMeansVisualizerConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Config")
	FLinearColor	Colour;

	UPROPERTY(EditAnywhere, Category = "Config")
	int32	ClusterId;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SMARTCAMERASYSTEN_API UKMeansVisualizerComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float InterpolateSpeed = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	int32 MaxNumberOfClusterObjects = 20;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	FVector ObjectSpawnLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	FRotator ObjectSpawnRotation;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TArray<FKMeansVisualizerConfig> KMeansVisualizerClusterSettings;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<ACentroidObject> CentroidObject;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<AClusterObject> ClusterObject;



public:	
	// Sets default values for this component's properties
	UKMeansVisualizerComponent();

	/*
	*	@parma centroids
	*	@param clusters
	*/
	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystem | KMeansVisualizerComponent | Function")
	void UpdateClusterObjects(const TArray<FPoint>& centroids, const TArray<FPoint>& clusters);


	/*
	*	
	*	@return array of centroids
	*/
	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystem | KMeansVisualizerComponent | Function")
	TArray<ACentroidObject*> GetCentroids() const;

	/*
	*
	*	@param array of clusters
	*/
	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystem | KMeansVisualizerComponent | Function")
	TArray<AClusterObject*> GetClusters() const;

protected:
	
	/*
	*	@param centroids
	*/
	void UpdateCentroidsObjects(const TArray<FPoint>& centroids);

	/*
	*	@param clusters
	*/
	void UpdateClustersObjects(const TArray<FPoint>& clusters);

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TMap<int32, ACentroidObject*> m_Centroids;

	UPROPERTY()
	TArray<AClusterObject*> m_Clusters;

	FThreadSafeBool m_IsClusterPendingUpdate = false;
};
