// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionTrigger.h"
#include "PlayerCharacter.h"

// Sets default values for this component's properties
UInteractionTrigger::UInteractionTrigger()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UInteractionTrigger::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UInteractionTrigger::AddToInteractionQueue);
	OnComponentEndOverlap.AddDynamic(this, &UInteractionTrigger::RemoveFromInteractionQueue);
}

void UInteractionTrigger::AddToInteractionQueue(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || !IsValid(GetOwner())) return;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

	if (!PlayerCharacter) return;

	PlayerCharacter->AddToInteractionQueue(GetOwner());
}

void UInteractionTrigger::RemoveFromInteractionQueue(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor) || !IsValid(GetOwner())) return;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

	if (!PlayerCharacter) return;

	Cast<APlayerCharacter>(OtherActor)->RemoveFromInteractionQueue(GetOwner());
}
