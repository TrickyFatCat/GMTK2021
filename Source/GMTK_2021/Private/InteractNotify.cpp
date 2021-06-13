// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractNotify.h"

void UInteractNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast();
	Super::Notify(MeshComp, Animation);
}
