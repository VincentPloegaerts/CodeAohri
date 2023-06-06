#include "B_AFKKiller.h"

#include "IU_Macros.h"
#include "M_GameMode.h"
#include "B_AI_AFKKiller.h"
#include "P_PatternCaster.h"

AB_AFKKiller::AB_AFKKiller()
{
	PrimaryActorTick.bCanEverTick = false;

	patternCaster = CreateDefaultSubobject<UP_PatternCaster>("Pattern Caster");
	AddOwnedComponent(patternCaster);
}

void AB_AFKKiller::BeginPlay()
{
	Super::BeginPlay();

	aiAFKKiller = GetController<AB_AI_AFKKiller>();
	if (!aiAFKKiller)
		LOG("AB_AFKKiller::BeginPlay -> Invalid AI_AFKKiller");
}
