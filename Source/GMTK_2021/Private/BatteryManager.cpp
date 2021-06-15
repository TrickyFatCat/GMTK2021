// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryManager.h"
#include "Battery.h"
#include "GameFramework/Character.h"
#include "HairStrandsInterface.h"


UBatteryManager::UBatteryManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UBatteryManager::BeginPlay()
{
	Super::BeginPlay();
	SpawnBattery();
	Energy = MaxEnergy;
	CalculateIntervals();
	OnEnergyChanged.Broadcast(Energy, 0.f);
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

void UBatteryManager::IncreaseEnergy(const float DeltaEnergy)
{
	if (DeltaEnergy <= 0.f || Energy >= MaxEnergy) return;

	Energy = FMath::Min(Energy + DeltaEnergy, MaxEnergy);
	OnEnergyChanged.Broadcast(Energy, DeltaEnergy);

	if (Energy >= MaxEnergy)
	{
		StopEnergyIncrease();
	}
}

void UBatteryManager::DecreaseEnergy(const float DeltaEnergy)
{
	if (DeltaEnergy <= 0.f || Energy <= 0.f) return;

	Energy = FMath::Max(Energy - DeltaEnergy, 0.f);
	OnEnergyChanged.Broadcast(Energy, -DeltaEnergy);

	if (Energy <= 0.f)
	{
		StopEnergyDecrease();
		OnDeath.Broadcast();
	}
}

bool UBatteryManager::StartEnergyDecrease()
{
	if (!GetWorld() || Energy <= 0.f || bIsBatteryEquipped) return false;

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(EnergyDecreaseHandle)) return false;

	if (TimerManager.IsTimerActive(EnergyIncreaseHandle))
	{
		StopEnergyIncrease();
	}

	FTimerDelegate DecreaseDelegate;
	DecreaseDelegate.BindUFunction(this, FName("DecreaseEnergy"), DecreaseAmount);
	TimerManager.SetTimer(EnergyDecreaseHandle, DecreaseDelegate, DecreaseInterval, true);
	return true;
}

bool UBatteryManager::StopEnergyDecrease()
{
	if (!GetWorld()) return false;

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (!TimerManager.IsTimerActive(EnergyDecreaseHandle)) return false;

	TimerManager.ClearTimer(EnergyDecreaseHandle);
	return true;
}

bool UBatteryManager::StartEnergyIncrease()
{
	if (!GetWorld() || Energy >= MaxEnergy) return false;

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(EnergyIncreaseHandle)) return false;

	if (TimerManager.IsTimerActive(EnergyDecreaseHandle))
	{
		StopEnergyDecrease();
	}

	FTimerDelegate IncreaseDelegate;
	IncreaseDelegate.BindUFunction(this, FName("IncreaseEnergy"), IncreaseAmount);
	TimerManager.SetTimer(EnergyIncreaseHandle, IncreaseDelegate, IncreaseInterval, true);
	return true;
}

bool UBatteryManager::StopEnergyIncrease()
{
	if (!GetWorld()) return false;

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (!TimerManager.IsTimerActive(EnergyIncreaseHandle)) return false;

	TimerManager.ClearTimer(EnergyIncreaseHandle);
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

void UBatteryManager::CalculateIntervals()
{
	DecreaseInterval = 1 / DecreaseRate;
	IncreaseInterval = 1 / IncreaseRate;
}

void UBatteryManager::EquipBattery()
{
	bIsBatteryEquipped = true;
	AttachBatteryToSocket(Cast<ACharacter>(GetOwner())->GetMesh(), BatterySocketName);
	StartEnergyIncrease();
}

void UBatteryManager::UnequipBattery(USkeletalMeshComponent* SkeletalMesh)
{
	if (!bIsBatteryEquipped) return;
	bIsBatteryEquipped = false;
	AttachBatteryToSocket(SkeletalMesh, BatterySocketName);
}

void UBatteryManager::AttachBatteryToSocket(USkeletalMeshComponent* SkeletalMesh, const FName SocketName)
{
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	BatteryActor->AttachToComponent(SkeletalMesh, AttachmentRules, SocketName);
}
