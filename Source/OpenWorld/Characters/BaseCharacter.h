// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BasePawn|Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BasePawn|Camera")
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BasePawn|Camera")
	float MinTargetArmLength = 120.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BasePawn|Camera")
	float MaxTargetArmLength = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BasePawn|Camera", meta = (ClampMin = 0.5f, UIMin = 0.5f))
	float ZoomSpeed = 3.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ULedgeDetectorComponent* LedgeDetector;

	// Input Mapping
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* MappingContext;

	// Input Actions 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* ZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* SwimAction;

	class UBaseCharacterMovementComponent* BaseCharacterMovementComponent;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	float DefaultCapsuleHalfHeight;
	float DefaultCapsuleRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Swimming")
	float SwimmingtCapsuleHalfHeight = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Swimming")
	float SwimmingtCapsuleRadius = 60.f;

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

	virtual void Jump() override;
};
