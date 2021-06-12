﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActors/ButtonBase.h"
#include "Components/TimelineComponent.h"


AButtonBase::AButtonBase()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractionTrigger = CreateDefaultSubobject<UInteractionTrigger>("InteractionTrigger");
	SetRootComponent(InteractionTrigger);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	BaseMesh->SetupAttachment(GetRootComponent());

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("ButtonBody");
	BodyMesh->SetupAttachment(BaseMesh);

	AnimationTimeline = CreateDefaultSubobject<UTimelineComponent>("AnimationTimeline");
}

void AButtonBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (AnimationCurve)
	{
		FOnTimelineFloat AnimationProgress;
		AnimationProgress.BindUFunction(this, FName("SetButtonBodyTransform"));
		AnimationTimeline->AddInterpFloat(AnimationCurve, AnimationProgress);
		FOnTimelineEvent AnimationFinish;
		AnimationFinish.BindUFunction(this, FName("FinishTransition"));
		AnimationTimeline->SetTimelineFinishedFunc(AnimationFinish);
		CalculatePlayRate();
	}

	InitialTransform = BodyMesh->GetComponentTransform();
}

void AButtonBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AButtonBase::DisableButton()
{
	if (IsCurrentState(EButtonState::Disabled) || IsCurrentState(EButtonState::Transition)) return false;

	ChangeState(EButtonState::Disabled);
	InteractionTrigger->DisableTrigger();
	return true;
}

bool AButtonBase::ProcessInteraction_Implementation(APlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter || IsCurrentState(EButtonState::Transition) || IsCurrentState(EButtonState::Disabled)) return false;

	StartTransition();
	
	return true;
}

void AButtonBase::ChangeState(const EButtonState NewState)
{
	if (CurrentState == NewState) return;

	PreviousState = CurrentState;
	CurrentState = NewState;
	OnChangeState.Broadcast(CurrentState, PreviousState);
}

void AButtonBase::SetTargetState()
{
	TargetState = CurrentState == EButtonState::PositionA ? EButtonState::PositionB : EButtonState::PositionA;
}

void AButtonBase::CalculatePlayRate() const
{
	if (!AnimationCurve) return;

	AnimationTimeline->SetPlayRate(1.f / AnimationDuration);
}


void AButtonBase::StartTransition()
{
	SetTargetState();

	switch (TargetState)
	{
		case EButtonState::PositionB:
			AnimationTimeline->PlayFromStart();
		break;
		case EButtonState::PositionA:
			AnimationTimeline->ReverseFromEnd();
		break;
	}

	ChangeState(EButtonState::Transition);
}

void AButtonBase::FinishTransition()
{
	ChangeState(TargetState);

	if (bPressOnce)
	{
		DisableButton();
	}
}

void AButtonBase::SetButtonBodyTransform(const float Progress)
{
	FTransform NewTransform = InitialTransform;
	NewTransform.SetLocation(NewTransform.GetLocation() + TargetTransform.GetLocation() * Progress);
	const FQuat NewRotation = FRotator(NewTransform.GetRotation().Rotator() + TargetTransform.GetRotation().Rotator() * Progress).Quaternion();
	NewTransform.SetRotation(NewRotation);
	BodyMesh->SetWorldTransform(NewTransform);
}
