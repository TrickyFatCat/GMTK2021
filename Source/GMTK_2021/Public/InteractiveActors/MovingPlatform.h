// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Containers/Set.h"
#include "MovingPlatform.generated.h"

UENUM(BlueprintType)
enum class EPlatformState : uint8
{
	Moving,
	Idle
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlatformStopMovingSignature);

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
	TSet<AActor*> TargetPoints;
	UFUNCTION(BlueprintCallable, Category="Platform")
	bool StartMoving();
	UPROPERTY(BlueprintAssignable, Category="Platform")
	FOnPlatformStopMovingSignature OnPlatformStopMoving;

private:
	FVector InitialLocation;
	UPROPERTY(VisibleAnywhere)
	int32 CurrentPointIndex = 0;
	UPROPERTY(VisibleAnywhere)
	TArray<FVector> PathLocations;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true", ClampMin = "0.0"))
	float TravelTime = 3.f;
	void CalculatePlayRate() const;
	UFUNCTION()
	void SetPlatformLocation(const float PathProgress);
	UFUNCTION()
	void FinishMovement();
	
};
