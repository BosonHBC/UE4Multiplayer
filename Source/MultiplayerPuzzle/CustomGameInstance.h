// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "CustomGameInstance.generated.h"

/**
 *
 */
class UMainMenu;
UCLASS()
class MULTIPLAYERPUZZLE_API UCustomGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UCustomGameInstance();

	void Init() override;

	UFUNCTION(Exec)
		void Host(const FString& i_sessionName) override;

	UFUNCTION(Exec)
		void Join(const FString& address) override;

	void Join(uint32 idx) override;

	UFUNCTION(Exec)
		void GotoMainMenu() override;

	UFUNCTION(BlueprintCallable)
		void LoadMainMenu();

	void RefreshSessionMenu() override;

protected:
	TSubclassOf<UUserWidget> m_mainMenuClass;
	UMainMenu* m_mainMenuRef;

	IOnlineSessionPtr m_sessionInterface;
	TSharedPtr<FOnlineSessionSearch> m_searchSettings;


	UFUNCTION()
		void OnSessionCreated(FName sessionName, bool bSucceed);	
	UFUNCTION()
		void OnSessionDestroyed(FName sessionName, bool bSucceed);
	UFUNCTION()
		void OnFindSessionComplete(bool bSuccess);

		void OnJoinSessionComplete(FName iSessionName, EOnJoinSessionCompleteResult::Type iResult);
	void createSession(const FString& i_sessionName);

	FString m_recreateSessionName = "";
	void findSessions();
};
