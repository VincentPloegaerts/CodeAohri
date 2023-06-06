#include "BT_TN_PlaySound.h"

#include "B_Boss.h"
#include "FMODBlueprintStatics.h"
#include "M_GameMode.h"

UBT_TN_PlaySound::UBT_TN_PlaySound()
{
	
}

EBTNodeResult::Type UBT_TN_PlaySound::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>();
	if (!_gameMode)return EBTNodeResult::Failed;
	
	const AActor* _requestingActor = _gameMode->GetBoss();
		
	const size_t& _max = soundFeedBack.Num();
	for (size_t i = 0; i < _max; ++i)
	{
		const FSongFeedBack& _songSettings = soundFeedBack[i];
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
	return EBTNodeResult::Succeeded;
}