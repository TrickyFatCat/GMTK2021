// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "BatteryManager.h"
#include "Battery.h"
#include "InteractionManager.h"
#include "InteractNotify.h"
#include "InteractiveActors/BatteryStation.h"
#include "GMTK_2021/Public/Utils.h"

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
	InteractionManager = CreateDefaultSubobject<UInteractionManager>("InteractionManager");
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	BatteryManager->OnDeath.AddUObject(this, &APlayerCharacter::OnDeath);

	if (GroundInteractionMontage && StandInteractionMontage)
	{
		InitAnimations();
	}
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
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::StartInteraction);
}

void APlayerCharacter::OnWin()
{
	SetInputEnabled(false);
	OnPlayerWin.Broadcast();
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
	
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();

	if (MovementComponent)
	{
		const float Speed = MovementComponent->Velocity.Size();
		MovementComponent->MaxWalkSpeed = FMath::Min(Speed + Acceleration, MaxSpeed);
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

void APlayerCharacter::SetInputEnabled(const bool bIsEnabled)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (!PlayerController) return;

	if (bIsEnabled)
	{
		EnableInput(PlayerController);
	}
	else
	{
		DisableInput(PlayerController);
	}
}

void APlayerCharacter::OnDeath()
{
	GetCharacterMovement()->DisableMovement();
	SetInputEnabled(false);
	PlayAnimMontage(DeathMontage);
	OnPlayerDeath.Broadcast();
}

void APlayerCharacter::StartInteraction()
{
	if (InteractionManager->IsQueueEmpty()) return;

	AActor* TargetActor = InteractionManager->GetTargetActor();

	if (!IsValid(TargetActor)) return;

	UAnimMontage* TargetMontage = TargetActor->IsA(ABatteryStation::StaticClass())
		                              ? GroundInteractionMontage
		                              : StandInteractionMontage;

	if (!InteractionManager->CanInteract()) return;

	if (!TargetMontage) return;

	SetInputEnabled(false);
	PlayAnimMontage(TargetMontage);
}

void APlayerCharacter::FinishInteraction()
{
	SetInputEnabled(true);
	InteractionManager->Interact();
}

void APlayerCharacter::InitAnimations()
{
	UInteractNotify* InteractNotify = Utils::FindFirstNotifyByClass<UInteractNotify>(GroundInteractionMontage);

	if (InteractNotify)
	{
		InteractNotify->OnNotified.AddUObject(this, &APlayerCharacter::FinishInteraction);
	}

	InteractNotify = Utils::FindFirstNotifyByClass<UInteractNotify>(StandInteractionMontage);

	if (InteractNotify)
	{
		InteractNotify->OnNotified.AddUObject(this, &APlayerCharacter::FinishInteraction);
	}
}
