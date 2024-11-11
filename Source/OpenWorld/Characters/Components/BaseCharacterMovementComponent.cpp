// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterMovementComponent.h"
#include "..\\BaseCharacter.h"
#include <Components/CapsuleComponent.h>

void UBaseCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	CachedCharacterOwner = StaticCast<ABaseCharacter*>(GetOwner());
	bOrientRotationToMovement = true;
}

void UBaseCharacterMovementComponent::StartMantle(FLedge &Ledge)
{
	MantlingTargetLedge = Ledge;
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
			float TimerProgressRatio = (GetWorld()->GetTimerManager().GetTimerElapsed(MantlingTimer))/MantlingTime;
			FVector CurrentLocation = FMath::Lerp(CharacterInitialLocation, MantlingTargetLedge.Location, TimerProgressRatio);
			FRotator CurrentRotation = FMath::Lerp(CharacterInitialRotation, MantlingTargetLedge.Rotation, TimerProgressRatio);
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
				CharacterInitialLocation = GetActorLocation();
				CharacterInitialRotation = CachedCharacterOwner->GetActorRotation();
				GetWorld()->GetTimerManager().SetTimer(MantlingTimer, this, &UBaseCharacterMovementComponent::EndMantling, MantlingTime);

				break;
			}
			default:
				break;
		}
	}
}

