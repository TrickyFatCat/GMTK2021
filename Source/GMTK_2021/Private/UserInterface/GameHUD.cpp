// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/GameHUD.h"

#include "Blueprint/UserWidget.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);

	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->AddToViewport();
	}
}
