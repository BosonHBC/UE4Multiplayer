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
UCLASS()
class MULTIPLAYERPUZZLE_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetMenuInterface(IMenuInterface* i_menuInterface);

	void SetupMenu();
	void TearDown();

	void SetServerList(const TArray<FString>& i_serverNames);

	void SelectIndex(uint32 idx);

	void ToggleEnterButton(bool i_enable);
protected:

	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
		UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
		UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* Switcher;

	UPROPERTY(meta = (BindWidget))
		UEditableTextBox* AddressInput;

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
		void OnJoinClicked();

	UFUNCTION()
		void OnEnterClicked();

	UFUNCTION()
		void OnBackClick();

	UFUNCTION()
		void OnTextChanged(const FText& text);

	TSubclassOf<UUserWidget> m_serverRowBPClass;

	IMenuInterface* MenuInterface;
	FString m_IPAddress = "";
	TOptional<uint32> m_selectedIndex;
};
