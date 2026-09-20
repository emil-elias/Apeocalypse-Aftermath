

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "DashItem.generated.h"

/**
 * Dash Item (Leafblower)
 * gibt dem Spieler einen horizontalen boost in blickrichtung
 */
UCLASS(Blueprintable)
class MONKEYGAME_API ADashItem : public AItem
{
	GENERATED_BODY()

public:

	// faktor, wie stark der dash sein soll
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_launch_factor = 10000.0;

	/*
	* override der AItem Parent-Klasse
	* Diese Funktion definiert das Enhancement des Spielers, wenn er das Item benutzt
	* 
	* gibt dem Spieler einen horizontalen boost in blickrichtung
	*
	* @param player		ein MyPlayer, der beinflusst wird
	*/
	virtual void enhance(class AMyPlayer* player) override;

	/*
	* override der AItem Parent-Klasse
	* Diese Funktion beendet das Enhancement, indem sie die Manipulation wieder zurücksetzt
	*
	* @param player		ein MyPlayer
	*/
	virtual void endEnhancement(class AMyPlayer* player) override;

};
