﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UENUM(BlueprintType)
enum class EPlatformState : uint8
{
	Moving,
	Idle
};

UCLASS()
class GMTK_2021_API AMovingPlatform : public AActor
{
	GENERATED_BODY()

public:
	AMovingPlatform();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* PlatformMesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UTimelineComponent* MovementTimeline = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	UCurveFloat* MovementCurve;
public:
	UPROPERTY(EditAnywhere, Category="Platform")
	TSet<AActor*> PathPoints;
	UFUNCTION(BlueprintCallable, Category="Platform")
	bool StartMoving();

private:
	FVector InitialLocation;
	TArray<FVector> PathLocations;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true", ClampMin = "0.0"))
	float TravelTime = 3.f;
	void CalculatePlayRate() const;
	UFUNCTION()
	void SetPlatformLocation(const float PathProgress);
	UFUNCTION()
	void FinishMovement();
	
};