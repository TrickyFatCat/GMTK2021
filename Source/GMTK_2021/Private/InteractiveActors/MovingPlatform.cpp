// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActors/MovingPlatform.h"


AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>("PlatformMesh");
	SetRootComponent(PlatformMesh);

	MovementTimeline = CreateDefaultSubobject<UTimelineComponent>("MovementTimeline");
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (TargetPoints.Num() == 2)
	{
		for (const auto Actor : TargetPoints)
		{
			if (!Actor) continue;
			PathLocations.Add(Actor->GetActorLocation());
		}
	}

	if (PathLocations.Num() == 2)
	{
		SetActorLocation(PathLocations[CurrentPointIndex]);

		if (MovementCurve)
		{
			FOnTimelineFloat MovementProgress;
			MovementProgress.BindUFunction(this, FName("SetPlatformLocation"));
			MovementTimeline->AddInterpFloat(MovementCurve, MovementProgress);
			FOnTimelineEvent MovementFinished;
			MovementFinished.BindUFunction(this, FName("FinishMovement"));
			MovementTimeline->SetTimelineFinishedFunc(MovementFinished);
			CalculatePlayRate();
		}
	}
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AMovingPlatform::StartMoving()
{
	if (PathLocations.Num() < 2 || PathLocations.Num() > 2) return false;
	
	InitialLocation = GetActorLocation();
	MovementTimeline->PlayFromStart();
	return true;
}

void AMovingPlatform::CalculatePlayRate() const
{
	if (!MovementCurve) return;

	MovementTimeline->SetPlayRate(1.f / TravelTime);
}

void AMovingPlatform::SetPlatformLocation(const float PathProgress)
{
	const int32 TargetPointIndex = CurrentPointIndex == 0 ? 1 : 0;
	const FVector NewLocation = FMath::Lerp(InitialLocation, PathLocations[TargetPointIndex], PathProgress);
	SetActorLocation(NewLocation);
}

void AMovingPlatform::FinishMovement()
{
	CurrentPointIndex = CurrentPointIndex == 0 ? 1 : 0;
	OnPlatformStopMoving.Broadcast();
}
