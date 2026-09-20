// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreCollectibleItem.h"
#include "MyPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "ScoreTest.h"
#include "UObject/UObjectGlobals.h"


/*
	* Override der onOverlap-Funktion
	* Das Item soll nicht eingesammelt werden, sondern einfach nur den Score-Multiplikator erhöhen und sich dann selber zerstören
	*/
void AScoreCollectibleItem::onOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// wenn Overlap mit Player, dann Score des Levels finden und Multiplikator erhöhen
	if (OtherActor->IsA(AMyPlayer::StaticClass()))
	{
		// Score Actor im aktuellem Level finden
		AScoreTest* level_score = Cast<AScoreTest>(UGameplayStatics::GetActorOfClass(GetWorld(), AScoreTest::StaticClass()));	

		level_score->setMultiplier(level_score->m_multiplier + m_multiplier_increase_step); // multiplikator erhöhen
		level_score->m_collected++; // anzahl der gesammelten collectibles erhöhen

		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("Collectible collected"))); //debug

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_collect_sound, GetActorLocation(), 1.0f, 1.0f, 0); // collect sound spielen

		this->Destroy();
	}
}

