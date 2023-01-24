// Copyright 2019 SIE-CCG
#include "ClusterObject.h"

//Ohter
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "ActorFactories/ActorFactoryBasicShape.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"

const FName AClusterObject::kMeshComponentName(TEXT("MeshComponent"));

// Sets default values
AClusterObject::AClusterObject(const FObjectInitializer& ObjectInitializer)
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
void AClusterObject::BeginPlay()
{
	Super::BeginPlay();

	if (MeshComponent != nullptr && MaterialID < MeshComponent->GetNumMaterials())
	{
		MaterialInstance = MeshComponent->CreateAndSetMaterialInstanceDynamic(MaterialID);
	}
}

// Called every frame
void AClusterObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AClusterObject::SetDesiredLocation(FVector position)
{
	m_DesiredLocation = position;
}

void AClusterObject::SetClusterId(int32 clusterId)
{
	m_ClusterId = clusterId;
}

void AClusterObject::SetOwnerGuid(FGuid guid)
{
	m_OwnerGuid = guid;
}

void AClusterObject::SetColour(FLinearColor colour)
{
	m_Colour = colour;
	if (MaterialInstance != nullptr)
	{
		MaterialInstance->SetVectorParameterValue(ColourParameterName, m_Colour);
	}
}

int32 AClusterObject::GetClusterId() const
{
	return m_ClusterId;
}

FGuid AClusterObject::GetOwnerGuid() const
{
	return m_OwnerGuid;
}

FVector AClusterObject::GetDesiredLocation() const
{
	return m_DesiredLocation;
}