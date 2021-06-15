// Fill out your copyright notice in the Description page of Project Settings.


#include "Battery.h"
#include "EnergyTrigger.h"


ABattery::ABattery()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
}

void ABattery::BeginPlay()
{
	Super::BeginPlay();
}

void ABattery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}