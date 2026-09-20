


#include "ScoreTest.h"

#define printString(string) GEngine->AddOnScreenDebugMessage(-1,1.0f, FColor::White,string);


AScoreTest::AScoreTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetActorHiddenInGame(true); // Score muss als Actor in der Welt platziert werden, soll aber nicht zu sehen sein
	SetActorEnableCollision(false); // Außerdem soll es keine Kollision geben

}


/*
* Diese Funktion legt fest, was zu Spielbegin mit dem Score geschehen soll
* Überschreibt eine vererbte Methode
*/
void AScoreTest::BeginPlay()
{
	Super::BeginPlay();

	// Timer erstellen, der jede Sekunde countUp() aufruft
	GetWorldTimerManager().SetTimer(m_score_timerhandle, this, &AScoreTest::countUp, 1.0f, true, 0.0f); 
	
	
}

// wird jeden Frame aufgerufen
void AScoreTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*
	* Zählt Zeit hoch
	* Sowohl Gesamtsekunden, als auch Minuten und Sekunden (mit Umbrüchen nach 60) einzeln für die Darstellung
	*/
void AScoreTest::countUp()
{
	m_total_seconds++;

	// Sekunden nur bis 59 hochzählen,
	// dann zurücksetzen und minuten inkrementieren
	if (m_seconds < 59)
	{
		m_seconds++;
	}
	else
	{
		m_seconds = 0;
		m_minutes++;
	}

	calculatePoints(); // Punkte für jede neue Sekunde ausrechnen

	//printString(FString::Printf(TEXT("%02d : %02d"), m_minutes, m_seconds)); //Debug-Ausgabe

}

/*
	* Diese Methode berechnet den finalen Score nach Ende des Spiels
	* 
	*/
float AScoreTest::calculateFinalScore()
{
	return m_points; //gut, ist jetzt nicht sonderlich kompliziert geworden. Könnte man theoretisch noch ausbauen
}

/*
	* Methode um den Multiplikator zu setzen / zu verändern
	*
	* @param multiplier		neuer Multiplikationsfaktor, alles kleiner als 1 wird automatisch auf 1 gesetzt
	*/
void AScoreTest::setMultiplier(float multiplier) {

	if (multiplier < 1) multiplier = 1;

	m_multiplier = multiplier;
}

void AScoreTest::calculatePoints()
{
	// solange der spieler unter dem Threshold ist, ist sein score der Abstand zu diesem Threshold * Multiplikator
	// ansonsten einfach nur der Multiplikator
	if (m_total_seconds < m_score_threshold)
	{
		m_points = (m_score_threshold - m_total_seconds) * m_multiplier;

	}
	else
	{
		m_points = m_multiplier;
	}
}

void AScoreTest::pauseTimer()
{
	if (!GetWorldTimerManager().TimerExists(m_score_timerhandle)) return; // falls kein Timer existiert (nur zur Sicherheit)

	GetWorldTimerManager().PauseTimer(m_score_timerhandle);
	
}

void AScoreTest::unPauseTimer()
{
	if (!GetWorldTimerManager().TimerExists(m_score_timerhandle)) return;

	GetWorldTimerManager().UnPauseTimer(m_score_timerhandle);

}


