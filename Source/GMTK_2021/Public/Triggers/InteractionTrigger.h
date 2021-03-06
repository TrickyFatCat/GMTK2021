// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "InteractionTrigger.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GMTK_2021_API UInteractionTrigger : public USphereComponent
{
	GENERATED_BODY()

public:
	UInteractionTrigger();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category="Interaction Trigger")
	void EnableTrigger();
	UFUNCTION(BlueprintCallable, Category="Interaction Trigger")
	void DisableTrigger();

private:
	UPROPERTY(VisibleAnywhere)
	bool bIsEnabled = true;
	UFUNCTION()
	void AddToInteractionQueue(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	UFUNCTION()
	void RemoveFromInteractionQueue(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
};
