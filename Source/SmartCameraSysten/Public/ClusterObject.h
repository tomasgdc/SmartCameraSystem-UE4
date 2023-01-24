// Copyright 2019 SIE-CCG

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClusterObject.generated.h"

UCLASS()
class SMARTCAMERASYSTEN_API AClusterObject : public AActor
{
	GENERATED_BODY()
	
private:
	static const FName kMeshComponentName;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Settings | Material")
	FName ColourParameterName;

	UPROPERTY(EditDefaultsOnly, Category = "Settings | Material")
	int32 MaterialID;

	UPROPERTY(BlueprintReadWrite, Category = "SmartCameraSystems | ClusterOjbect | Material")
	class UMaterialInstanceDynamic* MaterialInstance;

	UPROPERTY(VisibleDefaultsOnly, Category = "Settings")
	class UStaticMeshComponent* MeshComponent;

public:	
	// Sets default values for this actor's properties
	AClusterObject(const FObjectInitializer& ObjectInitializer);

	void SetDesiredLocation(FVector position);
	void SetClusterId(int32 clusterId);
	void SetOwnerGuid(FGuid guid);

	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystems | CentroidObject | Functions")
	void SetColour(FLinearColor color);

	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystems | ClusterObject | Functions")
	int32 GetClusterId() const;

	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystems | ClusterObject | Functions")
	FGuid GetOwnerGuid() const;

	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystems | ClusterObject | Functions")
	FVector GetDesiredLocation() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	int32			m_ClusterId;
	FGuid			m_OwnerGuid;
	FLinearColor	m_Colour;
	FVector			m_DesiredLocation;
};
