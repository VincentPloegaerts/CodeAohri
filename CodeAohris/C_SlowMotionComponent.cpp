#include "C_SlowMotionComponent.h"
#include "IU_Player.h"
#include "M_GameMode.h"
#include "UC_PlayerAttackComponent.h"

UC_SlowMotionComponent::UC_SlowMotionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UC_SlowMotionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (const AIU_Player* _player = Cast<AIU_Player>(GetOwner()))
	{
		playerAttack = _player->GetCharacterAttackComponent();
	}
	
	if (const AM_GameMode* _gameMode = Cast<AM_GameMode>(GetWorld()->GetAuthGameMode()))
	{
		worldTimeManager = _gameMode->GetWorldTimeManager();
	}
	fBaseOrbConsumptionPerSeconds = fOrbConsumptionPerSeconds;
}

void UC_SlowMotionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SlowMotionTimer();
}

void UC_SlowMotionComponent::ApplySlowMotion() 
{

	if (!playerAttack || !playerAttack->GetValidSlowMoOrbCount(fOrbConsumptionPerSeconds)) return;


	if(!bIsInSlowMotion)
	{
		bIsInSlowMotion = true;
		if (worldTimeManager)
		{
			worldTimeManager->SetWorldInSlowMotion();
			onSlowMoBegin.Broadcast();
		}
		return;
	}

	if(bIsInSlowMotion)
	{
		bIsInSlowMotion = false;
		
		if(playerAttack->GetIsInTuto())
			bHasUsedSlowMo = true;
		
		if (worldTimeManager)
		{
			worldTimeManager->SetWorldDefaultSpeed();
			onSlowMoEnd.Broadcast();
		}
		
	}
}

void UC_SlowMotionComponent::StopSlowMotion()
{
	if(!bIsInSlowMotion)return;
	
	bIsInSlowMotion = false;
	
	if (worldTimeManager)
	{
		worldTimeManager->SetWorldDefaultSpeed();
		onSlowMoEnd.Broadcast();
	}
}

void UC_SlowMotionComponent::SlowMotionTimer()
{
	if (!bIsInSlowMotion) return;
	
	fPrivateSlowMotionTimer += GetWorld()->DeltaTimeSeconds;
	
	if (fPrivateSlowMotionTimer > 1.0f)
	{
		fPrivateSlowMotionTimer = 0.0f;

		if (!playerAttack)return;
		const bool _b = playerAttack->GetValidSlowMoOrbCount(fOrbConsumptionPerSeconds);
		
		if (!_b)
		{
			onSlowMoEnd.Broadcast();
			StopSlowMotion();
		}
	}
}