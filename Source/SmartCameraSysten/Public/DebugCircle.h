// Copyright 
#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "DebugCircle.generated.h"

/**
 * 
 */
UCLASS()
class SMARTCAMERASYSTEN_API ADebugCircle : public ATargetPoint
{
	GENERATED_BODY()
protected:

	/** A radius suggesting an actor is within our range */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float Radius = 250.f;

	/** The height at which the waypoint line is rendered */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	FVector DebugLineOffset = FVector(0, 0, 100.f);

	/** The thickness of the waypoint line */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	float DebugLineThickness = 8.0f;

	/** The colour of the waypoint line */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	FColor DebugLineColour = FColor::Green;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	float DebugLineDuration = -1.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	float DebugArrowSize = 2000.f;


public:

	ADebugCircle(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaSeconds) override;
	virtual bool ShouldTickIfViewportsOnly() const override;

	UFUNCTION(BlueprintCallable, Category = "SmartCameraSystem | DebugCircle | Functions")
	void DrawCircle(FVector const& Start, FVector const& End, float InRadius, int32 Segments, const FColor& Color, bool bPersistentLines = false, float LifeTime = -1.f, uint8 DepthPriority = 0, float Thickness = 0.f, FVector YAxis = FVector(0.f, 1.f, 0.f), FVector ZAxis = FVector(0.f, 0.f, 1.f), bool bDrawAxis = true);
};
