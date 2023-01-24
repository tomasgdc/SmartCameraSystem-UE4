// Copyright 2019 SIE-CCG

#pragma once

#include "CoreMinimal.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Camera/CameraActor.h"
#include "KMeansDebugWidget.h"
#include "KMeansDebugCamera.generated.h"

UCLASS()
class SMARTCAMERASYSTEN_API AKMeansDebugCamera : public ACameraActor
{
	GENERATED_BODY()
	
private:
	static const FName kSceneCapture;
	static const FName kKMeansVisualizerComponentName;
	static const FName kTextureRenderTargetName;
	static const FName kTextureParameterName;

	UPROPERTY(VisibleAnywhere, Transient, /*BlueprintGetter = GetTexture,*/ Category = "Debug", meta = (DisplayName = "TextureRenderTarget"))
	UTextureRenderTarget2D* m_TextureRenderTarget;

	/** The dynamic instance of the material that the render target is attached to. */
	UPROPERTY(VisibleAnywhere, Transient, /*BlueprintGetter = GetMaterialInstance,*/ Category = "Debug", meta = (DisplayName = "KmeansDebugMaterialInstance"))
	UMaterialInstanceDynamic* m_KmeansDebugMaterialInstance;

	UPROPERTY(VisibleAnywhere, Transient, /*BlueprintGetter = GetKMeansDebugWidget,*/ Category = "Debug", meta = (DisplayName = "KmeansDebugMaterialInstance"))
	UKMeansDebugWidget* m_KMeansDebugWidgetInstance;

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	class USceneCaptureComponent2D* SceneCapture;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	class UKMeansVisualizerComponent* KMeansVisualizerComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	UMaterialInterface* KMeansDebugMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<UKMeansDebugWidget> DebugWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	int32 TextureWidth;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	int32 TextureHeight;

public:	

	// Sets default values for this actor's properties
	AKMeansDebugCamera(const FObjectInitializer& ObjectInitializer);

	/*
	*	@return
	*/
	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystem | KMeansDebugCamera | Functions")
	UTexture* GetTexture() const;


	/*
	*	@return
	*/
	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystem | KMeansDebugCamera | Functions")
	UMaterialInstanceDynamic* GetMaterialInstance() const;


	/*
	*	@return
	*/
	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystem | KMeansDebugCamera | Functions")
	UKMeansDebugWidget* GetKMeansDebugWidget() const;

	/*
	*	This will be called via BP
	*/
	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystem | KMeansDebugWidget | Functions")
	void DisplayDebugWidget();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	*	@return
	*/
	UTextureRenderTarget2D* GenerateTexture();

	/*
	*	
	*/
	void SetCaptureComponentShowOnlyActors();

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
