

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyPlayer.generated.h"

/**
 * Die Player-Klasse als Subklasse eines Paper2DCharacters
 */
UCLASS(Blueprintable) // muss Blueprintable sein, damit wir sie einfach als Blueprint-Subklasse in das Projekt einfügen können
class MONKEYGAME_API AMyPlayer : public APaperCharacter
{
	GENERATED_BODY()

public:

	AMyPlayer(); // Konstruktor

	// Die Flags UPROPERTY und UFUNCTION etc müssen davor stehen, wenn die Property oder die Funktion von Blueprint-Subklassen erkannt werden sollen
	// EditAnywhere -> Wert kann überall, also auch im Blueprint Editor bearbeitet werden
	// BlueprintReadWrite -> Blueprint-Funktionen können diesen Wert lesen und verändern
	// BlueprintReadOnly -> kann von Blueprints nur gelesen, aber nicht verändert werden
	// Category -> Die Kategorie unter der die Variable im Blueprint Editor gelistet werden soll


	// wird während Count Down runtergezählt
	UPROPERTY(BlueprintReadOnly)
		int m_countdown_variable = 0; 

	// wird während Cool Down runtergezählt, ist aber zunächst 0
	UPROPERTY(BlueprintReadOnly)
		int m_cooldown_variable = 0;

	//wie lange das Enhancement andauern soll
	// wird vom aktiven item geholt und ist nur für einfachere Kommunikation mit Blueprints da 
	// (damit man nicht immer erst das aktive item raussuchen muss)
	UPROPERTY(BlueprintReadOnly)
		int m_countdown_from_item = 0;

	// Cooldown, wie lange man nach einem Enhancement warten muss, um ein neues verwenden zu können 
	// wird vom aktiven item geholt und ist nur für einfachere Kommunikation mit Blueprints da
	UPROPERTY(BlueprintReadOnly)
		int m_cooldown_from_item = 0; 

	// Anzahl Leben
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Lives")
		int m_lives = 3; 

	// der Index für das derzeit aktive Item (in der Item-Liste)
	UPROPERTY(BlueprintReadWrite, Category = "Player Items")
		int m_active_item = 0; 

	// der Index für das derzeit ausgewählte Item in der Hotbar (in der Item-Liste)
	UPROPERTY(BlueprintReadWrite, Category = "Player Items")
		int m_selected_item = 0; 

	// Boolean, der Aussage darüber gibt, ob der Spieler gerade enhanced wird. 
	// Kann z.B. benutzt werden um zu verhindern, dass zwei Items kurz hintereinander benutzt werden oder ein enhancement verlängert wird
	// oder falls Blueprints die info brauchen
	UPROPERTY(BlueprintReadOnly)
		bool m_enhanced = false; 

	// Boolean, der anzeigt ob sich der Spieler nach rechts bewegt
	// wird benötigt um animation zu steuern oder von items, falls sie auf die bewegung zugreifen
	UPROPERTY(BlueprintReadWrite)
		bool m_is_moving_right = true;

	// Liste von Items in Besitz des Spielers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Items")
		TArray<class AItem*> m_items; 

	// Kamera-Komponente, die auf den Spieler gerichtet wird
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* m_playercamera; 

	// Arm für die Kamera
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpringArmComponent* m_cameraboom; 

	// Die Character Movement Component, in der Variablen für die Bewegung des Spielers eingestellt werden können
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCharacterMovementComponent* m_char_move_comp; 

	// Widget-Klasse für ein Widgget, das angezeigt wird, wenn ein neues Item eingesammelt wird
	// Das ist nur hier, weil es so am einfachsten ist das beim Einsammeln aufzurufen
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UItemCollectedWidget> m_new_item_widget_class;

	// Das letztendliche Widget wird hier gespeichert
	UPROPERTY()
		class UItemCollectedWidget* m_new_item_widget;

