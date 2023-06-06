#include "P_PatternShoot.h"

#include "M_GameMode.h"
#include "M_PatternsTickManager.h"
#include "M_ProjectileManager.h"
#include "P_DT_ShootDataTable.h"
#include "P_PB_ShootPoint.h"
#include "P_PB_ShootCircle.h"
#include "P_PB_ShootLine.h"
#include "P_PB_ShootArc.h"
#include "P_PB_ShootCross.h"

#include "P_PB_Random2D.h"
#include "P_PB_Random3D.h"

AP_PatternShoot::AP_PatternShoot()
{
	shootPointBehavior = CreateDefaultSubobject<UP_PB_ShootPoint>("ShootPointBehavior");
	AddOwnedComponent(shootPointBehavior);
	
	shootCircleBehavior = CreateDefaultSubobject<UP_PB_ShootCircle>("ShootCircleBehavior");
	AddOwnedComponent(shootCircleBehavior);
	
	shootLineBehavior = CreateDefaultSubobject<UP_PB_ShootLine>("ShootLineBehavior");
	AddOwnedComponent(shootLineBehavior);
	
	shootArcBehavior = CreateDefaultSubobject<UP_PB_ShootArc>("ShootArcBehavior");
	AddOwnedComponent(shootArcBehavior);
	
	shootCrossBehavior = CreateDefaultSubobject<UP_PB_ShootCross>("ShootCrossBehavior");
	AddOwnedComponent(shootCrossBehavior);

	uRandom2D = CreateDefaultSubobject<UP_PB_Random2D>("Random2DComponent");
	AddOwnedComponent(uRandom2D);
	uRandom3D = CreateDefaultSubobject<UP_PB_Random3D>("Random3DComponent");
	AddOwnedComponent(uRandom3D);
}

void AP_PatternShoot::BeginPlay()
{
	Super::BeginPlay();
	InitializePatternShoot();
}
void AP_PatternShoot::InitializePatternShoot()
{
	shootPointBehavior->OnShoot().AddUniqueDynamic(this, &AP_PatternShoot::PatternBehaviorOnShoot);
	shootLineBehavior->OnShoot().AddUniqueDynamic(this, &AP_PatternShoot::PatternBehaviorOnShoot);
	shootArcBehavior->OnShoot().AddUniqueDynamic(this, &AP_PatternShoot::PatternBehaviorOnShoot);
	shootCircleBehavior->OnShoot().AddUniqueDynamic(this, &AP_PatternShoot::PatternBehaviorOnShoot);
	shootCrossBehavior->OnShoot().AddUniqueDynamic(this, &AP_PatternShoot::PatternBehaviorOnShoot);
}

void AP_PatternShoot::PatternBehaviorOnShoot()
{
	onPatternShoot.Broadcast();
}

void AP_PatternShoot::InitializePattern(UP_DT_BaseDataTable* _dataTable)
{
	Super::InitializePattern(_dataTable);

	if (AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
		projectileManager = _gameMode->GetProjectileManager();
	
	UP_DT_ShootDataTable* _shootDataTable = Cast<UP_DT_ShootDataTable>(_dataTable);
	if (!_shootDataTable) return;

	AM_PatternsTickManager::FOnTick& _onTick = patternsTickManager->GetOnTick();	
	
	//Activate Behavior Component depends on booleans define in DataTable
	//Random Component MUST be activate before Shoot Component (Shoot component require Random to be activate first)
	if (uRandom2D && _shootDataTable->GetShootRandom2DState())
	{
		uRandom2D->ActivateBehavior(_shootDataTable);
		uRandomizer = uRandom2D;		
	}
	if (uRandom3D && _shootDataTable->GetShootRandom3DState())
	{
		uRandom3D->ActivateBehavior(_shootDataTable);
		uRandomizer = uRandom3D;
	}

	InitializeBehavior(_shootDataTable->GetShootPointState(), shootPointBehavior, _shootDataTable, _onTick);
	InitializeBehavior(_shootDataTable->GetShootCircleState(), shootCircleBehavior, _shootDataTable, _onTick);
	InitializeBehavior(_shootDataTable->GetShootLineState(), shootLineBehavior, _shootDataTable, _onTick);
	InitializeBehavior(_shootDataTable->GetShootArcState(), shootArcBehavior, _shootDataTable, _onTick);
	InitializeBehavior(_shootDataTable->GetShootCrossState(), shootCrossBehavior, _shootDataTable, _onTick);
}

void AP_PatternShoot::InitializePatternData(FPatternData& _patternData)
{
	Super::InitializePatternData(_patternData);
	
	if (AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>())
		projectileManager = _gameMode->GetProjectileManager();

	AM_PatternsTickManager::FOnTick& _onTick = patternsTickManager->GetOnTick();	
	
	//Activate Behavior Component depends on booleans define in DataTable
	//Random Component MUST be activate before Shoot Component (Shoot component require Random to be activate first)
	if (uRandom2D && _patternData.GetShootRandom2DState())
	{
		uRandom2D->ActivateBehavior(_patternData);
		uRandomizer = uRandom2D;		
	}
	if (uRandom3D && _patternData.GetShootRandom3DState())
	{
		uRandom3D->ActivateBehavior(_patternData);
		uRandomizer = uRandom3D;
	}

	InitializeBehavior(_patternData.GetShootPointState(), shootPointBehavior, _patternData, _onTick);
	InitializeBehavior(_patternData.GetShootCircleState(), shootCircleBehavior, _patternData, _onTick);
	InitializeBehavior(_patternData.GetShootLineState(), shootLineBehavior, _patternData, _onTick);
	InitializeBehavior(_patternData.GetShootArcState(), shootArcBehavior, _patternData, _onTick);
	InitializeBehavior(_patternData.GetShootCrossState(), shootCrossBehavior, _patternData, _onTick);
}

void AP_PatternShoot::DeActivatePatternBehavior()
{
	Super::DeActivatePatternBehavior();
	
	AM_PatternsTickManager::FOnTick& _onTick = patternsTickManager->GetOnTick();
	
	uRandom2D->DeActivateBehavior();
	uRandom3D->DeActivateBehavior();
	uRandomizer = nullptr;
	
	DeActivateBehavior(shootPointBehavior, _onTick);
	DeActivateBehavior(shootCircleBehavior, _onTick);
	DeActivateBehavior(shootLineBehavior, _onTick);
	DeActivateBehavior(shootArcBehavior, _onTick);
	DeActivateBehavior(shootCrossBehavior, _onTick);
}
