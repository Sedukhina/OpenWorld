// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActor.h"
#include "Components/CapsuleComponent.h"
#include "../Characters/BaseCharacter.h"

// Called when the game starts or when spawned
void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	
	InteractiveVolume->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveActor::OnInteractionVolumeOverlapBegin);
	InteractiveVolume->OnComponentEndOverlap.AddDynamic(this, &AInteractiveActor::OnInteractionVolumeOverlapEnd);
}

void AInteractiveActor::OnInteractionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* OverlappedCharacter = Cast<ABaseCharacter>(OtherActor);
	if (IsValid(OverlappedCharacter))
	{
		if (Cast<UCapsuleComponent>(OtherComp) == OverlappedCharacter->GetCapsuleComponent())
		{
			OverlappedCharacter->RegisterInteractiveActor(this);
		}
	}
}

void AInteractiveActor::OnInteractionVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABaseCharacter* OverlappedCharacter = Cast<ABaseCharacter>(OtherActor);
	if (IsValid(OverlappedCharacter))
	{
		if (Cast<UCapsuleComponent>(OtherComp) == OverlappedCharacter->GetCapsuleComponent())
		{
			OverlappedCharacter->UnregisterInteractiveActor(this);
		}
	}
}


