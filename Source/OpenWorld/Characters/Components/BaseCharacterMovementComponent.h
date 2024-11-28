// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LedgeDetectorComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	CMOVE_None UMETA(DisplayName = "None"),
	CMOVE_Mantling UMETA(DisplayName = "Mantling"),
	CMOVE_OnLadder UMETA(DisplayName = "OnLadder"),
	CMOVE_Max UMETA(Hidden)
};

struct FLedge;

USTRUCT()
struct FMantlingParameters
{
	GENERATED_BODY()

	TWeakObjectPtr<UPrimitiveComponent> ComponentLedgeAttachedTo;

	FVector CharacterInitialLocation = FVector::ZeroVector;
	FRotator CharacterInitialRotation = FRotator::ZeroRotator;

	FVector CharacterTargetRelativeLocation = FVector::ZeroVector;;
	FRotator CharacterTargetRotation = FRotator::ZeroRotator;

	float Duration = 0.f;
	float StartTime = 0.f;
	UCurveVector* MantlingCurve;
	FVector InitialAnimationLocation;
};


/**
 * 
 */
UCLASS()
class OPENWORLD_API UBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual float GetMaxSpeed() const override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	void StartMantle(FMantlingParameters& MantlingParameters);
	void EndMantling();
	bool IsMantling();

	class ALadder* GetCurrentLadder();
	void AttachToLadder(class ALadder* Ladder);
	void DetachFromLadder();
	bool IsOnLadder() const;

protected:
	TWeakObjectPtr<class ABaseCharacter> CachedCharacterOwner;

	virtual void PhysCustom(float DelatTime, int32 Iterations) override;
	void PhysMantle(float DelatTime, int32 Iterations);
	void PhysLadder(float DelatTime, int32 Iterations);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Swimming")
	float SwimmingtCapsuleHalfHeight = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Swimming")
	float SwimmingtCapsuleRadius = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Swimming")
	float SwimmingSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ClimbingLadderSpeed = 300.f;

	// Mantling
	FTimerHandle MantlingTimer;
	FMantlingParameters CurrentMantlingParameters;

	class ALadder* CurrentLadder;
};
