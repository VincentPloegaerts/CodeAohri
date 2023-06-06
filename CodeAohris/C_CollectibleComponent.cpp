#include "C_CollectibleComponent.h"

#include "HUD_Main.h"
#include "M_GameInstance.h"
#include "M_GameMode.h"
#include "M_UIManager.h"
#include "UI_PlayerWidget.h"
#include "M_CollectibleManager.h"
#include "Kismet/GameplayStatics.h"

void UC_CollectibleComponent::BeginPlay()
{
	Super::BeginPlay();
	InitMaxCollectible();
	
	FTimerHandle _timer;
	GetWorld()->GetTimerManager().SetTimer(_timer, this, &UC_CollectibleComponent::InitMaxCollectible, FMath::FRandRange(1.0f, 2.0f));
}

void UC_CollectibleComponent::InitMaxCollectible()
{
	if(const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
	{
		if(AM_CollectibleManager* _collectibleManager = _gameMode->GetCollectibleManager())
			iMaxCollectible = _collectibleManager->GetCollectibleCount();

		if (AHUD_Main* _hud = GetWorld()->GetFirstPlayerController()->GetHUD<AHUD_Main>())
			if(UUI_PlayerWidget* _playerWidget = _hud->GetWidget<UUI_PlayerWidget>(WIDGET_PLAYER))
				_playerWidget->SetParchmentVisibility(iMaxCollectible > 0);
	}
	
	EventBroadcast();
}

UC_CollectibleComponent::UC_CollectibleComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UC_CollectibleComponent::AddCollectible(const int _number)
{
	const int _current = iCurrentCollectible + _number;
	iCurrentCollectible = _current > iMaxCollectible ? iMaxCollectible : _current;
	EventBroadcast();

	if(iCurrentCollectible >= iMaxCollectible)
	{
		const UM_GameInstance* _gameInstance = GetWorld()->GetGameInstance<UM_GameInstance>();

		if(!_gameInstance)
			return;

		UAPI_SteamIntegration* _steam = _gameInstance->GetSteamIntegration();

		if(_steam)
			_steam->ValidateSuccess("ACH_COLLECT_ALL_COLLECTIBLE");
	}
}

