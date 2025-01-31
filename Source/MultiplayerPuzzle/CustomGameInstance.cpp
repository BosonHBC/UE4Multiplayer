// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameInstance.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "OnlineSubsystem.h"
#include "UObject/NameTypes.h"

const static FName SESSION_NAME = NAME_GameSession;
const static FName SESSION_NAME_KEY = TEXT("SessionName");

UCustomGameInstance::UCustomGameInstance()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/Blueprints/Menu/BP_Menu"));
	if (!MainMenuBPClass.Class) return;

	m_mainMenuClass = MainMenuBPClass.Class;
}

void UCustomGameInstance::Init()
{
	Super::Init();

	IOnlineSubsystem* oss = IOnlineSubsystem::Get();
	if (oss)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found online sub system %s"), *oss->GetSubsystemName().ToString());
		m_sessionInterface = oss->GetSessionInterface();
		if (m_sessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Found session interface"));
			m_sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCustomGameInstance::OnSessionCreated);
			m_sessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UCustomGameInstance::OnSessionDestroyed);
			m_sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UCustomGameInstance::OnFindSessionComplete);
			m_sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UCustomGameInstance::OnJoinSessionComplete);

		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Online sub system not found."));
	}
}

void UCustomGameInstance::Host(const FString& i_sessionName)
{
	if (m_sessionInterface.IsValid())
	{
		if (m_sessionInterface->GetNamedSession(SESSION_NAME) != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Session %s already exists."), *SESSION_NAME.ToString());
			m_sessionInterface->DestroySession(SESSION_NAME);
			m_recreateSessionName = i_sessionName;
		}
		else
		{
			createSession(i_sessionName);
		}
	}
}

void UCustomGameInstance::Join(const FString& address)
{

	/*UEngine* engine = GetEngine();
	if (!engine) return;

	engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining : %s"), *address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController) return;

	PlayerController->ClientTravel(address, ETravelType::TRAVEL_Absolute);*/
}


void UCustomGameInstance::Join(uint32 idx)
{
	if (!m_sessionInterface.IsValid()) return;
	if (!m_searchSettings.IsValid()) return;
	if (m_mainMenuRef)
	{
		// remove the main menu and change input style
		m_mainMenuRef->TearDown();
	}

	m_sessionInterface->JoinSession(0, SESSION_NAME, m_searchSettings->SearchResults[idx]);
}

void UCustomGameInstance::GotoMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController) return;
	// Client Travel will disconnect ourselves even we are server
	PlayerController->ClientTravel("/Game/Maps/MainMenu?listen", ETravelType::TRAVEL_Absolute);
}

void UCustomGameInstance::LoadMainMenu()
{
	m_mainMenuRef = CreateWidget<UMainMenu>(this, m_mainMenuClass);
	if (!m_mainMenuRef) return;
	m_mainMenuRef->SetupMenu();
	m_mainMenuRef->SetMenuInterface(this);
}

void UCustomGameInstance::RefreshSessionMenu()
{
	findSessions();
}

void UCustomGameInstance::StartSession()
{
	if (m_sessionInterface.IsValid())
	{
		m_sessionInterface->StartSession(SESSION_NAME);
	}
}

void UCustomGameInstance::OnSessionCreated(FName sessionName, bool bSucceed)
{
	if (!bSucceed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail to create session %s"), *sessionName.ToString());
		return;
	}
	if (m_mainMenuRef)
	{
		// remove the main menu and change input style
		m_mainMenuRef->TearDown();
		m_mainMenuRef = nullptr;
	}

	UEngine* engine = GetEngine();
	if (!engine) return;

	engine->AddOnScreenDebugMessage(0, 5, FColor::Green, TEXT("Hosting"));

	UWorld* world = GetWorld();
	if (!world) return;
	// server travel will take all clients together to the new level
	world->ServerTravel("/Game/Maps/Lobby?listen");

}

void UCustomGameInstance::OnSessionDestroyed(FName sessionName, bool bSucceed)
{
	if (!bSucceed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail to destroy session %s"), *sessionName.ToString());
		return;
	}
	else
	{
		createSession(m_recreateSessionName);
	}
}

void UCustomGameInstance::OnFindSessionComplete(bool bSuccess)
{
	UE_LOG(LogTemp, Warning, TEXT("Finding session finished."));
	if (!bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail to find sessions."));
		return;
	}
	if (m_sessionInterface.IsValid() && m_mainMenuRef)
	{
		TArray<FServerData> serverData;
		for (FOnlineSessionSearchResult& result : m_searchSettings->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found session name %s"), *result.GetSessionIdStr());
			FServerData data;
			FString nameData;
			if (result.Session.SessionSettings.Get(SESSION_NAME_KEY, nameData))
			{
				data.Name = nameData;
			}
			else
				data.Name = result.GetSessionIdStr();

			data.MaxPlayers = result.Session.SessionSettings.NumPublicConnections;
			data.CurrentPlayers = data.MaxPlayers - result.Session.NumOpenPublicConnections;

			data.HostUserName = result.Session.OwningUserName;

			serverData.Add(data);
		}
		m_mainMenuRef->SetServerList(serverData);
	}

}

void UCustomGameInstance::OnJoinSessionComplete(FName iSessionName, EOnJoinSessionCompleteResult::Type iResult)
{
	if (!m_sessionInterface.IsValid()) return;

	if (iResult != EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail to Join session: %d"), static_cast<int>(iResult));
		return;
	}
	FString contactInfo;
	if (!m_sessionInterface->GetResolvedConnectString(iSessionName, contactInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string"));
		return;
	}
	UEngine* engine = GetEngine();
	if (!engine) return;

	engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining : %s"), *contactInfo));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController) return;
	PlayerController->ClientTravel(contactInfo, ETravelType::TRAVEL_Absolute);
}

void UCustomGameInstance::createSession(const FString& i_sessionName)
{
	if (m_sessionInterface.IsValid())
	{
		int32 playerNum = 0;
		bool isLAN = false; // through steam
		FName onlineSubName = IOnlineSubsystem::Get()->GetSubsystemName();
		if (onlineSubName.IsEqual(FName("NULL")))
		{
			isLAN = true; // local network
		}

		FOnlineSessionSettings sessionSetting;
		sessionSetting.bIsLANMatch = isLAN;
		sessionSetting.NumPublicConnections = 5; // max number of connection
		sessionSetting.bShouldAdvertise = true; // make it visible to let other to find
		sessionSetting.bUsesPresence = true;
		sessionSetting.bAllowJoinInProgress = false;
		sessionSetting.Set(SESSION_NAME_KEY, i_sessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);


		m_sessionInterface->CreateSession(playerNum, SESSION_NAME, sessionSetting);
	}
}

void UCustomGameInstance::findSessions()
{
	// make shareable will make new operator work in unreal

	m_searchSettings = MakeShareable(new FOnlineSessionSearch());
	if (m_searchSettings.IsValid())
	{
		m_searchSettings->bIsLanQuery = false; // through network
		// allow search presence
		m_searchSettings->MaxSearchResults = 100; // Allow more search result so that the result 
		m_searchSettings->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Start to find sessions"));
		// make search setting are not null and convert shared pointer to shared reference
		m_sessionInterface->FindSessions(0, m_searchSettings.ToSharedRef());
	}
}