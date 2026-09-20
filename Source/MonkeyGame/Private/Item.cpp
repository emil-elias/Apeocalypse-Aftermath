


#include "Item.h"
#include "MyPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Components/BrushComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


#define PrintString(String) GEngine->AddOnScreenDebugMessage(-1,1.0f, FColor::White,String)

AItem::AItem()
{
	m_box_comp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp")); // Box Component für die Hitbox erstellen
	m_box_comp->SetCollisionProfileName(TEXT("Trigger")); // Kollisionsprofil Trigger: man soll durchlaufen können, aber es soll ein overlap-event ausgelöst werden
	m_box_comp->SetupAttachment(RootComponent); // an Root Component anheften

	m_box_comp->OnComponentBeginOverlap.AddDynamic(this, &AItem::onOverlapBegin); // Wenn Overlap passiert, dann onOverlapBegin() aufrufen
}

/*
* Diese Funktion definiert das Enhancement des Spielers, wenn er das Item benutzt
*
* @param player		ein MyPlayer, der beeinflusst werden soll
*/
void AItem::enhance(AMyPlayer* player)
{

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_use_sound, GetActorLocation(), 1.0f, 1.0f, 0); // use sound an der actor location abspielen

	// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("%s"), *m_name)); // Debug: Name ausgeben

}

/*
	* Diese Funktion beendet das Enhancement, indem sie die Manipulation wieder zurücksetzt
	*
	* @param player		ein MyPlayer
	*/
void AItem::endEnhancement(AMyPlayer* player)
{
	// wird in Subklassen implementiert
}

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
void AItem::onOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// testen ob es sich beim anderen Actor um einen MyPlayer handelt
	// wenn ja pickUpItem() vom Player aufrufen
	if (OtherActor->IsA(AMyPlayer::StaticClass()))
	{

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_collect_sound, GetActorLocation(), 1.0f, 1.0f, 0); // collect sound an der actor location abspielen

		AMyPlayer* player = Cast<AMyPlayer>(OtherActor); // OtherActor in MyPlayer casten um auf Methoden zuzugreifen
		player->pickUpItem(this); // dieses Item an pickUpItem() vom Player übergeben, welche das Item in dessen Itemliste hinzufügt

		
	}

}
