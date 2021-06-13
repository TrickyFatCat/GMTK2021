// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnergyTrigger.h"
#include "Battery.generated.h"

UCLASS()
class GMTK_2021_API ABattery : public AActor
{
	GENERATED_BODY()

public:
	ABattery();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	void EnableEnergyTrigger(); 
	void DisableEnergyTrigger();

	UFUNCTION(BlueprintImplementableEvent)
	void OnTriggerChangedState(const bool bIsEnabled);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	UEnergyTrigger* EnergyTrigger = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* Mesh = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* ZoneMesh = nullptr;
};
