// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OpenWorldPlayerController.generated.h"


struct FInputActionValue;
/**
 * 
 */
UCLASS(Blueprintable)
class OPENWORLD_API AOpenWorldPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

protected:

	TWeakObjectPtr<class ABaseCharacter> CachedCharacter;

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
	class UInputAction* MantleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* SwimAction;

	void Look(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);

	void Move(const FInputActionValue& Value);
	void Swim(const FInputActionValue& Value);

	void Jump();
	void StopJumping();
	void Mantle();

};
