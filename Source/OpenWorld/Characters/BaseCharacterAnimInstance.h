// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLD_API UBaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	TWeakObjectPtr<class ABaseCharacter> CachedOwner;

	//Current Character speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Speed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsSwimming = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsWalking = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsClimbing = false;
};
