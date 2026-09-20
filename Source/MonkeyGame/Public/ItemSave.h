

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ItemSave.generated.h"

/**
 * SaveGame Klasse zum Speichern der Klassennamen der Items, die der Spieler schon gesammelt hat
 * Objekte ansich lassen sich nicht speichern, da man nur Pointer in Arrays packen kann,
 * deswegen müssen wir die Klassenreferenzen speichern und dann von diesen Klassen wieder neue Objekte erstellen
 * (das macht die MyPlayer Klasse)
 */
UCLASS()
class MONKEYGAME_API UItemSave : public USaveGame
{
	GENERATED_BODY()

public:

	UItemSave();

	// Array, das die Klassennamen speichert
	// TSubclassOf<class> speichert quasi Klassennamen/Referenzen von Unterklassen einer Klasse
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<class AItem>> m_saved_items;
	
};
