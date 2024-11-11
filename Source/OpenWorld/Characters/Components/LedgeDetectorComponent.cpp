// Fill out your copyright notice in the Description page of Project Settings.


#include "LedgeDetectorComponent.h"

#include "GameFramework/Character.h"
#include <Components/CapsuleComponent.h>

#include "../../CollisionChannelsAliases.h"

// Called when the game starts
void ULedgeDetectorComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedCharacterOwner = StaticCast<ACharacter*>(GetOwner());
}

bool ULedgeDetectorComponent::DetectLedge(OUT FLedge& Ledge)
{
	UCapsuleComponent* PlayerCapsule = CachedCharacterOwner->GetCapsuleComponent();

	FCollisionQueryParams SweepsParams;
	SweepsParams.bTraceComplex = 1;
	SweepsParams.AddIgnoredActor(GetOwner());

	// Used to make a small offset of the floor to avoid unwanted collisions
	float GroundOffset = 2.f;
	FVector OwnerBottom = CachedCharacterOwner->GetActorLocation() - FVector::UpVector * (PlayerCapsule->GetScaledCapsuleHalfHeight() - GroundOffset);

	// Forward Check
	float ForwardCapsuleHalfHeight = (MaxLedgeHeight - MinLedgeHeight) / 2;
	FCollisionShape ForwardCapsule = FCollisionShape::MakeCapsule(PlayerCapsule->GetScaledCapsuleRadius(), ForwardCapsuleHalfHeight);
	// Player Bottom + MinLedgeHeight + HalfHeight of collision shape
	FVector ForwardCheckStart = OwnerBottom + FVector::UpVector * (ForwardCapsuleHalfHeight + MinLedgeHeight);
	FVector ForwardCheckEnd = ForwardCheckStart + CachedCharacterOwner->GetActorForwardVector() * MaxDistanceToLedge;

	FHitResult ForwardCheckHitResult;
	if (!GetWorld()->SweepSingleByChannel(ForwardCheckHitResult, ForwardCheckStart, ForwardCheckEnd, FQuat::Identity, ECC_Climbing, ForwardCapsule, SweepsParams))
	{
		return false;
	}

	// Downward Check
	float DownwardSphereRadius = PlayerCapsule->GetScaledCapsuleRadius();
	FCollisionShape DownwardSphere = FCollisionShape::MakeSphere(DownwardSphereRadius);
	FHitResult DownwardCheckHitResult;

	float ForwardOffset = 20.f;
	FVector DownwardCheckStart = ForwardCheckHitResult.ImpactPoint - ForwardOffset * ForwardCheckHitResult.ImpactNormal;
	DownwardCheckStart.Z = OwnerBottom.Z + MaxLedgeHeight + DownwardSphereRadius;
	FVector DownwardCheckEnd = DownwardCheckStart;
	DownwardCheckEnd.Z = OwnerBottom.Z + MinLedgeHeight + DownwardSphereRadius;

	if (!GetWorld()->SweepSingleByChannel(DownwardCheckHitResult, DownwardCheckStart, DownwardCheckEnd, FQuat::Identity, ECC_Climbing, DownwardSphere, SweepsParams))
	{
		return false;
	}

	// Overlap Check
	FCollisionShape OverlapCheckCapsule = FCollisionShape::MakeCapsule(PlayerCapsule->GetScaledCapsuleRadius(), PlayerCapsule->GetScaledCapsuleHalfHeight());
	FVector OverlapLocation = DownwardCheckHitResult.ImpactPoint + (PlayerCapsule->GetScaledCapsuleHalfHeight() + GroundOffset) * FVector::UpVector;

	DrawDebugCapsule(GetWorld(), OverlapLocation, PlayerCapsule->GetScaledCapsuleHalfHeight(), PlayerCapsule->GetScaledCapsuleRadius(), FQuat::Identity, FColor::Purple, false, 10.f);

	if (GetWorld()->OverlapAnyTestByProfile(OverlapLocation, FQuat::Identity, FName("Pawn"), OverlapCheckCapsule, SweepsParams))
	{
		return false;
	}

	Ledge.Location = OverlapLocation;
	// Making rotation opposite to normal, so character will face in that direction
	Ledge.Rotation = (ForwardCheckHitResult.ImpactNormal * FVector(-1, -1, 0)).ToOrientationRotator();
	return true;
}



