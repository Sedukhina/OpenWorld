// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveActor.h"
#include "Ladder.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class OPENWORLD_API ALadder : public AInteractiveActor
{
	GENERATED_BODY()

public:
	ALadder();

	virtual void OnConstruction(const FTransform& Transform) override;

	float GetLadderHeight();

protected:
	// Ladder parametrs
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LadderHeight = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LadderWidth = 60.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StepHeight = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FirstStepOffset = 40.f;

	// Ladder components
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* RightRailMeshComponent;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LeftRailMeshComponent;
	UPROPERTY(VisibleAnywhere)
	class UInstancedStaticMeshComponent* StepsMeshComponent;

	class UBoxComponent* GetLadderInteractionBoxVolume() const;

};
