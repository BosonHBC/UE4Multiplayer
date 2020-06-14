// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerPuzzleGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPUZZLE_API ALobbyGameMode : public AMultiplayerPuzzleGameMode
{
	GENERATED_BODY()
	
public:
	ALobbyGameMode();

	void PostLogin(APlayerController* NewPlayer) override;
	void Logout(AController* Exiting) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sessions")
		int PlayersReadyToStartGame = 2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sessions")
		float WaitTime = 5.f;
protected:
	uint32 m_numOfPlayers = 0;
	FTimerHandle m_startTimeHandle;

	void travelToGame();
};
