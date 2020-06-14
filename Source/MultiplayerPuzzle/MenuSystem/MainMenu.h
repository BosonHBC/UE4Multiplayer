// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 *
 */
class UButton;
class UTextBlock;
class UWidgetSwitcher;
class UEditableTextBox;
class UScrollBox;

USTRUCT(BlueprintType)
struct FServerData
{
	GENERATED_BODY()

		FString Name;
	FString HostUserName;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;

};

UCLASS()
class MULTIPLAYERPUZZLE_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetMenuInterface(IMenuInterface* i_menuInterface);

	void SetupMenu();
	void TearDown();

	void SetServerList(const TArray<FServerData>& i_serverNames);
	
	void SelectIndex(uint32 idx);

	void ToggleEnterButton(bool i_enable);
protected:

	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
		UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
		UButton* HostComfirmButton;
	UPROPERTY(meta = (BindWidget))
		UButton* HostBackButton;
	UPROPERTY(meta = (BindWidget))
		UEditableTextBox* SessionNameTextBox;

	UPROPERTY(meta = (BindWidget))
		UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* Switcher;

	UPROPERTY(meta = (BindWidget))
		UButton* EnterButton_1;

	UPROPERTY(meta = (BindWidget))
		UButton* BackButton_1;
	
	UPROPERTY(meta = (BindWidget))
		UTextBlock* TextInfo;

	UPROPERTY(meta = (BindWidget))
		UScrollBox* SessionScroll;

	UFUNCTION()
		void OnHostClicked();
	UFUNCTION()
	void OnHostComfirmButtonClicked();

	UFUNCTION()
		void OnJoinClicked();

	UFUNCTION()
		void OnEnterClicked();

	UFUNCTION()
		void OnBackClick();

	UFUNCTION()
		void OnTextChanged(const FText& text);

	void UpdateScrollRowChildren();

	TSubclassOf<UUserWidget> m_serverRowBPClass;

	IMenuInterface* MenuInterface;
	FString m_hostSessionName = "";
	TOptional<uint32> m_selectedIndex;
};
