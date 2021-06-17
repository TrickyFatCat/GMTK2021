// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/PlayerHUDWidget.h"
#include "Actors/PlayerCharacter.h"
#include "GMTK_2021/Public/Utils.h"

bool UPlayerHUDWidget::Initialize()
{
	UBatteryManager* BatteryManager = Utils::GetPlayerComponent<UBatteryManager>(GetOwningPlayerPawn());

	if (BatteryManager)
	{
		BatteryManager->OnEnergyChanged.AddUObject(this, &UPlayerHUDWidget::OnEnergyChanged);
	}

	return Super::Initialize();
}

float UPlayerHUDWidget::GetNormalizedEnergy() const
{
	const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());

	if (!PlayerCharacter) return 0.f;

	return PlayerCharacter->GetNormalizedEnergy();
}

float UPlayerHUDWidget::GetIsPlayerAlive() const
{
	return GetNormalizedEnergy() > 0.f;
}

bool UPlayerHUDWidget::GetCanPlayerInteract() const
{
	const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());

	if (!PlayerCharacter) return false;

	return PlayerCharacter->CanInteract();
}

AActor* UPlayerHUDWidget::GetPlayerTargetActor() const
{
	const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());

	if (!PlayerCharacter) return nullptr;

	return PlayerCharacter->GetTargetActor();
}

bool UPlayerHUDWidget::GetIsBatteryEquipped() const
{
	const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());

	if (!PlayerCharacter) return false;

	return PlayerCharacter->IsBatteryEquipped();
}

void UPlayerHUDWidget::OnEnergyChanged(const float Energy, const float DeltaEnergy)
{
	if (DeltaEnergy < 0)
	{
		OnEnergyDecrease();
	}
	else
	{
		OnEnergyIncrease();
	}
}
