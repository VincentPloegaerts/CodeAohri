#include "T_AdditionalCondition.h"
#include "UC_PlayerAttackComponent.h"

UT_AdditionalCondition::UT_AdditionalCondition()
{
	
}

void UT_AdditionalCondition::InitCondition(const AIU_Player* _player)
{
	switch (type)
	{
		case CONDITION_CHARGED_SHOT:
			attack = _player->GetCharacterAttackComponent();
			if (attack)
			{
				attack->OnPlayerShootFullyCharged().AddUniqueDynamic(this, &UT_AdditionalCondition::StopCondition);
			}
				
		default:
			return;
	}
}

void UT_AdditionalCondition::ResetCondition()
{
	if (attack)
	{
		attack->OnPlayerShootFullyCharged().RemoveDynamic(this, &UT_AdditionalCondition::StopCondition);
	}
}

void UT_AdditionalCondition::StopCondition()
{
	onConditionDone.Broadcast();
}