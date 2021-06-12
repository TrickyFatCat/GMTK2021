// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryStation.h"
#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"


ABatteryStation::ABatteryStation()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerSphere = CreateDefaultSubobject<USphereComponent>("TriggerSphere");
	SetRootComponent(TriggerSphere);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMesh->SetupAttachment(GetRootComponent());
}

void ABatteryStation::BeginPlay()
{
	Super::BeginPlay();
}

void ABatteryStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABatteryStation::ProcessInteraction_Implementation(APlayerCharacter* PlayerCharacter)
{
	return true;
}

