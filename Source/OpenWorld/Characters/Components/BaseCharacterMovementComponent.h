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
	CMOVE_Max UMETA(Hidden)
};

struct FLedge;

USTRUCT()
struct FMantlingParameters
{
	GENERATED_BODY()

	FVector CharacterInitialLocation = FVector::ZeroVector;
	FRotator CharacterInitialRotation = FRotator::ZeroRotator;

	FVector CharacterTargetLocation = FVector::ZeroVector;;
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

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	void StartMantle(FMantlingParameters& MantlingParameters);
	void EndMantling();
	bool IsMantling();

protected:
	TWeakObjectPtr<class ABaseCharacter> CachedCharacterOwner;

	virtual void PhysCustom(float DelatTime, int32 Iterations) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Swimming")
	float SwimmingtCapsuleHalfHeight = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Swimming")
	float SwimmingtCapsuleRadius = 60.f;

	// Mantling
	FTimerHandle MantlingTimer;
	FMantlingParameters CurrentMantlingParameters;
};
