

#include "API_SteamAchievementsTriggerBox.h"
#include "IU_Player.h"
#include "M_GameInstance.h"
#include "API_SteamAchievementsManager.h"
#include "IU_Macros.h"
#include "S_GameSave.h"

AAPI_SteamAchievementsTriggerBox::AAPI_SteamAchievementsTriggerBox()
{
	PrimaryActorTick.bCanEverTick = true;

	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	boxCollider->SetupAttachment(RootComponent);

}

void AAPI_SteamAchievementsTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &AAPI_SteamAchievementsTriggerBox::OnActorOverlap);
	
}

void AAPI_SteamAchievementsTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


UFUNCTION(BlueprintCallable) void AAPI_SteamAchievementsTriggerBox::OnActorOverlap(UPrimitiveComponent* _this, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _index, bool _sweep, const FHitResult& _result)
{
	if (Cast<AIU_Player>(_otherActor))
	{
		if (!isInDesert && !isInTemple)
		{
			LOG("YOU NEED TO SELECT A BOOL IN THE ACHIEVEMENT TRIGGER BOX");
			return;
		}
	

		if (UM_GameInstance* _gameInstance = GetWorld()->GetGameInstance<UM_GameInstance>())
		{
		
			if (isInDesert)
			{
				_gameInstance->GetSteamIntegration()->ValidateSuccess("ACH_TRAVELLER_DESERT");
				if (!_gameInstance->GetCurrentGameSave())return;
				_gameInstance->GetCurrentGameSave()->SetIsInDesertValidated(true);
			}


			if (isInTemple)
			{
				_gameInstance->GetSteamIntegration()->ValidateSuccess("ACH_TRAVELLER_TEMPLE");
				if (!_gameInstance->GetCurrentGameSave())return;
				_gameInstance->GetCurrentGameSave()->SetIsInTempleValidated(true);
				
			}

			_gameInstance->GetCurrentGameSave()->CheckForExplorationAchievementsValidationInGameSave(_gameInstance);
		}
	}
}

