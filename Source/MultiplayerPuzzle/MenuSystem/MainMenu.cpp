// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/PlayerController.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "UObject/ConstructorHelpers.h"
#include "ServerRow.h"

#define FINDING FText::FromString(TEXT("Finding servers..."))
#define NOT_FOUND FText::FromString(TEXT("Server not found."))

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/Blueprints/Menu/WBP_ServerRow"));
	if (!ServerRowBPClass.Class) return;

	m_serverRowBPClass = ServerRowBPClass.Class;
}

void UMainMenu::SetMenuInterface(IMenuInterface* i_menuInterface)
{
	MenuInterface = i_menuInterface;
}

void UMainMenu::SetupMenu()
{
	AddToViewport();
	bIsFocusable = true;
	// Set input mode for menu UI
	APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	if (!PlayerController) return;
	FInputModeUIOnly uiOnlyMode;
	uiOnlyMode.SetWidgetToFocus(TakeWidget());
	uiOnlyMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(uiOnlyMode);
	PlayerController->bShowMouseCursor = true;
}

void UMainMenu::TearDown()
{
	RemoveFromViewport();
	APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	if (!PlayerController) return;
	FInputModeGameOnly uiInputMode;
	PlayerController->SetInputMode(uiInputMode);
	PlayerController->bShowMouseCursor = false;
}

void UMainMenu::SetServerList(const TArray<FString>& i_serverNames)
{
	UWorld* world = GetWorld();
	if (!world) return;

	SessionScroll->ClearChildren();
	uint32 idx = 0;
	for (auto& serverName : i_serverNames)
	{
		auto serverRow = CreateWidget<UServerRow>(world, m_serverRowBPClass);
		if (!serverRow) return;
		serverRow->ServerName->SetText(FText::FromString(serverName));
		serverRow->Setup(this, idx);
		++idx;
		SessionScroll->AddChild(serverRow);
	}
	if (i_serverNames.Num() > 0)
	{
		TextInfo->SetVisibility(ESlateVisibility::Hidden);
	}
	else
		TextInfo->SetText(NOT_FOUND);
}

void UMainMenu::SelectIndex(uint32 idx)
{
	m_selectedIndex = idx;
}

void UMainMenu::ToggleEnterButton(bool i_enable)
{
	EnterButton_1->SetIsEnabled(i_enable);
}

bool UMainMenu::Initialize()
{
	bool result = Super::Initialize();
	if (!result) return result;

	if (!HostButton || !JoinButton) return false;

	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OnHostClicked);
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinClicked);

	EnterButton_1->OnClicked.AddDynamic(this, &UMainMenu::OnEnterClicked);
	BackButton_1->OnClicked.AddDynamic(this, &UMainMenu::OnBackClick);

	AddressInput->OnTextChanged.AddDynamic(this, &UMainMenu::OnTextChanged);

	return true;
}

void UMainMenu::OnHostClicked()
{
	// Host server
	if (MenuInterface)
	{
		MenuInterface->Host();
	}

}

void UMainMenu::OnJoinClicked()
{
	Switcher->SetActiveWidgetIndex(1);

	TextInfo->SetVisibility(ESlateVisibility::Visible);
	TextInfo->SetText(FINDING);
	ToggleEnterButton(false);
	MenuInterface->RefreshSessionMenu();
}

void UMainMenu::OnEnterClicked()
{
	if (m_selectedIndex.IsSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index set: %d"), m_selectedIndex.GetValue());
		if (MenuInterface)
		{
			MenuInterface->Join(m_selectedIndex.GetValue());
		}
	}

}

void UMainMenu::OnBackClick()
{
	SessionScroll->ClearChildren();
	Switcher->SetActiveWidgetIndex(0);
}

void UMainMenu::OnTextChanged(const FText& text)
{
	m_IPAddress = text.ToString();
}