	// Timer brauchen einen Timer Handle zur Identifikation
	FTimerHandle m_player_countdown_handle; // Handle für den Countdown Timer
	FTimerHandle m_player_cooldown_handle;  // Handle für den Cooldown Timer
	FTimerHandle m_new_item_countdown; // Handle für den Timer, der steuert wie lange das new item widget angezeigt werden soll

	

	/*
	* Um auf Input reagieren zu können, muss eine PlayerInputComponent definiert werden
	* @override, da sie eine vererbte Methode überschreibt
	* Diese Funktion wird automatisch von der Engine aufgerufen
	*
	* @param eine UInputComponent (bzw ein Pointer darauf)
	*/
	virtual void SetupPlayerInputComponent(UInputComponent* player_input_component) override;

	/*
	* Diese Funktion legt fest, was zu Spielbegin mit dem Spieler geschehen soll
	* Überschreibt ebenfalls eine vererbte Methode
	*/
	virtual void BeginPlay() override;

	/*
	* Diese Funktion wird aufgerufen, wenn der Spieler die Enhance-Taste drückt
	* Sie ruft die enhance-Methode des aktiven Items auf und startet den Count-Down
	* BlueprintCallable, da wir sie evtl auch über Blueprints aufrufen wollen
	*/
	UFUNCTION(BlueprintCallable)
		void getEnhanced();

	/*
	* Diese Funktion speichert die gesammelten Items in der Items-Liste in einem Save-Game
	*/
	UFUNCTION(BlueprintCallable)
		void saveItems();

	/*
	* Diese Funktion lädt die gesammelten Items aus einem Save-Game der Klasse ItemSave in die Item-Liste des Players
	*/
	UFUNCTION(BlueprintCallable)
		void loadItems();

	/*
	* Methode zum auswählen eines Items in der Item liste
	* 
	* @param	index des Items das ausgewählt werden soll
	*/
	UFUNCTION(BlueprintCallable)
		void setSelectedItem(int index);

	/*
	* Diese Funktion sorgt dafür, dass ein Item eingesammelt werden kann und zur Liste hinzugefügt wird
	* Außerdem löscht entfernt sie das eingesammelte Item aus der Welt
	* muss public sein, weil Item-Klasse darauf zugreift
	*
	* @param ein Objekt der Klasse Item, das eingesammelt werden soll
	*
	*/
	void pickUpItem(class AItem* item); // das "class" ist eine Forward-Declaration um das Header-File nicht einbinden zu müssen - im .cpp file muss es dann aber eingebunden werden

protected:

	int m_disclaimer_countdown = 5; // wie lange der disclaimer, das ein neues Item eingesammelt wurde angezeigt werden soll

	/*
	* Bewegt den Spieler auf der horizontalen Axe
	* (> 0 rechts, < 0 links)
	* für den Movement Input
	*/
	void moveLeftRight(float axis);

	/*
	* Diese Funktion zählt die Dauer des Enhancements herunter
	* indem m_countdown_variable dekrementiert wird
	* ist m_countdown_variable bei null angekommen, wird zurückgesetzt
	*/
	void countDown();

	/*
	* Diese Funktion zählt den Cooldown nach einem Enhancement herunter
	* indem m_cooldown_variable bis 0 dekrementiert wird
	*/
	void coolDown();

	/*
	* Zählt den Item Disclaimer countdown herunter
	*/
	void itemDisclaimerCountdown();

	/*
	* geht in der Items List einen Index zurück ("nach links")
	*/
	void leftThroughItems();

	/*
	* geht in der Items liste einen Index vor ("nach rechts")
	*/
	void rightThroughItems();

	// die sind für die Hotbar, um auf die Tasten 1,2 und 3 zu reagieren
	// etwas unelegant, aber wusste nicht wie man es besser lösen kann
	void setSelectedItem0();
	void setSelectedItem1();
	void setSelectedItem2();

	










};
