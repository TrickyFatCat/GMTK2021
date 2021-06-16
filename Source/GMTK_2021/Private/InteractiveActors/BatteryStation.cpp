// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActors/BatteryStation.h"
#include "Actors/PlayerCharacter.h"
#include "Triggers/InteractionTrigger.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"


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

	if (InitialState == EStationState::Active)
	{
		InitialState = EStationState::Inactive;
	}

	CurrentState = InitialState;

	if (CurrentState == EStationState::Disabled)
	{
		TriggerSphere->DisableTrigger();
	}

	OnChangedState(CurrentState);
	SpawnEnergyZone();
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

		if (ActivationSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ActivationSound, GetActorLocation());
		}

		EnergyZone->ActivateZone();
		break;
	case EStationState::Active:
		PlayerCharacter->EquipBattery();

		if (DeactivationSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DeactivationSound, GetActorLocation());
		}

		EnergyZone->DeactivateZone();
		break;
	}

	return true;
}

bool ABatteryStation::DisableStation()
{
	if (CurrentState == EStationState::Disabled || CurrentState == EStationState::Active) return false;

	CurrentState = EStationState::Disabled;
	TriggerSphere->DisableTrigger();
	OnChangedState(CurrentState);
	OnStateChanged.Broadcast(CurrentState);
	return true;
}

bool ABatteryStation::EnableStation()
{
	if (CurrentState != EStationState::Disabled) return false;

	CurrentState = EStationState::Inactive;
	TriggerSphere->EnableTrigger();
	OnChangedState(CurrentState);
	OnStateChanged.Broadcast(CurrentState);
	return true;
}

void ABatteryStation::ChangeState(const EZoneState NewZoneState)
{
	if (CurrentState == EStationState::Disabled) return;
	UE_LOG(LogTemp, Error, TEXT("Here am i"));
	switch (NewZoneState)
	{
		case EZoneState::Active:
			TriggerSphere->EnableTrigger();
			CurrentState = EStationState::Active;
		break;
		case EZoneState::Inactive:
			TriggerSphere->EnableTrigger();
			CurrentState = EStationState::Inactive;
		break;
		case EZoneState::Transition:
			TriggerSphere->DisableTrigger();
			CurrentState = EStationState::Transition;
		break;
	}

	OnChangedState(CurrentState);
	OnStateChanged.Broadcast(CurrentState);
}

void ABatteryStation::SpawnEnergyZone()
{
	if (!GetWorld()) return;

	EnergyZone = GetWorld()->SpawnActor<AEnergyZone>(EnergyZoneClass);

	if (!EnergyZone) return;

	EnergyZone->SetOwner(this);
	EnergyZone->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	EnergyZone->OnChangeState.AddDynamic(this, &ABatteryStation::ChangeState);
}
