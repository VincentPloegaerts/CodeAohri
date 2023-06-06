#include "ExternalLifeBar.h"

void UExternalLifeBar::SetLife(float _current, float _max)
{
	lifeBar->SetPercent(_current / _max);
}
