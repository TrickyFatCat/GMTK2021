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

	if (PathPoints.Num() > 0)
	{
		for (const auto Actor : PathPoints)
		{
			PathLocations.Add(Actor->GetActorLocation());
		}

		if (MovementCurve)
		{
			FOnTimelineFloat MovementProgress;
			MovementProgress.BindUFunction(this, FName("SetPlatformLocation"));
			MovementTimeline->AddInterpFloat(MovementCurve, MovementProgress);
			FOnTimelineEvent MovementFinished;
			MovementFinished.BindUFunction(this, FName("FinishMovement"));
			MovementTimeline->SetTimelineFinishedFunc(MovementFinished);
			CalculatePlayRate();
			StartMoving();
		}
	}
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AMovingPlatform::StartMoving()
{
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
	const FVector NewLocation = FMath::Lerp(InitialLocation, PathLocations[0], PathProgress);
	SetActorLocation(NewLocation);
}

void AMovingPlatform::FinishMovement()
{
	UE_LOG(LogTemp, Error, TEXT("Movement Finished"));
}
