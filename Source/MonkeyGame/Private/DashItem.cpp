


#include "DashItem.h"
#include "MyPlayer.h"


/*
	* gibt dem Spieler einen horizontalen boost in blickrichtung
	*
	* @param player		ein MyPlayer, der beinflusst wird
	*/
void ADashItem::enhance(AMyPlayer* player) 
{
	Super::enhance(player); 
	
	// Blick/bewegungsrichtung checken und launch factor ggf invertieren, damit spieler in die richtige richtung bewegt wird
	if (player->m_is_moving_right == false && m_launch_factor > 0 || player->m_is_moving_right && m_launch_factor < 0)
	{
		m_launch_factor = -m_launch_factor;

	}
	
	// Launch Charachter legt einen launch fest, der beim nächsten movement input ausgeführt wird
	// wir wollen den spieler entlang der x-achse boosten
	player->LaunchCharacter(FVector(m_launch_factor, 0, 0), false, false);
	// deswegen müssen wir einmal einen movement input ausführen wenn wir das instant bei tastendruck auslösen wollen
	player->AddMovementInput(FVector(1, 0, 0),  true);

}

void ADashItem::endEnhancement(AMyPlayer* player)
{
	// enhancement muss in diesem Fall nicht beendet werden, da keine werte langfristig verändert werden
}