// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interact.generated.h"

class APlayerCharacter;

UINTERFACE(BlueprintType)
class UInteract : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GMTK_2021_API IInteract
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ProcessInteraction(APlayerCharacter* PlayerCharacter);
};
