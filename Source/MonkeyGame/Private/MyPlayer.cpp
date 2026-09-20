


#include "MyPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Item.h"
#include "ItemSave.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "ItemCollectedWidget.h"
#include "Blueprint/UserWidget.h"

/*
* Einstellen von Sachen im Konstruktor-Aufruf, also beim erstellen des Objektes
*/
AMyPlayer::AMyPlayer()
{
	// Spring-Arm Sub-Komponente erstellen und an die Capsule Component anheften, damit sie dem Spieler folgt
	m_cameraboom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	m_cameraboom->SetupAttachment(GetCapsuleComponent()); // das Anheften an die RootComponent ist wichtig, sonst ist die Position des Arms und damit auch der Kamera unabhängig von der Position des Spielers

	// L�nge des Arms - wie lang der ist, ist relativ egal, weil die Kamera orthografisch projizieren wird, d.h. Entfernungen spielen keine Rolle.
	// nur sollte halt natürlich räumlich nix hinter dem Kamera-Arm liegen
	m_cameraboom->TargetArmLength = 512.0f;

	// relative Position und Rotation setzen, so dass Kamera auf den Spieler zeigt
	m_cameraboom->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	m_cameraboom->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	// Kamera erstellen und an den Spring Arm anheften, damit sie dem Spieler folgt
	m_playercamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	m_playercamera->SetupAttachment(m_cameraboom);

	// relative Position und Rotation zum Kamera Arm: Keine
	m_playercamera->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	m_playercamera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	m_playercamera->ProjectionMode = ECameraProjectionMode::Orthographic; // Projektionsmodus der Kamera auf orthografisch stellen um Distanz-Effekte zu vermeiden

	// Die Ortho-Width stellt dann ein "wie weit die Kamera vom Spieler weg ist", bzw eher, wie groß das Sichtfeld um den Spieler herum ist. Sollten idealerweise Potenzen von 2 sein
	m_playercamera->OrthoWidth = 4096.0f;

	// CharacterMovement Component holen und mit m_char_move_comp referenzieren
	m_char_move_comp = GetCharacterMovement();

	m_char_move_comp->JumpZVelocity = 1000.0f; // So kann man dann Werte, wie z.B. die JumpZVelocity ändern, kann man aber auch im Blueprint-Editor machen


}

/*
	* Um auf Input reagieren zu können, muss eine PlayerInputComponent definiert werden
	* @override, da sie eine vererbte Methode überschreibt
	* Diese Funktion wird automatisch von der Engine aufgerufen
	*
	* @param eine UInputComponent (bzw ein Pointer darauf)
	*/
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* player_input_component)
{
	Super::SetupPlayerInputComponent(player_input_component); // Funktion der Super-Klasse aufrufen (was auch immer die genau macht)

	// Die in der Engine definierten Actions können so an die Klasse gebunden werden
	// BindAction() hat als Parameter: Event-Name, Zustand den die zugewiesene Taste haben soll (in unserem Fall pressed), eine Referenz auf das Objekt (in diesem Fall dieses hier) 
	// und eine Referenz zur Methode, die beim Auslösen des Events ausgeführt werden soll

	player_input_component->BindAxis("MoveRight", this, &AMyPlayer::moveLeftRight);

	player_input_component->BindAction("Enhance", IE_Pressed, this, &AMyPlayer::getEnhanced); // Enhancement aktivieren, wenn Spieler Enhancement-Taste drückt

	// item wechseln beim scrollen des Mausrads
	player_input_component->BindAction("Scroll-Left", IE_Pressed, this, &AMyPlayer::leftThroughItems);
	player_input_component->BindAction("Scroll-Right", IE_Pressed, this, &AMyPlayer::rightThroughItems);

	// Hotbar bindings, entsprechendes Item an Index auswählen
	player_input_component->BindAction("Hotbar1", IE_Pressed, this, &AMyPlayer::setSelectedItem0);
	player_input_component->BindAction("Hotbar2", IE_Pressed, this, &AMyPlayer::setSelectedItem1);
	player_input_component->BindAction("Hotbar3", IE_Pressed, this, &AMyPlayer::setSelectedItem2);
}

/*
	* Diese Funktion legt fest, was zu Spielbegin mit dem Spieler geschehen soll
	* Überschreibt ebenfalls eine vererbte Methode
	*/
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay(); // zunächst Funktion der Superklasse aufrufen

	AMyPlayer::loadItems(); // wir wollen die gespeicherten Items bei jedem neuen Spiel laden
}

/*
	* Bewegt den Spieler auf der horizontalen Axe
	* (> 0 rechts, < 0 links)
	* für den Movement Input
	*/
void AMyPlayer::moveLeftRight(float axis)
{
	if (axis > 0.0)
	{
		m_is_moving_right = true;
	}
	else if (axis < 0.0)
	{
		m_is_moving_right = false;
	}

	AddMovementInput(FVector(1, 0, 0), axis); //das ist eine Methode der Pawn-Oberklasse, die nen Vektor und nen Faktor (links oder rechts) bekommt
}

