// Copyright 2019 SIE-CCG
#include "KMeansDebugCamera.h"
#include "KMeansVisualizerComponent.h"

//other
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

const FName AKMeansDebugCamera::kSceneCapture(TEXT("SceneCaptureComponent"));
const FName AKMeansDebugCamera::kKMeansVisualizerComponentName(TEXT("KMeansVisualizerComponent"));
const FName AKMeansDebugCamera::kTextureRenderTargetName(TEXT("KMeansDebugTexture"));
const FName AKMeansDebugCamera::kTextureParameterName(TEXT("RenderTarget"));

// Sets default values
AKMeansDebugCamera::AKMeansDebugCamera(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneCapture = ObjectInitializer.CreateDefaultSubobject<USceneCaptureComponent2D>(this, kSceneCapture);
	SceneCapture->bCaptureOnMovement = false;
	SceneCapture->bCaptureEveryFrame = false;
	SceneCapture->TextureTarget = nullptr;
	SceneCapture->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//SceneCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	
#if ENGINE_MINOR_VERSION > 24
	SceneCapture->bUseRayTracingIfEnabled = false;
#endif

	KMeansVisualizerComponent = ObjectInitializer.CreateDefaultSubobject<UKMeansVisualizerComponent>(this, kKMeansVisualizerComponentName);
}

// Called when the game starts or when spawned
void AKMeansDebugCamera::BeginPlay()
{
	Super::BeginPlay();

	//Cleanup existing RTT
	if (m_TextureRenderTarget != nullptr && m_TextureRenderTarget->IsValidLowLevel())
	{
		m_TextureRenderTarget->BeginDestroy();
		GetWorld()->ForceGarbageCollection();
	}

	m_TextureRenderTarget = GenerateTexture();
	SceneCapture->TextureTarget = m_TextureRenderTarget;

	if (KMeansDebugMaterial)
	{
		m_KmeansDebugMaterialInstance = UMaterialInstanceDynamic::Create(KMeansDebugMaterial, this);
		check(IsValid(m_KmeansDebugMaterialInstance))

		m_KmeansDebugMaterialInstance->SetTextureParameterValue(kTextureParameterName, m_TextureRenderTarget);
	}

	if (DebugWidget)
	{
		m_KMeansDebugWidgetInstance = CreateWidget<UKMeansDebugWidget>(GetWorld(), DebugWidget);
		check(IsValid(m_KMeansDebugWidgetInstance))

		//m_KMeansDebugWidgetInstance->SetKMeansDebugTexture(m_TextureRenderTarget);
		m_KMeansDebugWidgetInstance->SetMaterialInstance(m_KmeansDebugMaterialInstance);
		m_KMeansDebugWidgetInstance->SetMaterialBrush();
	}	

	SetCaptureComponentShowOnlyActors();
}

// Called every frame
void AKMeansDebugCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SceneCapture->CaptureScene();
}

UTexture* AKMeansDebugCamera::GetTexture() const
{
	return SceneCapture->TextureTarget;
}

UMaterialInstanceDynamic* AKMeansDebugCamera::GetMaterialInstance() const
{
	return m_KmeansDebugMaterialInstance;
}

UTextureRenderTarget2D* AKMeansDebugCamera::GenerateTexture()
{
	UTextureRenderTarget2D* TextureRenderTarget = NewObject<UTextureRenderTarget2D>(this, kTextureRenderTargetName);
	check(IsValid(TextureRenderTarget));

	TextureRenderTarget->TargetGamma = 1.0f;
	TextureRenderTarget->bAutoGenerateMips = false;
	TextureRenderTarget->InitCustomFormat(TextureWidth, TextureHeight, PF_B8G8R8A8, false);
	TextureRenderTarget->UpdateResourceImmediate();

	return TextureRenderTarget;
}

UKMeansDebugWidget* AKMeansDebugCamera::GetKMeansDebugWidget() const
{
	return m_KMeansDebugWidgetInstance;
}

void AKMeansDebugCamera::DisplayDebugWidget()
{
	// Flip | Flop
	if (m_KMeansDebugWidgetInstance->IsInViewport())
	{
		m_KMeansDebugWidgetInstance->RemoveFromViewport();
	}
	else
	{ 
		m_KMeansDebugWidgetInstance->AddToViewport();
	}
}

void AKMeansDebugCamera::SetCaptureComponentShowOnlyActors()
{
	TArray<AActor*> actors;
	TArray< ACentroidObject*> centroids = KMeansVisualizerComponent->GetCentroids();
	TArray<AClusterObject*> clusters = KMeansVisualizerComponent->GetClusters();

	check(centroids.Num() != 0);
	check(clusters.Num() != 0);

	actors.Append(centroids);
	actors.Append(clusters);

	
	SceneCapture->ShowOnlyActors = actors;
}