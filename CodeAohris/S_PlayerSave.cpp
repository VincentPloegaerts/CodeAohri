#include "S_PlayerSave.h"

void US_PlayerSave::PlayerSaveEdited() const
{
	onPlayerSaveEdited.Broadcast(this);
}