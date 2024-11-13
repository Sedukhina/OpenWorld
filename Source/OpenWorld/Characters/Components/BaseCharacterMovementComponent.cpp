// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterMovementComponent.h"
#include "..\\BaseCharacter.h"
#include <Components/CapsuleComponent.h>
#include <Curves/CurveVector.h>

void UBaseCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	CachedCharacterOwner = StaticCast<ABaseCharacter*>(GetOwner());
	bOrientRotationToMovement = true;
}

void UBaseCharacterMovementComponent::StartMantle(FMantlingParameters& MantlingParameters)
{
	CurrentMantlingParameters = MantlingParameters;
	SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_Mantling);
}

void UBaseCharacterMovementComponent::EndMantling()
{
	SetMovementMode(MOVE_Walking);
}

bool UBaseCharacterMovementComponent::IsMantling()
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_Mantling;
}

void UBaseCharacterMovementComponent::PhysCustom(float DelatTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
		case((uint8)ECustomMovementMode::CMOVE_Mantling):
		{
			float TimeElapsed = (GetWorld()->GetTimerManager().GetTimerElapsed(MantlingTimer)) + CurrentMantlingParameters.StartTime;

			FVector CurveValue = CurrentMantlingParameters.MantlingCurve->GetVectorValue(TimeElapsed);

			FVector CorrectedInitialLocation = FMath::Lerp(CurrentMantlingParameters.CharacterInitialLocation, CurrentMantlingParameters.InitialAnimationLocation, CurveValue.Y);
			CorrectedInitialLocation.Z = FMath::Lerp(CurrentMantlingParameters.CharacterInitialLocation.Z, CurrentMantlingParameters.InitialAnimationLocation.Z, CurveValue.Z);

			FVector CurrentLocation = FMath::Lerp(CorrectedInitialLocation, CurrentMantlingParameters.CharacterTargetLocation, CurveValue.X);
			FRotator CurrentRotation = FMath::Lerp(CurrentMantlingParameters.CharacterInitialRotation, CurrentMantlingParameters.CharacterTargetRotation, CurveValue.X);

			FVector LocationDelta = CurrentLocation - GetActorLocation();
			FHitResult HitResult;
			SafeMoveUpdatedComponent(LocationDelta, CurrentRotation, false, HitResult);
			break;
		}
		default:
			break;
	}

	Super::PhysCustom(DelatTime, Iterations);
}

void UBaseCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (PreviousMovementMode == MOVE_Swimming)
	{
		ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
		UCapsuleComponent* Capsule = DefaultCharacter->GetCapsuleComponent();
		CachedCharacterOwner->GetCapsuleComponent()->SetCapsuleSize(Capsule->GetUnscaledCapsuleRadius(), Capsule->GetUnscaledCapsuleHalfHeight());
	}
	else if (MovementMode == MOVE_Swimming)
	{
		CachedCharacterOwner->GetCapsuleComponent()->SetCapsuleSize(SwimmingtCapsuleRadius, SwimmingtCapsuleHalfHeight);
	}

	if (MovementMode == MOVE_Custom)
	{
		switch (CustomMovementMode)
		{
			case((uint8)ECustomMovementMode::CMOVE_Mantling):
			{
				GetWorld()->GetTimerManager().SetTimer(MantlingTimer, this, &UBaseCharacterMovementComponent::EndMantling, CurrentMantlingParameters.Duration);
				break;
			}
			default:
				break;
		}
	}
}

