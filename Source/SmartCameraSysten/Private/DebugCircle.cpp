// Copyright 
#include "DebugCircle.h"

//UE4 Incldues
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Components/LineBatchComponent.h"

ADebugCircle::ADebugCircle(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#if WITH_EDITOR
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
#else
	PrimaryActorTick.bCanEverTick = false;
#endif //WITH_EDITOR
}

void ADebugCircle::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
#if WITH_EDITOR

	FVector Location = GetActorLocation();
	if (Radius > 0.f)
	{
		//DrawCircle(Location, Location + FVector(0, 0, 50.f), Radius, 24, DebugLineColour, false, DebugLineDuration);
		DrawDebugCylinder(GetWorld(), Location, Location + FVector(0, 0, 50.f), Radius, 24, DebugLineColour, false, DebugLineDuration);
	}
#endif 
}

bool ADebugCircle::ShouldTickIfViewportsOnly() const
{
#if WITH_EDITOR
	return true;
#else
	return false;
#endif
}


void ADebugCircle::DrawCircle(FVector const& Start, FVector const& End, float InRadius, int32 Segments, const FColor& Color, bool PersistentLines, float LifeTime, uint8 DepthPriority, float Thickness, FVector YAxis, FVector ZAxis, bool bDrawAxis)
{
	/*
	if (ULineBatchComponent* const LineBatcher = GetWorld()->PersistentLineBatcher)
	{
		// Need at least 4 segments
		Segments = FMath::Max(Segments, 4);

		// Rotate a point around axis to form cylinder segments
		FVector Segment;
		FVector P1, P2, P3, P4;
		const float AngleInc = 360.f / Segments;
		float Angle = AngleInc;

		// Default for Axis is up
		FVector Axis = (End - Start).GetSafeNormal();
		if (Axis.IsZero())
		{
			Axis = FVector(0.f, 0.f, 1.f);
		}

		FVector Perpendicular;
		FVector Dummy;

		Axis.FindBestAxisVectors(Perpendicular, Dummy);

		Segment = Perpendicular.RotateAngleAxis(0, Axis) * InRadius;
		P1 = Segment + Start;
		P3 = Segment + End;

		const float LineLifeTime = DebugLineDuration;

		P4 = Segment + End;

		LineBatcher->DrawLine(P2, P4, Color, DepthPriority, Thickness, LineLifeTime);
		while (Segments--)
		{
			Segment = Perpendicular.RotateAngleAxis(Angle, Axis) * InRadius;
			P2 = Segment + Start;
			LineBatcher->DrawLine(P1, P2, Color, DepthPriority, Thickness, LineLifeTime);
			LineBatcher->DrawLine(P3, P4, Color, DepthPriority, Thickness, LineLifeTime);

			P1 = P2;
			P3 = P4;
			Angle += AngleInc;
		}
	}

	*/
}