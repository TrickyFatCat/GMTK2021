// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Door.h"
#include "Interact.h"
#include "InteractionTrigger.h"
#include "GameFramework/Actor.h"
#include "ButtonBase.generated.h"

class UTimelineComponent;

UENUM(BlueprintType)
enum class EButtonState : uint8
{
	PositionA,
	PositionB,
	Locked,
	Disabled,
	Transition
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonChangedStateSignature, EButtonState, NewState, EButtonState, PreviousState);

UCLASS()
class GMTK_2021_API AButtonBase : public AActor, public IInteract
{
	GENERATED_BODY()

public:
	AButtonBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

// Button controls
public:
	bool DisableButton();
protected:
	virtual bool ProcessInteraction_Implementation(APlayerCharacter* PlayerCharacter) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UInteractionTrigger* InteractionTrigger = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* BaseMesh = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* BodyMesh = nullptr;

private:
	UPROPERTY(VisibleAnywhere, Category="Button")
	FTransform InitialTransform;
	UPROPERTY(EditDefaultsOnly, Category="Button")
	FTransform TargetTransform;

	//States
public:
	UFUNCTION(BlueprintPure, Category="Button")
	bool IsCurrentState(const EButtonState ButtonState) const { return CurrentState == ButtonState; };
	UFUNCTION(BlueprintCallable, Category="Button")
	bool IsPreviousState(const EButtonState ButtonState) const { return PreviousState == ButtonState; };

	UPROPERTY(BlueprintAssignable, Category="Button")
	FOnButtonChangedStateSignature OnChangeState;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button")
	bool bPressOnce = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button")
	EButtonState InitialState = EButtonState::PositionA;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Button")
	EButtonState CurrentState = InitialState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Button")
	EButtonState PreviousState = InitialState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Button")
	EButtonState TargetState = EButtonState::PositionB;
	void ChangeState(const EButtonState NewState);
	void SetTargetState();
	
	// Animation
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	UTimelineComponent* AnimationTimeline = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation")
	UCurveFloat* AnimationCurve = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	float AnimationDuration = 1.f;
	void CalculatePlayRate() const;
	UFUNCTION()
	virtual void StartTransition();
	UFUNCTION()
	virtual void FinishTransition();
	UFUNCTION()
	void SetButtonBodyTransform(const float Progress);
};
