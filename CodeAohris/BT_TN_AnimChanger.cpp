#include "BT_TN_AnimChanger.h"

#include "B_Boss.h"
#include "B_BossAnim.h"
#include "IU_Macros.h"
#include "M_GameMode.h"

EBTNodeResult::Type UBT_TN_AnimChanger::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
	{
		if (const AB_Boss* _boss = _gameMode->GetBoss())
		{
			if (UB_BossAnim* _bossAnim = _boss->GetBossAnim())
			{
				if (newIdleAnim)
				{
					_bossAnim->SetCurrentIdleAnim(newIdleAnim);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}

	LOG_ERROR("UBT_TN_AnimChanger::ExecuteTask -> Invalid boss anim instance or NewIdleAnim is nullptr");
	return EBTNodeResult::Failed;
}