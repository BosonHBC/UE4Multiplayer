// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MULTIPLAYERPUZZLE_API IMenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void Host(const FString& i_sessionName) = 0;
	virtual void Join(const FString& address) = 0;
	virtual void Join(uint32 idx) = 0;
	virtual void GotoMainMenu() = 0;
	virtual void RefreshSessionMenu() = 0;
};
