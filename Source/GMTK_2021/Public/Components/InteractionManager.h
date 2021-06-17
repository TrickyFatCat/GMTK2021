// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionManager.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GMTK_2021_API UInteractionManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionManager();

protected:
	virtual void BeginPlay() override;

public:
	void AddToQueue(AActor* InteractiveActor);
	void RemoveFromQueue(AActor* InteractiveActor);
	bool Interact();
	bool IsQueueEmpty() const { return InteractionQueue.Num() <= 0; }
	UFUNCTION(BlueprintPure, Category="Interaction")
	AActor* GetTargetActor() const;
	UFUNCTION(BlueprintPure, Category="Interaction")
	bool CanInteract() const;

private:
	UPROPERTY(VisibleAnywhere, Category="Interaction")
	TArray<AActor*> InteractionQueue{};
};
