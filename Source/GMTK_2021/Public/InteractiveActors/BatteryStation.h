// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interact.h"
#include "GameFramework/Actor.h"
#include "BatteryStation.generated.h"

class USkeletalMeshComponent;
class UInteractionTrigger;

UENUM(BlueprintType)
enum class EStationState : uint8
{
	Active,
	Inactive,
	Disabled
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChangedSignature, EStationState, NewState);

UCLASS()
class GMTK_2021_API ABatteryStation : public AActor, public IInteract
{
	GENERATED_BODY()

public:
	ABatteryStation();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	bool ProcessInteraction_Implementation(APlayerCharacter* PlayerCharacter);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	UInteractionTrigger* TriggerSphere = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	USkeletalMeshComponent* SkeletalMesh = nullptr;

	// States
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battery")
	EStationState InitialState = EStationState::Inactive;
	UFUNCTION(BlueprintPure, Category="Battery")
	EStationState GetCurrentState() const { return CurrentState; }
	UPROPERTY(BlueprintAssignable, Category="Battery")
	FOnStateChangedSignature OnStateChanged;

	UFUNCTION(BlueprintCallable, Category="Battery")
	bool DisableStation();
	UFUNCTION(BlueprintCallable, Category="Battery")
	bool EnableStation();

private:
	EStationState CurrentState = EStationState::Inactive;
};
