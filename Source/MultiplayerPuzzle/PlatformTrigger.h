// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformTrigger.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UPrimitiveComponent;
class AMovingPlatform;
UCLASS()
class MULTIPLAYERPUZZLE_API APlatformTrigger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlatformTrigger();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component)
		UBoxComponent* OverlapComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = Reference)
		TArray< AMovingPlatform* > m_controledPlatforms;

	uint8 m_enteredPlayer;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
