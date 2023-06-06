#include "BT_D_CheckPlayerAFK.h"

#include "IU_Macros.h"
#include "IU_PlayerMovementComponent.h"
#include "IU_Player.h"
#include "M_GameMode.h"

bool UBT_D_CheckPlayerAFK::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
	{
		if (const AIU_Player* _player = _gameMode->GetPlayer())
		{
			if (const UIU_PlayerMovementComponent* _mouvementComp = _player->MovementComponent())
			{
				const float& _afkTime = _mouvementComp->GetAFKTime();
				const float& _random = FMath::RandRange(fMinimumAFKTime, fMaximumAFKTime);

				return _afkTime > _random;
			}
			else
				LOG("UBT_D_CheckPlayerAFK::CalculateRawConditionValue -> invalid player mouvement component");
		}
		else
			LOG("UBT_D_CheckPlayerAFK::CalculateRawConditionValue -> invalid player");
	}
	else
		LOG("UBT_D_CheckPlayerAFK::CalculateRawConditionValue -> invalid gameMode");
	
	return false;
}

void UBT_D_CheckPlayerAFK::PostInitProperties()
{
	Super::PostInitProperties();
	
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	NodeName = FString("Player AFK");
#endif
}

FString UBT_D_CheckPlayerAFK::GetStaticDescription() const
{	
	return "Check Player AFK";
}

#if WITH_EDITOR
void UBT_D_CheckPlayerAFK::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetFName() == "fMinimumAFKTime")
	{
		if (fMinimumAFKTime > fMaximumAFKTime)
		{
			fMinimumAFKTime = fMaximumAFKTime;
		}
	}
	
	if (PropertyChangedEvent.Property->GetFName() == "fMaximumAFKTime")
	{
		if (fMaximumAFKTime < fMinimumAFKTime)
		{
			fMaximumAFKTime = fMinimumAFKTime;
		}
	}
}
#endif