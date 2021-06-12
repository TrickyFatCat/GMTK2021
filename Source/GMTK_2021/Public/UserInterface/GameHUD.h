// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class GMTK_2021_API AGameHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UserInterface")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;
};
