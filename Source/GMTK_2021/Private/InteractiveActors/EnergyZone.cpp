// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActors/EnergyZone.h"


AEnergyZone::AEnergyZone()
{
	PrimaryActorTick.bCanEverTick = true;

	EnergyTrigger = CreateDefaultSubobject<UEnergyTrigger>("EnergyTrigger");
	SetRootComponent(EnergyTrigger);
	EnergyTrigger->SetSphereRadius(64.f);

	ZoneEffect = CreateDefaultSubobject<UStaticMeshComponent>("ZoneEffect");
	ZoneEffect->SetupAttachment(GetRootComponent());

	AnimationTimeline = CreateDefaultSubobject<UTimelineComponent>("AnimationTimeline");
}

void AEnergyZone::BeginPlay()
{
	Super::BeginPlay();
	EnergyTrigger->bIsIncreasingEnergy = bIsIncreasingEnergy;
	
	if (InitialState == EZoneState::Transition)
	{
		InitialState = EZoneState::Inactive;
	}

	CurrentState = InitialState;

	switch (CurrentState)
	{
		case EZoneState::Active:
			SetZoneScale(1.f);
			EnergyTrigger->EnableTrigger();
			break;
		case EZoneState::Inactive:
			SetZoneScale(0.f);
			EnergyTrigger->DisableTrigger();
			break;
	}

	OnStateChanged(CurrentState);

	if (AnimationCurve)
	{
		FOnTimelineFloat AnimationProgress;
        AnimationProgress.BindUFunction(this, FName("SetZoneScale"));
        AnimationTimeline->AddInterpFloat(AnimationCurve, AnimationProgress);
        FOnTimelineEvent AnimationFinish;
        AnimationFinish.BindUFunction(this, FName("FinishTransition"));
        AnimationTimeline->SetTimelineFinishedFunc(AnimationFinish);
        CalculatePlayRate();
	}
}

void AEnergyZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AEnergyZone::ActivateZone()
{
	if (IsCurrentState(EZoneState::Transition) || IsCurrentState(EZoneState::Active)) return false;

	StartTransition();
	return true;
}

bool AEnergyZone::DeactivateZone()
{
	if (IsCurrentState(EZoneState::Transition) || IsCurrentState(EZoneState::Inactive)) return false;

	StartTransition();
	return false;
}

void AEnergyZone::ChangeState(const EZoneState NewState)
{
	if (IsCurrentState(NewState)) return;

	CurrentState = NewState;

	if (IsCurrentState(EZoneState::Inactive))
	{
		EnergyTrigger->DisableTrigger();
		ZoneEffect->SetHiddenInGame(true);
	}

	OnStateChanged(NewState);
	OnChangeState.Broadcast(NewState);
}

void AEnergyZone::SetAnimationDuration(const float NewDuration)
{
	if (NewDuration <= 0.f) return;
	
	AnimationDuration = NewDuration;
	CalculatePlayRate();
}

void AEnergyZone::CalculatePlayRate() const
{
	if (AnimationDuration <= 0.f) return;
	
	AnimationTimeline->SetPlayRate(1 / AnimationDuration);
}

void AEnergyZone::StartTransition()
{
	TargetState = IsCurrentState(EZoneState::Inactive) ? EZoneState::Active : EZoneState::Inactive;

	switch (TargetState)
	{
	case EZoneState::Active:
		EnergyTrigger->EnableTrigger();
		ZoneEffect->SetHiddenInGame(false);
		AnimationTimeline->PlayFromStart();
		break;
	case EZoneState::Inactive:
		AnimationTimeline->ReverseFromEnd();
		break;
	}
}

void AEnergyZone::SetZoneScale(const float Progress)
{
	NewScale = InitialScale + TargetScale * Progress;
	SetActorScale3D(NewScale);
}

void AEnergyZone::FinishTransition()
{
	ChangeState(TargetState);
}
