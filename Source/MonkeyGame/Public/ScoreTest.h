

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreTest.generated.h"

UCLASS(Blueprintable)
class MONKEYGAME_API AScoreTest : public AActor
{
	GENERATED_BODY()

public:
	AScoreTest();

	// sozusagen die maximale Zeit, die man für ein level brauchen sollte (in sekunden)
	// und an dem sich der score orientiert. desto weiter man davon weg ist, desto höher der score (ohne multiplier)
	// sollte an die länge des levels angepasst werden
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_score_threshold = 300;

	// Variable zum Hochzählen der Sekunden
	// Soll vom HUD Widget gelesen werden
	UPROPERTY(BlueprintReadOnly)
		int m_seconds = 0;

	// Variable zum Hochzählen der Minuten
	// Soll vom HUD Widget gelesen werden
	UPROPERTY(BlueprintReadOnly)
		int m_minutes = 0;

	UPROPERTY(BlueprintReadOnly)
		// Gesamtzeit in Sekunden
		int m_total_seconds = 0;

	// gesammelte Punkte
	UPROPERTY(BlueprintReadOnly)
		float m_points = 0;

	// Multiplikator für den Score
	UPROPERTY(BlueprintReadOnly)
		float m_multiplier = 1;

	// Wie viele Collectibles gefunden wurden
	UPROPERTY(BlueprintReadOnly)
		int m_collected = 0;


	// Timer Handle für den Score
	FTimerHandle m_score_timerhandle;

	// Tick wird jeden Frame aufgerufen
	virtual void Tick(float DeltaTime) override;

	/*
	* Diese Methode berechnet den finalen Score nach Ende des Spiels
	*/
	UFUNCTION(BlueprintCallable)
		float calculateFinalScore();

	/*
	* Methode um den Multiplikator zu setzen / zu verändern
	*
	* @param multiplier		neuer Multiplikationsfaktor, sollte natürlich nicht 0 und eigentlich auch nicht kleiner als 1 sein
	*/
	UFUNCTION(BlueprintCallable)
		void setMultiplier(float multiplier);

	/*
	* Pausiert den Timer (m_score_timerhandle) und damit auch die weitere Score berechnung
	*/
	UFUNCTION(BlueprintCallable)
		void pauseTimer();

	/*
	* Lässt den Timer (m_score_timerhandle) weiterlaufen
	*/
	UFUNCTION(BlueprintCallable)
		void unPauseTimer();

protected:

	/*
	* Diese Funktion legt fest, was zu Spielbegin mit dem Score geschehen soll
	* Überschreibt eine vererbte Methode
	*/
	virtual void BeginPlay() override;

	/*
	* Zählt Zeit hoch
	* Sowohl Gesamtsekunden, als auch Minuten und Sekunden (mit Umbrüchen nach 60) einzeln für die Darstellung
	*/
	void countUp();

	/*
	* Rechnet die Points basierend auf der Zeit aus, abhängig davon wie weit der Spieler zeitlich vom Threshold entfernt ist
	*/
	void calculatePoints();


};
