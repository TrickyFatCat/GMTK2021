// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActors/ButtonBase.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"

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

	InitialTransform = BodyMesh->GetRelativeTransform();

	CurrentState = InitialState;
	float StartProgress = 0.f;

	switch (CurrentState)
	{
		case EButtonState::PositionA:
			StartProgress = 0.f;
		break;
		case EButtonState::Locked:
			StartProgress = 0.f;
			InteractionTrigger->DisableTrigger();
		break;
		case EButtonState::PositionB:
			StartProgress = 1.f;
		break;
		case EButtonState::Transition:
			CurrentState = EButtonState::PositionA;
			StartProgress = 0.f;
		break;
		case EButtonState::Disabled:
			if (!bPressOnce)
			{
				CurrentState = EButtonState::Locked;
				InteractionTrigger->DisableTrigger();
			}
		break;
	}

	OnStateChanged(CurrentState);
	SetButtonBodyTransform(StartProgress);
}

void AButtonBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AButtonBase::LockButton()
{
	if (IsCurrentState(EButtonState::Locked) || IsCurrentState(EButtonState::Transition) || (IsCurrentState(EButtonState::Disabled) && bPressOnce)) return false;
	
	ChangeState(EButtonState::Locked);
	return true;
}

bool AButtonBase::UnlockButton()
{
	if (!IsCurrentState(EButtonState::Locked)) return false;

	ChangeState(PreviousState);
	return true;
}

bool AButtonBase::DisableButton()
{
	if (IsCurrentState(EButtonState::Disabled) || IsCurrentState(EButtonState::Transition)) return false;

	ChangeState(EButtonState::Disabled);
	return true;
}

bool AButtonBase::ProcessInteraction_Implementation(APlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter ||  IsCurrentState(EButtonState::Disabled) || IsCurrentState(EButtonState::Locked) || IsCurrentState(EButtonState::Transition)) return false;

	StartTransition();
	
	return true;
}

void AButtonBase::ChangeState(const EButtonState NewState)
{
	if (CurrentState == NewState) return;

	PreviousState = CurrentState;
	CurrentState = NewState;

	switch (NewState)
	{
		case EButtonState::PositionA:
		case EButtonState::PositionB:
			InteractionTrigger->EnableTrigger();
		break;
		default:
			InteractionTrigger->DisableTrigger();
		break;
	}
	OnChangeState.Broadcast(CurrentState, PreviousState);
	OnStateChanged(CurrentState);
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

	if (ButtonSoundStart)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ButtonSoundStart, GetActorLocation());
	}
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

	if (ButtonSoundStop)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ButtonSoundStop, GetActorLocation());
	}

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
	BodyMesh->SetRelativeTransform(NewTransform);
}