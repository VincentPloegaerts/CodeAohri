#include "P_PB_Shoot.h"

#include "IU_Macros.h"
#include "M_BlackBoardManager.h"
#include "M_FeedbackManager.h"
#include "M_GameMode.h"
#include "P_DT_ShootDataTable.h"
#include "M_ProjectileManager.h"
#include "M_WorldTimeManager.h"
#include "P_PatternShoot.h"
#include "P_PB_Random.h"
#include "P_Pattern.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UP_PB_Shoot::ActivateBehavior(UP_DT_BaseDataTable* _dataTable)
{
	Super::ActivateBehavior(_dataTable);
	if (!bIsEnable) return;

	shootDataTable = Cast<UP_DT_ShootDataTable>(_dataTable);
	InitializeShoot(shootDataTable); //Will Initialize all Child class, child class check if _data is valid

	if (AP_PatternBase* _pattern = Cast<AP_Pattern>(owner))
		_pattern->OnComplete().AddDynamic(this, &UP_PB_Shoot::DisableShootBehavior);
}

void UP_PB_Shoot::ActivateBehavior(FPatternData& _patternData)
{
	Super::ActivateBehavior(_patternData);
	if (!bIsEnable) return;

	InitializeShoot(_patternData);

	if (AP_PatternBase* _pattern = Cast<AP_Pattern>(owner))
		_pattern->OnComplete().AddDynamic(this, &UP_PB_Shoot::DisableShootBehavior);
}

void UP_PB_Shoot::TickBehavior(const float& _deltaTime)
{
	if (!bIsEnable) return;

	fElapsedTime += _deltaTime;
	fTime += _deltaTime;
	if (fElapsedTime > shootSettings.fFireRate)
	{
		fElapsedTime = 0;
		Shoot();
		feedbackManager->PlaySound(shootSettings.songFeedbackData, GetOwner(), shootSettings.songFeedbackData.fActivationDelay);
		
		blackBoardManager->UPDATE_DEBUG();
		++iProjectileFired;

		//Only check if Shot completion after a shot (avoid checking on tick which is useless)
		CheckShotComplete();
	}
}

void UP_PB_Shoot::InitializeShoot(UP_DT_ShootDataTable* _shootDataTable)
{
	if (!_shootDataTable || !owner) return;
	shootSettings = _shootDataTable->GetShootSettings();
	projectileSettings = _shootDataTable->GetProjectileSettings();
	
	float _multiplier = 1.f;

	if (const AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
		if (const AM_WorldTimeManager* _worldTick = _gameMode->GetWorldTimeManager())
			_multiplier = _worldTick->GetCurrentWorldSpeed() + 0.75f;
	
	fElapsedTime = shootSettings.fFireRate * _multiplier;

	if (const AP_PatternShoot* _owner = Cast<AP_PatternShoot>(owner))
		uRandom = _owner->GetRandomizer();

	if (const AM_GameMode* _gm = GetWorld()->GetAuthGameMode<AM_GameMode>())
	{
		projectileManager = _gm->GetProjectileManager();
		blackBoardManager = _gm->GetBlackBoardManager();
		feedbackManager = _gm->GetFeedbackManager();
	}
}

void UP_PB_Shoot::InitializeShoot(FPatternData& _patternData)
{
	if (!owner) return;
	shootSettings = _patternData.GetShootSettings();
	projectileSettings = _patternData.GetProjectileShootSettings();
	fElapsedTime = shootSettings.fFireRate;

	if (const AP_PatternShoot* _owner = Cast<AP_PatternShoot>(owner))
		uRandom = _owner->GetRandomizer();

	if (const AM_GameMode* _gm = GetWorld()->GetAuthGameMode<AM_GameMode>())
		projectileManager = _gm->GetProjectileManager();
}

void UP_PB_Shoot::Shoot()
{
	onShoot.Broadcast();
	//Declaration in Child class (way of shooting : Line / Circle / ...)
}
void UP_PB_Shoot::ShootProjectile(const FVector& _location, FVector& _defaultDirection) const
{
	if (!projectileManager) return;
	
	if (uRandom)
		uRandom->Randomize(_defaultDirection);

	const FRotator& _rotationLook = UKismetMathLibrary::FindLookAtRotation(_location, _location + _defaultDirection);

	if (shootSettings.bDelayed)
	{
		FTimerHandle _timer;
		FTimerDelegate _delegate;
		const float _delay = FMath::RandRange(shootSettings.minDelay,shootSettings.maxDelay);
		_delegate.BindUFunction(projectileManager, "SpawnProjectileDelayed", _delay, shootSettings.projectileType, projectileSettings, _location, _rotationLook);
		GetWorld()->GetTimerManager().SetTimer(_timer, _delegate, _delay, false);	
	}
	else
		projectileManager->SpawnProjectile(shootSettings.projectileType, projectileSettings, _location, _rotationLook);

	
	//Delay on shoot removed since Pooling system
	/*FTimerHandle _timer;
	FTimerDelegate _delegate;
	_delegate.BindUFunction(projectileManager, "SpawnProjectile", shootSettings.projectileType, projectileSettings, _location, _rotationLook);
	GetWorld()->GetTimerManager().SetTimer(_timer, _delegate, FMath::RandRange(0.01f, 0.03f), false);*/
}

void UP_PB_Shoot::CheckShotComplete()
{
	if (IsFireDurationComplete() || IsFireQuantityComplete())
	{
		bIsEnable = false;
		onBehaviorComplete.Broadcast();
	}
}
bool UP_PB_Shoot::IsFireDurationComplete()
{
	return shootSettings.eShootingMode == EShootingMode::DURATION && fTime > shootSettings.fFireDuration;
}
bool UP_PB_Shoot::IsFireQuantityComplete()
{
	return shootSettings.eShootingMode == EShootingMode::QUANTITY && iProjectileFired >= shootSettings.iFireQuantity;
}
void UP_PB_Shoot::DisableShootBehavior(AP_PatternBase* _pattern)
{
	bIsEnable = false;
}