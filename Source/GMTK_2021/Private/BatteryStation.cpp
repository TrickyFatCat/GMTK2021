// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryStation.h"
#include "PlayerCharacter.h"
#include "InteractionTrigger.h"
#include "Components/SkeletalMeshComponent.h"


ABatteryStation::ABatteryStation()
{
	PrimaryActorTick.bCanEverTick = true;
	TriggerSphere = CreateDefaultSubobject<UInteractionTrigger>("TriggerSphere");
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
	if (!PlayerCharacter) return false;

	switch (CurrentState)
	{
	case EStationState::Inactive:
		PlayerCharacter->UnequipBattery(SkeletalMesh);
		CurrentState = EStationState::Active;
		OnStateChanged.Broadcast(CurrentState);
		break;
	case EStationState::Active:
		PlayerCharacter->EquipBattery();
		CurrentState = EStationState::Inactive;
		OnStateChanged.Broadcast(CurrentState);
		break;
	}

	return true;
}

bool ABatteryStation::DisableStation()
{
	if (CurrentState == EStationState::Disabled) return false;

	CurrentState = EStationState::Disabled;
	TriggerSphere->DisableTrigger();
	OnStateChanged.Broadcast(CurrentState);
	return true;
}

bool ABatteryStation::EnableStation()
{
	if (CurrentState != EStationState::Disabled) return false;

	CurrentState = EStationState::Inactive;
	TriggerSphere->EnableTrigger();
	OnStateChanged.Broadcast(CurrentState);
	return true;
}
