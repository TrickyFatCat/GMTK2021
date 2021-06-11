// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "BatteryManager.h"
#include "Battery.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 750.f;
	SpringArm->SetRelativeRotation(FRotator(-55.f, 225.f, 0.f));
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	PlayerCamera->SetupAttachment(SpringArm);
	PlayerCamera->FieldOfView = 100.f;

	GetCapsuleComponent()->SetCapsuleHalfHeight(40.f);
	GetCapsuleComponent()->SetCapsuleRadius(15.f);

	GetCharacterMovement()->bOrientRotationToMovement = true;

	BatteryManager = CreateDefaultSubobject<UBatteryManager>("BatteryManager");
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
}

void APlayerCharacter::MoveForward(const float AxisValue)
{
	if (!IsValid(Controller)) return;

	if (AxisValue != 0.f)
	{
		const FRotator ControllerRotation = Controller->GetControlRotation();
		const FRotator NewYawRotation(0.f, ControllerRotation.Yaw, 0.f);
		const FVector MoveDirection = FRotationMatrix(NewYawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(MoveDirection, AxisValue);
	}
}

void APlayerCharacter::MoveRight(const float AxisValue)
{
	if (!IsValid(Controller)) return;

	if (AxisValue != 0.f)
	{
		const FRotator ControllerRotation = Controller->GetControlRotation();
		const FRotator NewYawRotation(0.f, ControllerRotation.Yaw, 0.f);
		const FVector MoveDirection = FRotationMatrix(NewYawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(MoveDirection, AxisValue);
	}
}

