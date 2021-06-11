// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryManager.h"
#include "Battery.h"
#include "GameFramework/Character.h"
#include "HairStrandsInterface.h"


UBatteryManager::UBatteryManager()
{
	PrimaryComponentTick.bCanEverTick = false;;
}


void UBatteryManager::BeginPlay()
{
	Super::BeginPlay();
}

void UBatteryManager::SetDecreaseRate(const float NewRate)
{
	if (NewRate <= 0.f) return;

	DecreaseRate = NewRate;
	DecreaseInterval = 1.f / DecreaseRate;
}

void UBatteryManager::SetIncreaseRate(const float NewRate)
{
	if (NewRate <= 0.f) return;

	IncreaseRate = NewRate;
	IncreaseInterval = 1.f / IncreaseRate;
}

bool UBatteryManager::IncreaseEnergy(const float DeltaEnergy)
{
	if (DeltaEnergy <= 0.f) return false;

	Energy = FMath::Min(Energy + DeltaEnergy, MaxEnergy);
	OnEnergyChanged.Broadcast(Energy, DeltaEnergy);
	return true;
}

bool UBatteryManager::DecreaseEnergy(const float DeltaEnergy)
{
	if (DeltaEnergy <= 0.f) return false;

	Energy = FMath::Max(Energy - DeltaEnergy, 0.f);
	OnEnergyChanged.Broadcast(Energy, -DeltaEnergy);
	return true;
}

void UBatteryManager::SpawnBattery()
{
	if (!GetWorld() || !GetOwner()) return;

	BatteryActor = GetWorld()->SpawnActor<ABattery>(BatteryClass);

	if (!BatteryActor) return;
	
	BatteryActor->SetOwner(GetOwner());
	AttachBatteryToSocket(Cast<ACharacter>(GetOwner())->GetMesh(), BatterySocketName);
}

void UBatteryManager::EquipBattery()
{
	AttachBatteryToSocket(Cast<ACharacter>(GetOwner())->GetMesh(), BatterySocketName);
}

void UBatteryManager::UnequipBattery(USkeletalMeshComponent* SkeletalMesh)
{
	AttachBatteryToSocket(SkeletalMesh, BatterySocketName);
}

void UBatteryManager::AttachBatteryToSocket(USkeletalMeshComponent* SkeletalMesh, const FName SocketName)
{
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	BatteryActor->AttachToComponent(SkeletalMesh, AttachmentRules, SocketName);
}
