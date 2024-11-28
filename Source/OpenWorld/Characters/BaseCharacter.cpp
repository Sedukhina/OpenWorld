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
#include <Curves/CurveVector.h>

#include "../Actors/InteractiveActor.h"

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

void ABaseCharacter::RegisterInteractiveActor(AInteractiveActor* Actor)
{
	if (IsValid(Actor))
	{
		InteractiveActors.Add(Actor);
	}
}

void ABaseCharacter::UnregisterInteractiveActor(AInteractiveActor* Actor)
{
	if (IsValid(Actor))
	{
		InteractiveActors.Remove(Actor);
	}
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
	if (CanMantle() && MantlingSettingsArray.Num() != 0 && LedgeDetector->DetectLedge(Ledge))
	{
		FMantlingParameters MantlingParameters;
		MantlingParameters.ComponentLedgeAttachedTo = Ledge.ComponentLedgeAttachedTo;
		MantlingParameters.CharacterInitialLocation = GetActorLocation();
		MantlingParameters.CharacterInitialRotation = GetActorRotation();
		MantlingParameters.CharacterTargetRelativeLocation = Ledge.RelativeLocation;
		MantlingParameters.CharacterTargetRotation = Ledge.Rotation;

		float LedgeHeight = (Ledge.ComponentLedgeAttachedTo->GetComponentLocation() + Ledge.RelativeLocation).Z - MantlingParameters.CharacterInitialLocation.Z;
		for (FMantlingSettings& MantlingSettings : MantlingSettingsArray)
		{
			if (LedgeHeight >= MantlingSettings.MinLedgeHeight && LedgeHeight <= MantlingSettings.MaxLedgeHeight)
			{
				MantlingParameters.MantlingCurve = MantlingSettings.MantlingCurve;

				float MinRange, MaxRange;
				MantlingSettings.MantlingCurve->GetTimeRange(MinRange, MaxRange);
				MantlingParameters.Duration = MaxRange - MinRange;

				FVector2D SourceRange(MantlingSettings.MinLedgeHeight, MantlingSettings.MaxLedgeHeight);
				FVector2D TargetRange(MantlingSettings.MinLedgeHeightStartTime, MantlingSettings.MaxLedgeHeightStartTime);

				MantlingParameters.StartTime = FMath::GetMappedRangeValueClamped(SourceRange, TargetRange, LedgeHeight);
					
				MantlingParameters.InitialAnimationLocation = Ledge.ComponentLedgeAttachedTo->GetComponentLocation() + Ledge.RelativeLocation - FVector::UpVector * MantlingSettings.AnimationCorrectionZ + Ledge.Normal * MantlingSettings.AnimationCorrectionXY;

				BaseCharacterMovementComponent->StartMantle(MantlingParameters);
				GetMesh()->GetAnimInstance()->Montage_Play(MantlingSettings.MantlingMontage, 1.0f, EMontagePlayReturnType::Duration, MantlingParameters.StartTime);
				break;
			}
		}
	}
}

bool ABaseCharacter::CanMantle()
{
	return bCanMantle && !BaseCharacterMovementComponent->IsMantling();
}

