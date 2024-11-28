// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterMovementComponent.h"
#include "..\\BaseCharacter.h"
#include <Components/CapsuleComponent.h>
#include <OpenWorld/Actors/Ladder.h>
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

ALadder* UBaseCharacterMovementComponent::GetCurrentLadder()
{
	return CurrentLadder;
}

void UBaseCharacterMovementComponent::AttachToLadder(ALadder* Ladder)
{
	if (IsValid(Ladder))
	{
		CurrentLadder = Ladder;
		SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_OnLadder);
		FVector TargetLocation = FVector(Ladder->GetActorLocation().X, Ladder->GetActorLocation().Y, GetActorLocation().Z) + Ladder->GetActorForwardVector() * CachedCharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius();
		FVector LocationDelta = TargetLocation - GetActorLocation();
		FHitResult HitResult;
		SafeMoveUpdatedComponent(LocationDelta, Ladder->GetActorRotation().Add(0.f, 180.f, 0.f), false, HitResult);
	}
}

void UBaseCharacterMovementComponent::DetachFromLadder()
{
	SetMovementMode(MOVE_Falling);
	CurrentLadder = nullptr;
}

bool UBaseCharacterMovementComponent::IsOnLadder() const
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_OnLadder;
}

void UBaseCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
		case((uint8)ECustomMovementMode::CMOVE_Mantling):
		{
			PhysMantle(DeltaTime, Iterations);
			break;
		}
		case((uint8)ECustomMovementMode::CMOVE_OnLadder):
		{
			PhysLadder(DeltaTime, Iterations);
			break;
		}
		default:
			break;
	}

	Super::PhysCustom(DeltaTime, Iterations);
}

void UBaseCharacterMovementComponent::PhysMantle(float DelatTime, int32 Iterations)
{
	float TimeElapsed = (GetWorld()->GetTimerManager().GetTimerElapsed(MantlingTimer)) + CurrentMantlingParameters.StartTime;

	FVector CurveValue = CurrentMantlingParameters.MantlingCurve->GetVectorValue(TimeElapsed);

	FVector CorrectedInitialLocation = FMath::Lerp(CurrentMantlingParameters.CharacterInitialLocation, CurrentMantlingParameters.InitialAnimationLocation, CurveValue.Y);
	CorrectedInitialLocation.Z = FMath::Lerp(CurrentMantlingParameters.CharacterInitialLocation.Z, CurrentMantlingParameters.InitialAnimationLocation.Z, CurveValue.Z);

	FVector CharacterTargetLocation = CurrentMantlingParameters.ComponentLedgeAttachedTo->GetComponentLocation() + CurrentMantlingParameters.CharacterTargetRelativeLocation;
	FVector CurrentLocation = FMath::Lerp(CorrectedInitialLocation, CharacterTargetLocation, CurveValue.X);
	FRotator CurrentRotation = FMath::Lerp(CurrentMantlingParameters.CharacterInitialRotation, CurrentMantlingParameters.CharacterTargetRotation, CurveValue.X);

	FVector LocationDelta = CurrentLocation - GetActorLocation();
	FHitResult HitResult;
	SafeMoveUpdatedComponent(LocationDelta, CurrentRotation, false, HitResult);
}

void UBaseCharacterMovementComponent::PhysLadder(float DeltaTime, int32 Iterations)
{
	// Delta, friction force, fluid, deccelaration speed
	CalcVelocity(DeltaTime, 1.f, false, 2048.f);
	FVector LocationDelta = Velocity * DeltaTime;
	FHitResult HitResult;
	SafeMoveUpdatedComponent(LocationDelta, FQuat::Identity, false, HitResult);
}

float UBaseCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	if (IsSwimming())
	{
		MaxSpeed = SwimmingSpeed;
	}
	else if (IsOnLadder())
	{
		MaxSpeed = ClimbingLadderSpeed;
	}

	return MaxSpeed;
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

