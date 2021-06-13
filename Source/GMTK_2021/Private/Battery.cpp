// Fill out your copyright notice in the Description page of Project Settings.


#include "Battery.h"
#include "EnergyTrigger.h"


ABattery::ABattery()
{
	PrimaryActorTick.bCanEverTick = true;

	EnergyTrigger = CreateDefaultSubobject<UEnergyTrigger>("EnergyTrigger");
	SetRootComponent(EnergyTrigger);
	EnergyTrigger->DisableTrigger();
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());

	ZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>("ZoneMesh");
	ZoneMesh->SetupAttachment(GetRootComponent());
}

void ABattery::BeginPlay()
{
	Super::BeginPlay();
}

void ABattery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABattery::EnableEnergyTrigger()
{
	EnergyTrigger->EnableTrigger();
	OnTriggerChangedState(true);
	ZoneMesh->SetHiddenInGame(false);
}

void ABattery::DisableEnergyTrigger()
{
	EnergyTrigger->DisableTrigger();
	OnTriggerChangedState(false);
}
