// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);

	MovingSpeed = 300;
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		// Set replicates on client is not allowed
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!NextTarget || !CurrentTarget || !bCanMove) return;

	if (HasAuthority())
	{
		FVector nextLoc = GetActorLocation();
		nextLoc += (CurrentTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal() * MovingSpeed * DeltaSeconds;
		SetActorLocation(nextLoc);

		if (FVector::Dist(GetActorLocation(), CurrentTarget->GetActorLocation()) < 10.f)
		{
			AActor* temp = CurrentTarget;
			CurrentTarget = NextTarget;
			NextTarget = temp;
		}
	}

}
