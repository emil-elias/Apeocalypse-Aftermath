// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Kismet/GameplayStatics.h"
#include "ScoreCollectibleItem.generated.h"


/**
 * Besondere Item-Klasse, die beim Einsammeln den Score-Multiplier erhöht (Münzen)
 * hat kein enhancement, dafür wird onOverlapBegin überschrieben
 */
UCLASS(Blueprintable)
class MONKEYGAME_API AScoreCollectibleItem : public AItem
{
	GENERATED_BODY()

public:

	// um wie viel der Multiplikator bei jedem einsammeln erhöht werden soll
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_multiplier_increase_step = 0.5;

	/*
	* Override der onOverlap-Funktion
	* Das Item soll nicht eingesammelt werden, sondern einfach nur den Score-Multiplikator erhöhen und sich dann selber zerstören
	*/
	virtual void onOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;



};
