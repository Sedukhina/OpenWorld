// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

USTRUCT()
struct FMantlingSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float MinLedgeHeight = 100.f;
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float MaxLedgeHeight = 200.f;

	// Animation Start Time Range
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float MinLedgeHeightStartTime = 0.5f;
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float MaxLedgeHeightStartTime = 0.f;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* MantlingMontage;
	UPROPERTY(EditDefaultsOnly)
	class UCurveVector* MantlingCurve;

	// Ledge offset, value around capsule radius usually picked
	UPROPERTY(EditDefaultsOnly)
	float AnimationCorrectionXY = 60.f;
	UPROPERTY(EditDefaultsOnly)
	// Height animation was made for
	float AnimationCorrectionZ = 150.f;
};

UCLASS(Blueprintable)
class OPENWORLD_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);

	class UBaseCharacterMovementComponent* GetBaseCharacterMovementComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseCharacter|Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseCharacter|Camera")
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseCharacter|Camera")
	float MinTargetArmLength = 120.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseCharacter|Camera")
	float MaxTargetArmLength = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BaseCharacter|Camera", meta = (ClampMin = 0.5f, UIMin = 0.5f))
	float ZoomSpeed = 3.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Movement|Mantling")
	class ULedgeDetectorComponent* LedgeDetector;

	UPROPERTY(EditDefaultsOnly, Category = "Character|Movement|Mantling")
	TArray<FMantlingSettings> MantlingSettingsArray;

	class UBaseCharacterMovementComponent* BaseCharacterMovementComponent;

	virtual bool CanJumpInternal_Implementation() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanMantle = true;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Look(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);

	// Movement Functions
	void Move(const FInputActionValue& Value);
	void Swim(const FInputActionValue& Value);

	void Mantle();
	bool CanMantle();
};
