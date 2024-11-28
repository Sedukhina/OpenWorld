// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterAnimInstance.h"

#include "BaseCharacter.h"
#include "Components/BaseCharacterMovementComponent.h"

void UBaseCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	CachedOwner = StaticCast<ABaseCharacter*>(TryGetPawnOwner());
}

void UBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!CachedOwner.IsValid())
	{
		return;
	}
	UBaseCharacterMovementComponent* OwnerCharacterMovementComponent = CachedOwner->GetBaseCharacterMovementComponent();
	Speed = OwnerCharacterMovementComponent->Velocity.Size();
	bIsFalling = OwnerCharacterMovementComponent->IsFalling();
	bIsSwimming = OwnerCharacterMovementComponent->IsSwimming();
	bIsWalking = OwnerCharacterMovementComponent->IsWalking();
	bIsClimbing = OwnerCharacterMovementComponent->IsOnLadder();
}
