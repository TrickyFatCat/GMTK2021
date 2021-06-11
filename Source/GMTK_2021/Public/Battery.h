// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
};
