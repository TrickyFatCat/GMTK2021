// Fill out your copyright notice in the Description page of Project Settings.


#include "EnergyTrigger.h"
#include "PlayerCharacter.h"

void UEnergyTrigger::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UEnergyTrigger::OnEnterTrigger);
	OnComponentEndOverlap.AddDynamic(this, &UEnergyTrigger::OnExitTrigger);
}

void UEnergyTrigger::EnableTrigger()
{
	if (bIsEnabled) return;
	
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	bIsEnabled = true;
	SetHiddenInGame(false);
}

void UEnergyTrigger::DisableTrigger()
{
	if (!bIsEnabled) return;
	
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	bIsEnabled = false;
	SetHiddenInGame(true);
}

void UEnergyTrigger::OnEnterTrigger(UPrimitiveComponent* OverlappedComponent,
                                    AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp,
                                    int32 OtherBodyIndex,
                                    bool bFromSweep,
                                    const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || !bIsIncreasingEnergy) return;

	const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

	if (!PlayerCharacter) return;
	
	PlayerCharacter->StartEnergyIncrease();
}

void UEnergyTrigger::OnExitTrigger(UPrimitiveComponent* OverlappedComponent,
                                   AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp,
                                   int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor)) return;

	const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

	if (!PlayerCharacter) return;

	PlayerCharacter->StartEnergyDecrease();
}