/*
	* Diese Funktion wird aufgerufen, wenn der Spieler die Enhance-Taste drückt
	* Sie ruft die enhance-Methode des aktiven Items auf und startet den Count-Down
	* BlueprintCallable, da wir sie evtl auch über Blueprints aufrufen wollen
	*/
void AMyPlayer::getEnhanced()
{
	// Erstmal nur zum Debuggen, kann man eventuell verwenden, muss aber nicht
	if (m_items.Num() == 0 || m_selected_item >= m_items.Num()) { // Fall Items-Liste leer oder ausgewähltes item nicht existiert
		// So kann man Debug-Messages auf den Screen schreiben
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("No Item"))); 
	}
	else if (m_enhanced && m_selected_item != m_active_item) // Fall ein Enhancement ist bereits aktiv und das ausgewählte item ist nicht das gerade aktive
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("Already enhanced")));
	}
	else if (m_cooldown_variable == 0) // nur enhancen wenn nicht grade ein CoolDown läuft
	{

		// in der hotbar ausgewähltes item aktiv setzen
		// zwischen den beiden wird unterschieden, um Fehler zu verhindern, 
		// wenn z.B. ein anderes Item ausgewählt wird, während das enhancement eines anderen noch aktiv ist und noch beendet werden muss
		m_active_item = m_selected_item;
		
		if (m_countdown_variable <= 0 ) // falls kein Countdown läuft
		{
			// Countdown Dauer von item hohlen und setzen
			m_countdown_from_item = m_items[m_active_item]->m_duration;
			m_countdown_variable = m_countdown_from_item + 1; // +1 weil direkt dekrementiert wird und z.B. 5 Sekunden dann effektiv nur 4 währen

			// Enhancement Countdown starten
			// Dafür wird ein neuer Timer erstellt, welcher mit dem dem timer handle identifiziert wird und auf diesem Objekt in Schritten von 1 (Sekunden) countDown() aufruft
			// loop: ja, Start-Delay: 0 (darüber könnte man sonst ein Delay einstellen, nach dem der Timer erst gestartet wird)
			GetWorldTimerManager().SetTimer(m_player_countdown_handle, this, &AMyPlayer::countDown, 1.0f, true, 0.0f);
		}
		

		// aktives Item aus der Liste holen und dessen enhance() Methode ausführen
		// als Referenz muss logischerweise dieser Spieler übergeben werden
		m_items[m_active_item]->enhance(this); 

		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("Hello"))); // nur zum debuggen

		m_enhanced = true; // Status setzen
		
	}



}

/*
	* Diese Funktion zählt die Dauer des Enhancements herunter
	* indem m_enhance_countdown dekrementiert wird
	* ist m_enhance_countdown bei null angekommen, wird zurückgesetzt
	*/
void AMyPlayer::countDown()
{
	m_countdown_variable--; // herunter zählen

	// wenn bei 0 angekommen enhancement beenden und countdown zurücksetzen
	if (m_countdown_variable <= 0)
	{

		m_items[m_active_item]->endEnhancement(this);
		m_enhanced = false;

		GetWorldTimerManager().ClearTimer(m_player_countdown_handle); // Countdown Timer löschen um herunterzählen zu stoppen

		// cooldown starten
		m_cooldown_from_item = m_items[m_active_item]->m_cooldown;
		m_cooldown_variable = m_cooldown_from_item+1;
		GetWorldTimerManager().SetTimer(m_player_cooldown_handle, this, &AMyPlayer::coolDown, 1.0f, true, 0.0f);
	}
}

/*
* Diese Funktion zählt den Cooldown nach einem Enhancement herunter
* indem m_cooldown_variable bis 0 dekrementiert wird
*/
void AMyPlayer::coolDown()
{
	m_cooldown_variable--;

	if (m_cooldown_variable <= 0)
	{
		GetWorldTimerManager().ClearTimer(m_player_cooldown_handle); // Cooldown Timer löschen um herunterzählen zu stoppen
	}

}

/*
	* Diese Funktion sorgt dafür, dass ein Item eingesammelt werden kann und zur Liste hinzugefügt wird
	* Außerdem löscht entfernt sie das eingesammelte Item aus der Welt
	*
	* @param ein Objekt der Klasse Item, das eingesammelt werden soll
	*
	*/
void AMyPlayer::pickUpItem(AItem* item)
{
	// item kopieren und dann original item l�schen
	// w�rde man das original item bzw den pointer darauf ins array pushen und dann löschen, w�rde darauf zugegriffen obwohl es nicht mehr existiert - suboptimal
	AItem* the_item = NewObject<AItem>(item, item->GetClass()); // neues AItem bzw subclass davon erstellen

	m_items.Add(the_item); // zur liste hinzufügen

	// disclaimer text für das widget erstellen und einblenden
	if (m_new_item_widget_class) // nur falls ein Widget festgelegt wurde
	{
		m_new_item_widget = CreateWidget<UItemCollectedWidget>(GetWorld(), m_new_item_widget_class);
		m_new_item_widget->setText(the_item->m_name);
		m_new_item_widget->AddToViewport();

		GetWorldTimerManager().SetTimer(m_new_item_countdown, this, &AMyPlayer::itemDisclaimerCountdown, 1.0f, true, 0.0f);
	}


	item->Destroy(); // original item in der Welt zerstören

}

