// Copyright 2019 SIE-CCG

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CentroidObject.generated.h"

UCLASS()
class SMARTCAMERASYSTEN_API ACentroidObject : public AActor
{
	GENERATED_BODY()

private:
	static const FName kMeshComponentName;

protected:

	UPROPERTY(EditDefaultsOnly,  Category = "Settings | Material")
	FName ColourParameterName;

	UPROPERTY(EditDefaultsOnly, Category = "Settings | Material")
	FName RadiusParameterName;

	UPROPERTY(EditDefaultsOnly, Category = "Settings | Material")
	int32 MaterialID;

	UPROPERTY(BlueprintReadWrite, Category = "SmartCameraSystems | CentroidObject | Material")
	class UMaterialInstanceDynamic* MaterialInstance;

	UPROPERTY(VisibleDefaultsOnly, Category = "Setting | Mesh")
	class UStaticMeshComponent* MeshComponent;

public:	
	// Sets default values for this actor's properties
	ACentroidObject(const FObjectInitializer& ObjectInitializer);

	void SetDesiredLocation(FVector position);

	void SetClusterId(int32 clusterId);

	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystems | CentroidObject | Functions")
	void SetRadius(float radius);

	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystems | CentroidObject | Functions")
	void SetColour(FLinearColor color);

	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystems | CentroidObject | Functions")
	FVector GetDesiredLocation() const;

	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystems | CentroidObject | Functions")
	int32 GetClusterId() const;

	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystems | CentroidObject | Functions")
	float GetRadius() const;

	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystems | CentroidObject | Functions")
	FLinearColor GetColour() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	int32			m_ClusterId = 0;
	float			m_Radius = 0.0f;
	FLinearColor	m_Colour;
	FVector			m_DesiredLocation;
};
