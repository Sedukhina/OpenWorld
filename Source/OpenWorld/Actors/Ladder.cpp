// Fill out your copyright notice in the Description page of Project Settings.


#include "Ladder.h"

#include "Components/InstancedStaticMeshComponent.h"
#include <Components/BoxComponent.h>
#include <OpenWorld/CollisionChannelsAliases.h>

ALadder::ALadder()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	RightRailMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Rail"));
	RightRailMeshComponent->SetupAttachment(RootComponent);

	LeftRailMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Rail"));
	LeftRailMeshComponent->SetupAttachment(RootComponent);

	StepsMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Steps"));
	StepsMeshComponent->SetupAttachment(RootComponent);

	InteractiveVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Interactive Volume"));
	InteractiveVolume->SetupAttachment(RootComponent);
}

void ALadder::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	LeftRailMeshComponent->SetRelativeLocation(FVector(0.f, 0.5f * LadderWidth, 1.f));
	RightRailMeshComponent->SetRelativeLocation(FVector(0.f, -0.5f * LadderWidth, 1.f));

	UStaticMesh* LeftRailMesh = LeftRailMeshComponent->GetStaticMesh();
	if (IsValid(LeftRailMesh))
	{
		float MeshHeight = LeftRailMesh->GetBoundingBox().GetSize().Z;
		if (!FMath::IsNearlyZero(MeshHeight))
		{
			LeftRailMeshComponent->SetRelativeScale3D(FVector(1.f, 1.f, LadderHeight / MeshHeight));
		}
	}

	UStaticMesh* RightRailMesh = RightRailMeshComponent->GetStaticMesh();
	if (IsValid(RightRailMesh))
	{
		float MeshHeight = RightRailMesh->GetBoundingBox().GetSize().Z;
		if (!FMath::IsNearlyZero(MeshHeight))
		{
			RightRailMeshComponent->SetRelativeScale3D(FVector(1.f, 1.f, LadderHeight / MeshHeight));
		}
	}

	UStaticMesh* StepMesh = StepsMeshComponent->GetStaticMesh();
	if (IsValid(StepMesh))
	{
		float MeshWidth = StepMesh->GetBoundingBox().GetSize().Y;
		if (!FMath::IsNearlyZero(MeshWidth))
		{
			StepsMeshComponent->SetRelativeScale3D(FVector(1.f, LadderWidth/MeshWidth, 1.f));
		}
	}
	StepsMeshComponent->ClearInstances();
	int StepsAmount = FMath::FloorToInt((LadderHeight - FirstStepOffset) / StepHeight);
	for (size_t i = 0; i < StepsAmount; i++)
	{
		FTransform InstanceTransform(FVector(1.f, 1.f, FirstStepOffset + i * StepHeight));
		StepsMeshComponent->AddInstance(InstanceTransform);
	}


	UBoxComponent* InteractiveBox = GetLadderInteractionBoxVolume();
	FVector InteractiveBoxVolumeExtent = FVector(0.5 * LadderWidth, 0.5 * LadderWidth, 0.5 * LadderHeight);
	// Extend is the half of the size of the box
	InteractiveBox->SetBoxExtent(InteractiveBoxVolumeExtent);
	InteractiveBox->SetRelativeLocation(FVector(InteractiveBoxVolumeExtent.X, 0.f, InteractiveBoxVolumeExtent.Z));
	InteractiveBox->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	InteractiveBox->SetGenerateOverlapEvents(true);
}

UBoxComponent* ALadder::GetLadderInteractionBoxVolume() const
{
	return StaticCast<UBoxComponent*>(InteractiveVolume);
}
