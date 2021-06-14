// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionManager.h"
#include "Interact.h"
#include "PlayerCharacter.h"
#include "InteractiveActors/BatteryStation.h"


UInteractionManager::UInteractionManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UInteractionManager::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionManager::AddToQueue(AActor* InteractiveActor)
{
	if (!IsValid(InteractiveActor) || InteractionQueue.Find(InteractiveActor) >= 0) return;

	InteractionQueue.AddUnique(InteractiveActor);
}

void UInteractionManager::RemoveFromQueue(AActor* InteractiveActor)
{
	if (!IsValid(InteractiveActor) || InteractionQueue.Find(InteractiveActor) < 0) return;

	InteractionQueue.Remove(InteractiveActor);
}

bool UInteractionManager::Interact()
{
	if (InteractionQueue.Num() == 0) return false;

	AActor* TargetActor = GetTargetActor();

	if (!TargetActor) return false;

	return IInteract::Execute_ProcessInteraction(TargetActor, Cast<APlayerCharacter>(GetOwner()));
}

AActor* UInteractionManager::GetTargetActor() const
{
	if (InteractionQueue.Num() == 0) return nullptr;
	
	AActor* TargetActor = InteractionQueue[0];

	while (!IsValid(TargetActor))
	{
		TargetActor = InteractionQueue[0];
	}

	return TargetActor;
}

bool UInteractionManager::CanInteract() const
{
	AActor* TargetActor = GetTargetActor();

	if (!TargetActor) return false;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());

	if (!PlayerCharacter) return false;

	if (TargetActor->IsA(ABatteryStation::StaticClass())
								&& Cast<ABatteryStation>(TargetActor)->IsInactive()
								&& !PlayerCharacter->IsBatteryEquipped()) return false;

	return true;
}
