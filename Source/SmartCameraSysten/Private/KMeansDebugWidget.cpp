// Copyright 2021 SIE
#include "KMeansDebugWidget.h"

//Engine includes
#include "Engine/Canvas.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"

void UKMeansDebugWidget::Setup()
{
	MapCanvas = Cast<UCanvasRenderTarget2D>(UCanvasRenderTarget2D::CreateCanvasRenderTarget2D(GetWorld(), UCanvasRenderTarget2D::StaticClass(), 256, 256));
	if (MapCanvas)
	{
		MapCanvas->OnCanvasRenderTargetUpdate.AddDynamic(this, &UKMeansDebugWidget::OnRender);
	}
}

void UKMeansDebugWidget::OnUpdate(float elapsedTime)
{

}

void UKMeansDebugWidget::OnRender(UCanvas* Canvas, int32 Width, int32 Height)
{
	UTexture* debugTexture = GetTexture();
	if(debugTexture)
	{ 
		Canvas->K2_DrawTexture(debugTexture, FVector2D(0, 0), FVector2D(Width, Height), FVector2D::ZeroVector);
	}
}

void UKMeansDebugWidget::SetTexture(UTexture* Texture)
{
	m_KMeansDebguTexture = Texture;
}

void UKMeansDebugWidget::SetMaterialInstance(UMaterialInstanceDynamic* MaterialInstance)
{
	m_KMeansDebugWidgetInstance = MaterialInstance;
}

UTexture* UKMeansDebugWidget::GetTexture() const
{
	return m_KMeansDebguTexture;
}

UMaterialInstanceDynamic* UKMeansDebugWidget::GetMaterialInstance() const
{
	return m_KMeansDebugWidgetInstance;
}