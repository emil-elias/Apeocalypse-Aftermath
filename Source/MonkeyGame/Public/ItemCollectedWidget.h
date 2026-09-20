// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ItemCollectedWidget.generated.h"

/**
 * Dieses Widget wird erstellt, wenn ein neues Item eingesammelt wird
 * es enthält einen Text, in den der Item name eingefügt werden kann
 * Existiert nur als C++ Klasse um einfach vom Code darauf zugreifen zu können
 * Anpassungen und Looks werden im Blueprint gemacht
 */
UCLASS(Blueprintable)
class MONKEYGAME_API UItemCollectedWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// der Text Block
	// dieser Tag sorgt dafür, dass zur Laufzeit im Blueprint nach einem Textblock gesucht wird, der genau diesen Namen hat,
	// der wird dann automatisch verknüpft
	// wichtig ist dementsprechend, dass der textblock im Blueprint dann auch "m_text_block" heißt und "is variable" aktiviert ist
	UPROPERTY(meta = (BindWidget)) 
	UTextBlock* m_text_block;

	/*
	* Diese Funktion fügt den Item Namen in den Text ein
	* 
	* @param	der Item name als String
	*/
	void setText(FString item_name);
	
};
