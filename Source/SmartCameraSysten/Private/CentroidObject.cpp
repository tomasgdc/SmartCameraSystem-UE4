// Copyright 2019 SIE-CCG
#include "CentroidObject.h"

//Ohter
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "ActorFactories/ActorFactoryBasicShape.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"

const FName ACentroidObject::kMeshComponentName(TEXT("MeshComponent"));

// Sets default values
ACentroidObject::ACentroidObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, kMeshComponentName);
	MeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	MeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	/*
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneVisualAsset(*UActorFactoryBasicShape::BasicCube.ToString());
	if (PlaneVisualAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(PlaneVisualAsset.Object);
		MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		MeshComponent->SetWorldScale3D(FVector(1.0f));
	}
	*/
}

// Called when the game starts or when spawned
void ACentroidObject::BeginPlay()
{
	Super::BeginPlay();
	
	if (MeshComponent != nullptr && MaterialID < MeshComponent->GetNumMaterials())
	{
		MaterialInstance = MeshComponent->CreateAndSetMaterialInstanceDynamic(MaterialID);
	}
}

// Called every frame
void ACentroidObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACentroidObject::SetDesiredLocation(FVector position)
{
	m_DesiredLocation = position;
}

void ACentroidObject::SetClusterId(int32 clusterId)
{
	m_ClusterId = clusterId;
}

void ACentroidObject::SetRadius(float radius)
{
	m_Radius = radius;
	if (MaterialInstance != nullptr)
	{
		MaterialInstance->SetScalarParameterValue(RadiusParameterName, m_Radius);
	}
}

void ACentroidObject::SetColour(FLinearColor colour)
{
	m_Colour = colour;
	if (MaterialInstance != nullptr)
	{
		MaterialInstance->SetVectorParameterValue(ColourParameterName, m_Colour);
	}
}

FVector ACentroidObject::GetDesiredLocation() const
{
	return m_DesiredLocation;
}

int32 ACentroidObject::GetClusterId() const
{
	return m_ClusterId;
}

float ACentroidObject::GetRadius() const
{
	return m_Radius;
}

FLinearColor ACentroidObject::GetColour() const
{
	return m_Colour;
}