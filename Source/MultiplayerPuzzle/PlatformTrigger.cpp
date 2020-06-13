// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformTrigger.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MultiplayerPuzzle/MultiplayerPuzzleCharacter.h"
#include "MovingPlatform.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap Comp"));
	SetRootComponent(OverlapComp);

	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionObjectType(ECC_WorldStatic);
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnOverlap);
	OverlapComp->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnEndOverlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(OverlapComp);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}


void APlatformTrigger::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr || OtherComp == OverlappedComp) return;

	AMultiplayerPuzzleCharacter* player = Cast<AMultiplayerPuzzleCharacter>(OtherActor);
	if (!player) return;

	UE_LOG(LogTemp, Log, TEXT("Player%d Enter"), m_enteredPlayer);
	m_enteredPlayer++;

	if (m_enteredPlayer == 1)
	{
		for (auto it : m_controledPlatforms)
		{
			it->bCanMove = true;
		}
	}

}

void APlatformTrigger::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr || OtherComp == OverlappedComp) return;

	AMultiplayerPuzzleCharacter* player = Cast<AMultiplayerPuzzleCharacter>(OtherActor);
	if (!player) return;

	m_enteredPlayer--;

	if (m_enteredPlayer == 0)
	for (auto it : m_controledPlatforms)
	{
		it->bCanMove = false;
	}

	UE_LOG(LogTemp, Log, TEXT("Player%d Left"), m_enteredPlayer);
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

