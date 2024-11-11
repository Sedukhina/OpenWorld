// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LedgeDetectorComponent.generated.h"

USTRUCT()
struct FLedge
{
	GENERATED_BODY()

	FVector Location;
	FRotator Rotation;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPENWORLD_API ULedgeDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TWeakObjectPtr<class ACharacter> CachedCharacterOwner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxLedgeHeight = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinLedgeHeight = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxDistanceToLedge = 100.f;

public:
	bool DetectLedge(OUT FLedge&Ledge);
};
