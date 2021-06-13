// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"

#include "Door.generated.h"

UENUM(BlueprintType)
enum class EDoorState : uint8
{
	Opened,
	Closed,
	Locked,
	Transition
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDoorChangedStateSignature, EDoorState, NewState, EDoorState, PreviousState);

UCLASS()
class GMTK_2021_API ADoor : public AActor
{
	GENERATED_BODY()

public:
	ADoor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Door controls
public:
	UFUNCTION(BlueprintCallable, Category="Door")
	bool OpenDoor();
	UFUNCTION(BlueprintCallable, Category="Door")
	bool CloseDoor();
	UFUNCTION(BlueprintCallable, Category="Door")
	bool LockDoor();
	UFUNCTION(BlueprintCallable, Category="Door")
	bool UnlockDoor();

	// Door objects
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* DoorRoot = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* DoorFrame = nullptr;
	UPROPERTY(VisibleAnywhere, Category="Door")
	TArray<UStaticMeshComponent*> DoorLeaves;
	UPROPERTY(VisibleAnywhere, Category="Door")
	TArray<FTransform> InitialTransforms;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Door")
	TArray<FTransform> TargetTransforms;
	// Door states
public:
	UFUNCTION(BlueprintPure, Category="Door")
	EDoorState GetCurrentState() const { return CurrentState; }

	UFUNCTION(BlueprintPure, Category="Door")
	bool IsCurrentState(const EDoorState DoorState) const { return CurrentState == DoorState; }
	UFUNCTION(BlueprintPure, Category="Door")
	bool IsPreviousState(const EDoorState DoorState) const { return PreviousState == DoorState; }

	UPROPERTY(BlueprintAssignable, Category="Door")
	FOnDoorChangedStateSignature OnChangeState;
	UFUNCTION(BlueprintImplementableEvent, Category="Door")
	void OnStateChanged(EDoorState NewState);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door")
	EDoorState InitialState = EDoorState::Locked;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door")
	EDoorState CurrentState = InitialState;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Door")
	EDoorState PreviousState = InitialState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door")
	EDoorState TargetState = EDoorState::Closed;
	void ChangeState(const EDoorState NewState);
	void SetTargetState();

	// Door Animation
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	UTimelineComponent* AnimationTimeline = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation")
	UCurveFloat* AnimationCurve = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	float AnimationDuration = 1.f;
	void CalculatePlayRate() const;
	UFUNCTION()
	void StartTransition();
	UFUNCTION()
	void FinishTransition();
	UFUNCTION()
	void SetDoorTransform(const float Progress);

	// Sound
	UPROPERTY(EditDefaultsOnly, Category="Sound")
	USoundBase* DoorSound = nullptr;
};
