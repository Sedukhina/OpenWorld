// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"

// Camera an Camera boom
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Components/BaseCharacterMovementComponent.h"
#include "Components/LedgeDetectorComponent.h"
#include <Components/CapsuleComponent.h>

// Sets default values
ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creating Camera Boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Creating Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepRelativeTransform);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;

	BaseCharacterMovementComponent = StaticCast<UBaseCharacterMovementComponent*>(GetCharacterMovement());

	LedgeDetector = CreateDefaultSubobject<ULedgeDetectorComponent>(TEXT("Ledge Detector"));
}

UBaseCharacterMovementComponent* ABaseCharacter::GetBaseCharacterMovementComponent()
{
	return BaseCharacterMovementComponent;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}


bool ABaseCharacter::CanJumpInternal_Implementation() const
{
	return (!BaseCharacterMovementComponent->IsMantling()) && Super::CanJumpInternal_Implementation();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	if (Controller)
	{
		FVector2D LookRotVector = Value.Get<FVector2D>();
		AddControllerYawInput(LookRotVector.X);
		AddControllerPitchInput(LookRotVector.Y);
	}
}

void ABaseCharacter::Zoom(const FInputActionValue& Value)
{
	float ZoomValue = Value.Get<float>();
	CameraBoom->TargetArmLength += ZoomValue * ZoomSpeed;
	CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength, MinTargetArmLength, MaxTargetArmLength);
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	// Checling if controller posessing this actor isn't null
	if (Controller && BaseCharacterMovementComponent->MovementMode == MOVE_Walking)
	{
		// Getting movement vector from IA value 
		FVector2D MovementVector = Value.Get<FVector2D>();

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotationMatrix YawRotationMatrix(FRotator(0, Rotation.Yaw, 0));

		AddMovementInput(YawRotationMatrix.GetUnitAxis(EAxis::X), MovementVector.Y);
		AddMovementInput(YawRotationMatrix.GetUnitAxis(EAxis::Y), MovementVector.X);
	}
}

void ABaseCharacter::Swim(const FInputActionValue& Value)
{
	// Checling if controller posessing this actor isn't null
	if (Controller && BaseCharacterMovementComponent->MovementMode == MOVE_Swimming)
	{
		FVector MovementVector = Value.Get<FVector>();
		const FRotator Rotation = Controller->GetControlRotation();

		MovementVector = Rotation.RotateVector(MovementVector);
		AddMovementInput(MovementVector);
	}
}

void ABaseCharacter::Mantle()
{
	FLedge Ledge;
	if (LedgeDetector->DetectLedge(Ledge))
	{
		BaseCharacterMovementComponent->StartMantle(Ledge);
	}
}

