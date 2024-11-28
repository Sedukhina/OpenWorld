// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWorldPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Characters/BaseCharacter.h"

#include "InputActionValue.h"

void AOpenWorldPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}

	CachedCharacter = Cast<ABaseCharacter>(GetPawn());
}

void AOpenWorldPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOpenWorldPlayerController::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AOpenWorldPlayerController::Look);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AOpenWorldPlayerController::Zoom);
		EnhancedInputComponent->BindAction(MantleAction, ETriggerEvent::Triggered, this, &AOpenWorldPlayerController::Mantle);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AOpenWorldPlayerController::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AOpenWorldPlayerController::StopJumping);
		EnhancedInputComponent->BindAction(SwimAction, ETriggerEvent::Triggered, this, &AOpenWorldPlayerController::Swim);
		EnhancedInputComponent->BindAction(InteractLadderAction, ETriggerEvent::Triggered, this, &AOpenWorldPlayerController::InteractWithLadder);
		EnhancedInputComponent->BindAction(ClimbUpLadderAction, ETriggerEvent::Triggered, this, &AOpenWorldPlayerController::ClimbUpLadder);
	}
}

void AOpenWorldPlayerController::Look(const FInputActionValue& Value)
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->Look(Value);
	}
}

void AOpenWorldPlayerController::Zoom(const FInputActionValue& Value)
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->Zoom(Value);
	}
}

void AOpenWorldPlayerController::Move(const FInputActionValue& Value)
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->Move(Value);
	}
}

void AOpenWorldPlayerController::ClimbUpLadder(const FInputActionValue& Value)
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->ClimbUpLadder(Value);
	}
}

void AOpenWorldPlayerController::InteractWithLadder()
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->InteractWithLadder();
	}
}

void AOpenWorldPlayerController::Swim(const FInputActionValue& Value)
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->Swim(Value);
	}
}

void AOpenWorldPlayerController::Jump()
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->Jump();
	}
}

void AOpenWorldPlayerController::StopJumping()
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->StopJumping();
	}
}

void AOpenWorldPlayerController::Mantle()
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->Mantle();
	}
}
