#include "A_ArenaBlockAnimInstance.h"

void UA_ArenaBlockAnimInstance::UpdateIsOpening(const bool& _isOpening)
{
	isOpening = _isOpening;
	isInstant = false;
}

void UA_ArenaBlockAnimInstance::UpdateIsInstantOpening(const bool& _isOpening)
{
	isOpening = _isOpening;
	isInstant = true;
}