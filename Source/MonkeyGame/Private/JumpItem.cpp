// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpItem.h"
#include "MyPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

/*
	* Diese Funktion definiert das Enhancement des Spielers, wenn er das Item benutzt
	* In diesem Fall wird die Jump Height bzw Jump Velocity erhöht
	*
	* @param player		ein MyPlayer, der beeinflusst wird
	*/
void AJumpItem::enhance(AMyPlayer* player)
{

	// alternative option
	// enhance setzt die jump velocity generell, d.h. der spieler springt auch immer höher wenn man normal w/space drückt,
	// nicht nur wenn man E drückt
	/*if (!m_currently_active) // das natürlich nur wenn nicht eh grad aktiv, sonst würde sich die jump height exponentiell mit jedem aufruf verdoppeln
	{
		m_value_to_enhance = player->m_char_move_comp->JumpZVelocity; // Wert zwischenspeichern um ihn später wieder zurücksetzen zu können

		player->m_char_move_comp->JumpZVelocity = m_value_to_enhance * 2; // Wert verdoppeln

		m_currently_active = true;

		// jetzt hier ein neues action binding definieren
		// immer wenn das jump event ausgelöst wird, wird statt einem normalen jump getEnhanced() beim spieler aufgerufen
		// so wird z.B. erreicht, dass auch der Sound dann bei normalen Jumps entsprechend abgespielt wird, das passiert ja nur hier im Item
		// das action binding braucht ein handle, damit wir es später einfach wieder entfernen können, was wichtig ist,
		// denn sonst würde es einfach bleiben und auch bei anderen items auslösen
		m_action_binding = FInputActionBinding("Jump", IE_Pressed);
		m_action_binding.ActionDelegate.BindDelegate(player, &AMyPlayer::getEnhanced);
		m_action_binding = player->InputComponent->AddActionBinding(m_action_binding);
	}*/

	// nur springen wenn Spieler grad nicht schon springt (oder fällt)
	if (!player->m_char_move_comp->IsFalling())
	{
		Super::enhance(player); // super funktion wird aufgerufen um den sound abzuspielen

		// spieler nach oben (z-achse) launchen (mit doppelter JumpZVelocity)
		player->LaunchCharacter(FVector(0, 0, player->m_char_move_comp->JumpZVelocity * 2), false, false);

		player->Jump();
	}

}

/*
	* Diese Funktion beendet das Enhancement, indem sie die Manipulation wieder zurücksetzt
	* JumpZVelocity zurücksetzen und Binding entfernen
	*
	* @param player		ein MyPlayer
	*/
void AJumpItem::endEnhancement(AMyPlayer* player)
{
	// im aktuellen zustand nicht nötig

	/*player->m_char_move_comp->JumpZVelocity = m_value_to_enhance;
	m_currently_active = false;
	player->InputComponent->RemoveActionBindingForHandle(m_action_binding.GetHandle());*/
}