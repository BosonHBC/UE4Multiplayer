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
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ServerName;

	void Setup(UMainMenu* parent, uint32 idx);

protected:

	UMainMenu* m_parent;
	uint32 m_idx;

	UPROPERTY(meta = (BindWidget))
		UButton* ServerRowButton;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* PlayerCount;

	UFUNCTION()
		void OnServerRowButtonClick();
};
