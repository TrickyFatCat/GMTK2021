// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Interact.h"
#include "GameFramework/Actor.h"
#include "BatteryStation.generated.h"

class USkeletalMeshComponent;
class USphereComponent;

UCLASS()
class GMTK_2021_API ABatteryStation : public AActor, public IInteract
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABatteryStation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	bool ProcessInteraction_Implementation(APlayerCharacter* PlayerCharacter);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	USphereComponent* TriggerSphere = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	USkeletalMeshComponent* SkeletalMesh = nullptr;
};
