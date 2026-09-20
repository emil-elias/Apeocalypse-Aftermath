// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemCollectedWidget.h"

/*
	* Diese Funktion fügt den Item Namen in den Text ein
	*
	* @param	der Item name als String
	*/
void UItemCollectedWidget::setText(FString item_name)
{
	if (m_text_block) // wenn m_text_block existiert
	{

		// item name einfügen und text setzen
		FString text = "You collected " + item_name +"! Press E to try out";
		m_text_block->SetText(FText::FromString(text));

	}

	
}