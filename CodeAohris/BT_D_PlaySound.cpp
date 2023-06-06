#include "BT_D_PlaySound.h"

#include "BT_D_BossMovement.h"
#include "B_Boss.h"
#include "FMODBlueprintStatics.h"
#include "IU_Macros.h"
#include "M_GameMode.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/BTTaskNode.h"

UBT_D_PlaySound::UBT_D_PlaySound()
{
	bNotifyActivation = true;
}

void UBT_D_PlaySound::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	if (UBTCompositeNode* _node = GetParentNode())
	{
		TArray<FBTCompositeChild>& _children = _node->Children;
		
		const uint& _max = _children.Num();					
		for (uint i(0); i < _max; ++i)
		{
			FBTCompositeChild& _compositeChild = _children[i];
			
			TArray<TObjectPtr<UBTDecorator>>& _decorator = _compositeChild.Decorators;
			const uint& _max2 = _decorator.Num();
			for (uint j(0); j < _max2; ++j)
			{
				if (this == _decorator[j] || (bTryGetNextBossMovement && (_compositeChild.ChildTask->GetExecutionIndex() == iPatternExecutionIndex)))
				{
					for (uint k = 0; k < _max2; ++k)
					{
						if (UBT_D_BossMovement* _bossMovement = Cast<UBT_D_BossMovement>(_decorator[k]))
						{
							bossMovement = _bossMovement;
							bossMovement->OnStopMovingAndNotStun().AddUniqueDynamic(this, &UBT_D_PlaySound::PlaySound);
							return;
						}
					}
				}
			}
		}
	}
	
	PlaySound(false);
}

void UBT_D_PlaySound::PlaySound(const bool _isStun)
{
	if (bossMovement)
	{
		bossMovement->OnStopMovingAndNotStun().RemoveDynamic(this, &UBT_D_PlaySound::PlaySound);
		bossMovement = nullptr;
	}

	if (_isStun)return;
	
	const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>();
	if (!_gameMode)return;
	const AActor* _requestingActor = _gameMode->GetBoss();
		
	const TArray<FSongFeedBack>& _soundFeedBack = sound.soundFeedBack;
	const size_t& _max = _soundFeedBack.Num();
	for (size_t i = 0; i < _max; ++i)
	{
		const FSongFeedBack& _songSettings = _soundFeedBack[i];
		if (UFMODEvent* _sound = _songSettings.soundFeedback)
		{
			if (_songSettings.bAttachToRequestingActor)
			{
				UFMODBlueprintStatics::PlayEventAttached(_sound, _requestingActor->GetRootComponent(), NAME_None,
														 _songSettings.vSoundOffset, EAttachLocation::KeepRelativeOffset,
														 true, true, true);
			}
			else
			{
				UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), _sound, _requestingActor->GetTransform(), true);
			}
		}
	}
}