// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionManager.h"
#include "Interact.h"
#include "PlayerCharacter.h"


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

	AActor* TargetActor = InteractionQueue[0];

	while (!IsValid(TargetActor))
	{
		InteractionQueue.Remove(TargetActor);
		TargetActor = InteractionQueue[0];

		if (InteractionQueue.Num() == 0) return false;
	}

	return IInteract::Execute_ProcessInteraction(TargetActor, Cast<APlayerCharacter>(GetOwner()));
}

AActor* UInteractionManager::GetTargetActor() const
{
	AActor* TargetActor = InteractionQueue[0];

	while (!IsValid(TargetActor))
	{
		TargetActor = InteractionQueue[0];
	}

	return TargetActor;
}
