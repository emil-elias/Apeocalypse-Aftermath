

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "Item.generated.h"


/**
 * Item Klasse mit spezifischer, überschreibbarer enhance() Funktion, die den Spieler beinflussen kann
 * Subklasse des PaperFlipbookActors, da dieser schon eine Flipbook-Komponente enthält, falls wir das Item animieren wollen
 */
UCLASS(Blueprintable, BlueprintType)
class MONKEYGAME_API AItem : public APaperFlipbookActor
{
	GENERATED_BODY()

public:

	AItem();

	// Name
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString m_name = "Item Superclass"; 

	// Beschreibung
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString m_description = "Create a subclass to define an item specific enhancement"; 

	// Wie lange das enhancement anhalten/verfügbar sein soll
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_duration = 5;

	// Cooldown, wie lange man nach benutzung warten muss um dieses oder ein anderes Item wieder zu verwenden
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_cooldown = 3;

	float m_value_to_enhance = 0; // falls ein Variablen-Wert im Spieler für kurze Zeit geändert werden soll, wird er hier gespeichert um ihn danach wieder zurücksetzen zu können

	// eine Box-Component für die Hitbox
	UPROPERTY(EditAnywhere)
		class UBoxComponent* m_box_comp; 

	// Bild für die Hotbar
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* m_hotbar_image; 

	// Sound Cue, der beim Einsammeln des Items abgespielt wird
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* m_collect_sound;

	// Sound Cue, der beim Benutzen des Items abgespielt wird
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* m_use_sound;

	

	/*
	* Diese Funktion definiert, was bei einem Overlap passieren soll
	* Wird bei Overlap-Event aufgerufen
	* Die Parameter werden von der Engine automatisch übergeben
	* 
	* @param OverlappedComp		die Komponente in der die Überlappung passiert
	* @param OtherActor			der Actor mit dem kollidiert wird
	* @param OtherComp			die Komponente des anderen Actors
	* bei den anderen Paramateren kann ich nicht genau sagen, wofür sie wichtig sind tbh
	* 
	* Wenn OtherActor ein MyPlayer ist, dann soll dieser dieses Item in seine Itemsliste aufnehmen
	* (zumindest in der Regel)
	*/
	UFUNCTION()
		virtual void onOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/*
	* Diese Funktion definiert das Enhancement des Spielers, wenn er das Item benutzt
	* 
	* @param player		ein MyPlayer, der beeinflusst werden soll
	*/
	UFUNCTION(BlueprintCallable)
		virtual void enhance(class AMyPlayer* player);

	/*
	* Diese Funktion beendet das Enhancement, indem sie die Manipulation wieder zurücksetzt
	* 
	* @param player		ein MyPlayer
	*/
	UFUNCTION(BlueprintCallable)
		virtual void endEnhancement(class AMyPlayer* player);


	
};
