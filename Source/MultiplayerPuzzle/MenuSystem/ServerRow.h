// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 *
 */
class UTextBlock;
class UButton;
class UMainMenu;
class UMainMenu;
UCLASS()
class MULTIPLAYERPUZZLE_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* ServerName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* HostName;

	void Setdata(uint16 currentPlayer, uint16 maxPlayer);
	void Setup(UMainMenu* parent, uint32 idx);

	UFUNCTION(BlueprintImplementableEvent)
		void OnButtonSelected();
	UFUNCTION(BlueprintImplementableEvent)
		void OnButtonDeSelected();

protected:

	UMainMenu* m_parent;
	uint32 m_idx;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* ServerRowButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* PlayerCount;

	UFUNCTION()
		void OnServerRowButtonClick();
};
