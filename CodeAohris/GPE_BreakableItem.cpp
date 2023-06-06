#include "GPE_BreakableItem.h"

void AGPE_BreakableItem::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void AGPE_BreakableItem::Init()
{
	//TODO
	healthSystemComponent->OnDeath()->AddDynamic(this, &AGPE_BreakableItem::SetObjectsDestructible);
}

void AGPE_BreakableItem::SetObjectsDestructible()
{
	const int _max = protectedObjects.Num();
	for (int i = 0; i < _max ; i++)
	{
		if (protectedObjects[i] && protectedObjects[i]->HealthSystemComponent())
		{
			protectedObjects[i]->HealthSystemComponent()->SetCanTakeDamages(true);
		}
	}
}
