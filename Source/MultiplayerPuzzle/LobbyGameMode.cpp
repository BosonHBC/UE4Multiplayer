// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "CustomGameInstance.h"

ALobbyGameMode::ALobbyGameMode() : AMultiplayerPuzzleGameMode()
{

}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++m_numOfPlayers;
	APawn* controlledPawn = NewPlayer->GetPawn();
	UE_LOG(LogTemp, Warning, TEXT("Player [%s], login, current player count: %d."),  *(controlledPawn ? controlledPawn->GetName() : NewPlayer->GetName()), m_numOfPlayers);

	if (m_numOfPlayers >= static_cast<uint32>(PlayersReadyToStartGame))
	{
		GetWorld()->GetTimerManager().SetTimer(m_startTimeHandle, this, &ALobbyGameMode::travelToGame, WaitTime, false);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--m_numOfPlayers;

	APawn* controlledPawn = Exiting->GetPawn();
	UE_LOG(LogTemp, Warning, TEXT("Player [%s], logout, current player count: %d."),  *(controlledPawn ? controlledPawn->GetName() : Exiting->GetName()), m_numOfPlayers);

}

void ALobbyGameMode::travelToGame()
{
	UCustomGameInstance* GI = GetGameInstance<UCustomGameInstance>();
	if (!GI) return;

	GI->StartSession();

	UWorld* world = GetWorld();
	if (!world) return;
	// server travel will take all clients together to the new level
	bUseSeamlessTravel = true;
	world->ServerTravel("/Game/Maps/ThirdPersonExampleMap?listen");
}
