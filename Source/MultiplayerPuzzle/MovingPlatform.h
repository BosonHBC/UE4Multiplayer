// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPUZZLE_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
		AMovingPlatform();


		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Moving)
		AActor* NextTarget;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Moving)
		AActor* CurrentTarget;

		bool bCanMove = false;

protected:
	void BeginPlay();
	void Tick(float DeltaSeconds);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Moving)
		float MovingSpeed;
};
