// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"
#include "MainMenu.h"
#include "Components/Button.h"

void UServerRow::Setup(UMainMenu* parent, uint32 idx)
{
	m_parent = parent;
	m_idx = idx;
	ServerRowButton->OnClicked.AddDynamic(this, &UServerRow::OnServerRowButtonClick);
}

void UServerRow::OnServerRowButtonClick()
{
	if (m_parent)
	{
		m_parent->SelectIndex(m_idx);
		m_parent->ToggleEnterButton(true);
	}

}