void AMyPlayer::itemDisclaimerCountdown()
{
	m_disclaimer_countdown--;

	if (m_disclaimer_countdown <= 0)
	{
		m_new_item_widget->RemoveFromViewport();
		GetWorldTimerManager().ClearTimer(m_new_item_countdown);
		m_disclaimer_countdown = 5;

	}
}

/*
	* Diese Funktion speichert die gesammelten Items in der Items-Liste in einem Save-Game
	*/
void AMyPlayer::saveItems()
{
	// neue Save Game Instanz erstellen
	// da CreateSaveGameObject nur ein Standard SaveGame Klassenobjekt erstellt, wir aber speziell eins der Klasse ItemSave haben wollen, muss gecasted werden
	UItemSave* item_save_instance = Cast<UItemSave>(UGameplayStatics::CreateSaveGameObject(UItemSave::StaticClass())); 

	// Item liste durchgehen (bei TArrays erhalten wir die Länge durch Num())
	// und Klassenreferenz von jedem Item in array der ItemSave Instanz speichern, weil man objekte selber nicht speichern kann (weil das nur pointer sind) 
	// Uns interessiert auch nur die Klasse und deren spezifische enhance() Funktion
	// wir wollen dann beim neu laden mithilfe der Klassenreferenz ein neues Objekt der Klasse erstellen
	for (int i = 0; i < m_items.Num(); i++)
	{

		TSubclassOf<AItem> item_class = m_items[i]->GetClass(); // Klassenreferenz vom Item holen

		item_save_instance->m_saved_items.Add(item_class); //zur liste im save game hinzufügen
	}

	// SaveGame Instanz in Slot speichern. Name des Slots ist "items", Player Index 0, da wir nur einen Spieler haben
	UGameplayStatics::SaveGameToSlot(item_save_instance, TEXT("items"), 0);

	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("Items saved"))); //debug


}

/*
	* Diese Funktion lädt die gesammelten Items aus einem Save-Game der Klasse ItemSave in die Item-Liste des Players
	*/
void AMyPlayer::loadItems()
{
	// Save Game Instanz erstellen
	UItemSave* itemLoadInstance = Cast<UItemSave>(UGameplayStatics::CreateSaveGameObject(UItemSave::StaticClass()));

	itemLoadInstance = Cast<UItemSave>(UGameplayStatics::LoadGameFromSlot("items", 0)); // SaveGame aus Slot "items" laden

	// wenn ein SaveGame existiert, 
	// dann Liste der Klassennamen durchlaufen und entsprechende Objekte dieser Klassen erstellen und zur Items-Liste hinzufügen
	if (itemLoadInstance != nullptr)
	{

		for (int i = 0; i < itemLoadInstance->m_saved_items.Num(); i++)
		{
			
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("%s"), *itemLoadInstance->m_saved_items[i])); //debug

			TSubclassOf<AItem> item_class = itemLoadInstance->m_saved_items[i]; // Klassenreferenz holen

			if (item_class)
			{
				m_items.Add(NewObject<AItem>(this, item_class)); // neues Objekt der Klasse erstellen und ins array hinzuf�gen

			}
		}
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("Items loaded"))); // debug
	}

}

/*
	* geht in der Items List einen Index zurück ("nach links")
	*/
void AMyPlayer::leftThroughItems() 
{
	if (m_selected_item > 0)
	{
		m_selected_item--;
	}
	else
	{
		m_selected_item = 2; // bei -1 muss umgesprungen werden ans ende, wir haben nur drei slots (ja ist hardgecoded, sollte aber für unsere Zwecke erstmal reichen)
	}

	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("%d"), m_selected_item)); //debug
}

/*
* geht in der Items liste einen Index vor ("nach rechts")
*/
void AMyPlayer::rightThroughItems()
{
	if (m_selected_item < 2)
	{
		m_selected_item++;
	}
	else
	{
		m_selected_item = 0;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("%d"), m_selected_item)); //debug
}

/*
	* Methode zum auswählen eines Items in der Item liste
	*
	* @param	index des Items das ausgewählt werden soll
	*/
void AMyPlayer::setSelectedItem(int index)
{
	m_selected_item = index;
}

// für die Hotbar-Key-Bindings
void AMyPlayer::setSelectedItem0()
{
	setSelectedItem(0);
}

void AMyPlayer::setSelectedItem1()
{
	setSelectedItem(1);
}

void AMyPlayer::setSelectedItem2()
{
	setSelectedItem(2);
}
