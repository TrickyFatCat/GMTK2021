﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Battery.h"


ABattery::ABattery()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABattery::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABattery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

