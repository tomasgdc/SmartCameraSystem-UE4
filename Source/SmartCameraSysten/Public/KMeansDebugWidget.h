// Copyright 2019 SIE-CCG

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KMeansDebugWidget.generated.h"

/**
 * 
 */
UCLASS()
class SMARTCAMERASYSTEN_API UKMeansDebugWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Config")
	class UCanvasRenderTarget2D* MapCanvas;

public:
	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystem | KMeansDebugWidget | Functions")
	void Setup();

	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystem | KMeansDebugWidget | Functions")
	void OnUpdate(float elapsedTime);

	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystem | KMeansDebugWidget | Functions")
	void OnRender(UCanvas* Canvas, int32 Width, int32 Height);

	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystem | KMeansDebugWidget | Functions")
	void SetTexture(UTexture* Texture);

	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystem | KMeansDebugWidget | Functions")
	void SetMaterialInstance(UMaterialInstanceDynamic* MaterialInstance);

	UFUNCTION(BlueprintImplementableEvent, Category = "SmartCameraSystem | KMeansDebugWidget | Events")
	void OnDrawTexture(UCanvas* Canvas, UTexture* RenderTexture, FVector2D ScreenPosition, FVector2D ScreenSize, FVector2D CoordinatePosition, FVector2D CoordinateSize = FVector2D::UnitVector, FLinearColor RenderColor = FLinearColor::White, EBlendMode BlendMode = BLEND_Translucent, float Rotation = 0.f, FVector2D PivotPoint = FVector2D(0.5f, 0.5f));

	UFUNCTION(BlueprintImplementableEvent, Category = "SmartCameraSystem | KMeansDebugWidget | Events")
	void SetMaterialBrush();

	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystem | KMeansDebugWidget | Functions")
	UTexture* GetTexture() const;

	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystem | KMeansDebugWidget | Functions")
	UMaterialInstanceDynamic* GetMaterialInstance() const;

private:
	UPROPERTY(VisibleAnywhere, Transient, Category = "Debug", meta = (DisplayName = "KmeansDebugMaterialInstance"))
	class UTexture* m_KMeansDebguTexture;

	UPROPERTY(VisibleAnywhere, Transient, Category = "Debug", meta = (DisplayName = "KmeansDebugMaterialInstance"))
	UMaterialInstanceDynamic* m_KMeansDebugWidgetInstance;

	bool m_bInitialised = false;
};
