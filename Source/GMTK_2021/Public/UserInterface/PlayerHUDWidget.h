// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class GMTK_2021_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;
	UFUNCTION(BlueprintImplementableEvent, Category="UserInterface")
	void OnEnergyIncrease();
	UFUNCTION(BlueprintImplementableEvent, Category="UserInterface")
	void OnEnergyDecrease();
	
protected:
	UFUNCTION(BlueprintPure, Category="UserInterface")
	float GetNormalizedEnergy() const;
	UFUNCTION(BlueprintPure, Category="UserInterface")
	float GetIsPlayerAlive() const;
	void OnEnergyChanged(const float Energy, const float DeltaEnergy);
};
