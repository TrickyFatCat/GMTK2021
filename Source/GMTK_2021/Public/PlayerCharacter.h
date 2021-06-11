// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BatteryManager.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

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

	// Camera
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Component", meta=(AllowPrivateAccess="true"))
	USpringArmComponent* SpringArm = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	UCameraComponent* PlayerCamera = nullptr;

	// Movement
private:
	void MoveForward(const float AxisValue);
	void MoveRight(const float AxisValue);

	// Battery
	public:
	UFUNCTION(BlueprintPure, Category="Energy")
	float GetEnergy() const { return BatteryManager->GetEnergy(); }
	UFUNCTION(BlueprintCallable, Category="Battery")
	ABattery* GetBattery() const { return BatteryManager->GetBatteryActor(); }
	
	private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	UBatteryManager* BatteryManager = nullptr;
};
