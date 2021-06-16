// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BatteryManager.h"
#include "Components/InteractionManager.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerWinSignature);

UCLASS()
class GMTK_2021_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerDeathSignature OnPlayerDeath;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerWinSignature OnPlayerWin;
	UFUNCTION(BlueprintCallable)
	void OnWin();

	// Camera
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Component", meta=(AllowPrivateAccess="true"))
	USpringArmComponent* SpringArm = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	UCameraComponent* PlayerCamera = nullptr;

	// Movement
private:
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float Acceleration = 8.f;
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float MaxSpeed = 400.f;
	void MoveForward(const float AxisValue);
	void MoveRight(const float AxisValue);
	void SetInputEnabled(const bool bIsEnabled);

	// Battery
public:
	UFUNCTION(BlueprintPure, Category="Energy")
	float GetEnergy() const { return BatteryManager->GetEnergy(); }

	UFUNCTION(BlueprintPure, Category="Player")
	bool IsDead() const { return GetEnergy() <= 0.f; }

	UFUNCTION(BlueprintPure, Category="Energy")
	float GetNormalizedEnergy() const { return BatteryManager->GetNormalizedEnergy(); }

	UFUNCTION(BlueprintCallable, Category="Energy")
	bool StartEnergyIncrease() const { return BatteryManager->StartEnergyIncrease(); }

	UFUNCTION(BlueprintCallable, Category="Energy")
	bool StopEnergyIncrease() const { return BatteryManager->StopEnergyIncrease(); }

	UFUNCTION(BlueprintCallable, Category="Energy")
	bool StartEnergyDecrease() const { return BatteryManager->StartEnergyDecrease(); }

	UFUNCTION(BlueprintCallable, Category="Energy")
	bool StopEnergyDecrease() const { return BatteryManager->StopEnergyDecrease(); }

	UFUNCTION(BlueprintCallable, Category="Battery")
	ABattery* GetBattery() const { return BatteryManager->GetBatteryActor(); }

	UFUNCTION(BlueprintPure, Category="Battery")
	bool IsBatteryEquipped() const { return BatteryManager->GetIsBatteryEquipped(); }

	UFUNCTION(BlueprintCallable, Category="Battery")
	void UnequipBattery(USkeletalMeshComponent* TargetMesh) { BatteryManager->UnequipBattery(TargetMesh); }

	UFUNCTION(BlueprintCallable, Category="Battery")
	void EquipBattery() const { BatteryManager->EquipBattery(); }

	UFUNCTION(BlueprintImplementableEvent, Category="Battery")
	void OnEnergyStartIncreasing();

	UFUNCTION(BlueprintImplementableEvent, Category="Battery")
	void OnEnergyStopIncreasing();
	
	UFUNCTION(BlueprintImplementableEvent, Category="Battery")
	void OnEnergyChanged(const float Energy, const float DeltaEnergy);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	UBatteryManager* BatteryManager = nullptr;
	void OnDeath();

	// Interaction
public:
	void AddToInteractionQueue(AActor* Actor) { InteractionManager->AddToQueue(Actor); }
	void RemoveFromInteractionQueue(AActor* Actor) { InteractionManager->RemoveFromQueue(Actor); }
	UFUNCTION(BlueprintPure, Category="Interaction")
	bool CanInteract() const { return InputEnabled() && InteractionManager->CanInteract(); }
	UFUNCTION(BlueprintPure, Category="Interaction")
	AActor* GetTargetActor() const { return InteractionManager->GetTargetActor(); }
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	UInteractionManager* InteractionManager = nullptr;
	void StartInteraction();
	void FinishInteraction(); 

	// Animations
protected:
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimMontage* DeathMontage = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimMontage* StandInteractionMontage = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimMontage* GroundInteractionMontage = nullptr;
	void InitAnimations();
};
