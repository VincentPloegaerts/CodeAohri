#include "C_RespawnComponent.h"

#include "IU_HealthSystemComponent.h"
#include "IU_PlayerMovementComponent.h"
#include "M_GameInstance.h"
#include "S_GameSave.h"
#include "TriggerBoxLevelLoading.h"
#include "UC_PlayerAttackComponent.h"

UC_RespawnComponent::UC_RespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UC_RespawnComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeFadingWidget();
	
	owner = Cast<AIU_Player>(GetOwner());
	gameSave = owner->GetGameInstance<UM_GameInstance>()->GetCurrentGameSave();
}
void UC_RespawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bIsFading)
		UpdateFading(DeltaTime);
}

void UC_RespawnComponent::Death(bool _isDead)
{
	isDead = _isDead;
	
	FadingStart();
	
	if (owner)
	{
		owner->SetPlayerStatus(false, isDead);
		UC_PlayerAnimInstance* _playerAnimInst = Cast<UC_PlayerAnimInstance>(owner->GetSkeletalMesh()->GetAnimInstance());
		if (owner->HealthComponent()->IsDead())
		{
			if (gameSave && gameSave->GetSaveTriggerDeath())
			{
				TArray<TSoftObjectPtr<ATriggerBoxLevelLoading>> _boxes = gameSave->GetSaveTriggerDeath()->CorrespondingLevelStreamer();
				const int& _max = _boxes.Num();
				for (int i = 0;i<_max;i++)
				{
					_boxes[i]->AddLevels();
				}
			}
		}
		if(!_playerAnimInst)return;

		_playerAnimInst->UpdateIsSliding(false);
	}
}
void UC_RespawnComponent::Respawn()
{
	if (owner)
	{
		owner->SetPlayerStatus(true, isDead);
		if (owner->HealthComponent()->IsDead())
			owner->HealthComponent()->ResetHealth();
	}
	
	onRespawn.Broadcast();
}

void UC_RespawnComponent::InitializeFadingWidget()
{
	fadingWidget = CreateWidget<UUserWidget>(GetWorld(), fadingWidgetType);
	if (fadingWidget)
	{
		fadingWidget->AddToViewport();
		SetFadingWidgetOpacity(0.0f);
	}
}
void UC_RespawnComponent::UpdateFading(const float _deltaTime)
{
	if (!fadingWidget) return;

	if (bIsFadingIn)
		UpdateFadingIn(_deltaTime);
	else if (!bIsHolding)
		UpdateFadingOut(_deltaTime);
	else
		UpdateHolding(_deltaTime);
		
	const float _opacity = fFadingProgression / GetFadingDuration();
	SetFadingWidgetOpacity(_opacity);
}

void UC_RespawnComponent::UpdateFadingIn(const float _deltaTime)
{
	fFadingProgression += _deltaTime;
	
	if (fFadingProgression >= GetFadingDuration())
	{
		bIsFadingIn = false;
		bIsHolding = true;
		
		owner->SetPlayerStatus(true, isDead);
		owner->SetPlayerInputs(false);
	}
}
void UC_RespawnComponent::UpdateFadingOut(const float _deltaTime)
{
	fFadingProgression -= _deltaTime;

	if (fFadingProgression <= 0)
	{
		FadingCompleted();
	}
}
void UC_RespawnComponent::UpdateHolding(const float _deltaTime)
{
	fElapsedHoldingTime += _deltaTime;
	if (fElapsedHoldingTime >= fHoldTime + (owner->HealthComponent()->IsDead() ? fAdditionalHoldTimeDeath : 0.0f))
	{
		bIsHolding = false;
		Respawn();
	}
}

void UC_RespawnComponent::FadingStart()
{
	fFadingProgression = 0;
	fElapsedHoldingTime = 0;
	bIsFading = true;
	bIsFadingIn = true;
	bIsHolding = false;
	onRespawnStart.Broadcast();
}
void UC_RespawnComponent::FadingCompleted()
{
	bIsFading = false;
	bIsFadingIn = false;
	bIsHolding = false;
	fFadingProgression = 0;
	fElapsedHoldingTime = 0;

	if (owner)
		owner->MovementComponent()->SetCanMove(true);

	onFadeInFinish.Broadcast();
}

void UC_RespawnComponent::SetFadingWidgetOpacity(const float _opacity) const
{
	fadingWidget->SetRenderOpacity(_opacity);
}

void UC_RespawnComponent::ToDeath()
{
	FTimerDelegate _delegate;
	_delegate.BindUFunction(this, "Death", true);
	FTimerHandle _timer;
	float _intime = 0.f;

	if (!owner)return;

	owner->GetPlayerAnimInstance()->SetDeadStatus();
	owner->MovementComponent()->SetCanMove(false);
	owner->GetCharacterAttackComponent()->CheckHoldingTime();
	if (owner->IsInArena())
		_intime = fArenaRespawnTime;
	else
		_intime = fRespawnTime;


	GetWorld()->GetTimerManager().SetTimer(_timer, _delegate, _intime, false);
	//Death(true); 
}
