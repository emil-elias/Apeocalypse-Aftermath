// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "JumpItem.generated.h"

/**
 * Item, dass die Sprungkraft erhöht
 */
UCLASS()
class MONKEYGAME_API AJumpItem : public AItem
{
	GENERATED_BODY()

public:

	/*
	* Diese Funktion definiert das Enhancement des Spielers, wenn er das Item benutzt
	* In diesem Fall wird die Jump Height bzw Jump Velocity erhöht
	*
	* @param player		ein MyPlayer, der beeinflusst wird
	*/
	virtual void enhance(class AMyPlayer* player) override;

	/*
	* Diese Funktion beendet das Enhancement, indem sie die Manipulation wieder zurücksetzt
	*
	* @param player		ein MyPlayer
	*/
	virtual void endEnhancement(class AMyPlayer* player) override;

protected:

	// gibt aussage darüber ob das enhancement gerade aktiv ist
	bool m_currently_active = false;

	// das hier kann man benutzen um neue Action Bindings für den Spieler zu definieren
	// z.B. könnte man immer wenn w/spache gedrückt wird so getEnhanced() statt Jump() beim beim spieler aufrufen wenn dieses item aktiv ist
	FInputActionBinding m_action_binding;

	
};
