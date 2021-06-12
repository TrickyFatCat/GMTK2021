// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "EnergyTrigger.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeStateSignature, bool, IsEnabled);
/**
 * 
 */
UCLASS()
class GMTK_2021_API UEnergyTrigger : public USphereComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
public:
	UFUNCTION(BlueprintPure, Category="Energy")
	bool GetIsIncreasingEnergy() const { return bIsIncreasingEnergy; }
	UFUNCTION(BlueprintCallable, Category="Energy")
	void EnableTrigger();
	UFUNCTION(BlueprintCallable, Category="Energy")
	void DisableTrigger();
	UFUNCTION(BlueprintPure)
	bool GetIsEnabled() const { return bIsEnabled; }
	UPROPERTY(BlueprintAssignable, Category="Energy")
	FOnChangeStateSignature OnChangeState;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Energy", meta=(AllowPrivateAccess="true"))
	bool bIsIncreasingEnergy = true;
	bool bIsEnabled = true;
	UFUNCTION()
	void OnEnterTrigger(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	UFUNCTION()
	void OnExitTrigger(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
};
