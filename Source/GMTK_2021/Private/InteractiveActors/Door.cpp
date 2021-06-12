// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActors/Door.h"


ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;

	AnimationTimeline = CreateDefaultSubobject<UTimelineComponent>("AnimationTimeline");
	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>("DoorFrame");
	SetRootComponent(DoorFrame);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	if (AnimationCurve)
	{
		FOnTimelineFloat AnimationProgress;
		AnimationProgress.BindUFunction(this, FName("SetDoorTransform"));
		AnimationTimeline->AddInterpFloat(AnimationCurve, AnimationProgress);
		FOnTimelineEvent AnimationFinish;
		AnimationFinish.BindUFunction(this, FName("FinishTransition"));
		AnimationTimeline->SetTimelineFinishedFunc(AnimationFinish);
		CalculatePlayRate();
	}

	DoorLeaves.Empty();
	TArray<USceneComponent*> FrameChildren;
	DoorFrame->GetChildrenComponents(false, FrameChildren);
	for (auto FrameChild : FrameChildren)
	{
		UStaticMeshComponent* DoorLeaf = Cast<UStaticMeshComponent>(FrameChild);

		if (DoorLeaf)
		{
			DoorLeaves.Add(DoorLeaf);
			InitialTransforms.Add(DoorLeaf->GetComponentTransform());
		}
	}

	ChangeState(InitialState);
	PreviousState = InitialState;
	float StartProgress = 0.f;

	switch (CurrentState)
	{
		case EDoorState::Closed:
		case EDoorState::Locked:
			StartProgress = 0.f;
		break;
		case EDoorState::Opened:
			StartProgress = 1.f;
		break;
		case EDoorState::Transition:
			FinishTransition();
		break;
	}
	SetDoorTransform(StartProgress);
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ADoor::OpenDoor()
{
	if (IsCurrentState(EDoorState::Opened) || IsCurrentState(EDoorState::Locked) || IsCurrentState(EDoorState::Transition)) return false;

	if (AnimationDuration <= 0.f)
	{
		SetTargetState();
		FinishTransition();
		return true;
	}

	StartTransition();
	return true;
}

bool ADoor::CloseDoor()
{
	if (IsCurrentState(EDoorState::Closed) ||  IsCurrentState(EDoorState::Locked) || IsCurrentState(EDoorState::Transition)) return false;

	if (AnimationDuration <= 0.f)
	{
		SetTargetState();
		FinishTransition();
		return true;
	}

	StartTransition();
	return true;
}

bool ADoor::LockDoor()
{
	if (IsCurrentState(EDoorState::Locked) || !IsCurrentState(EDoorState::Closed)) return false;

	ChangeState(EDoorState::Locked);
	return true;
}

bool ADoor::UnlockDoor()
{
	if (!IsCurrentState(EDoorState::Locked)) return false;

	ChangeState(EDoorState::Closed);
	return true;
}

void ADoor::ChangeState(const EDoorState NewState)
{
	if (CurrentState == NewState) return;

	PreviousState = CurrentState;
	CurrentState = NewState;
	OnChangeState.Broadcast(NewState, PreviousState);
}

void ADoor::SetTargetState()
{
	TargetState = CurrentState == EDoorState::Closed ? EDoorState::Opened : EDoorState::Closed;
}

void ADoor::CalculatePlayRate() const
{
	if (!AnimationCurve) return;

	AnimationTimeline->SetPlayRate(1.f / AnimationDuration);
}

void ADoor::StartTransition()
{
	SetTargetState();

	switch (TargetState)
	{
	case EDoorState::Opened:
		AnimationTimeline->PlayFromStart();
		break;
	case EDoorState::Closed:
		AnimationTimeline->ReverseFromEnd();
		break;
	}

	ChangeState(EDoorState::Transition);
}

void ADoor::FinishTransition()
{
	ChangeState(TargetState);
}

void ADoor::SetDoorTransform(const float Progress)
{
	if (InitialTransforms.Num() == 0 || TargetTransforms.Num() == 0 || DoorLeaves.Num() == 0) return;
	
	if (TargetTransforms.Num() != InitialTransforms.Num()) return;

	for (int32 i = 0; i < DoorLeaves.Num(); ++i)
	{
		FTransform NewTransform = InitialTransforms[i];
		FTransform TargetTransform = TargetTransforms[i];
		NewTransform.SetLocation(NewTransform.GetLocation() + TargetTransform.GetLocation() * Progress);
		FQuat NewRotation = FRotator(NewTransform.GetRotation().Rotator() + TargetTransform.GetRotation().Rotator() * Progress).Quaternion();
		NewTransform.SetRotation(NewRotation);
		DoorLeaves[i]->SetWorldTransform(NewTransform);
	}
}
