#include "P_DT_ShootDataTable.h"

#include "M_SaveManager.h"

#include "STRUCT_PatternData.h"

FProjectileSettings UP_DT_ShootDataTable::GetProjectileSettings() const
{
	FProjectileSettings _settings;

	_settings.bEditProjectileKnockbackSettings = bEditProjectileKnockbackSettings;
	_settings.projectileKnockbackSettings = projectileKnockbackSettings;
	
	_settings.bEditGlobalSettings = bEditGlobalSettings;
	_settings.bEditGuidedSettings = bEditGuidedSettings;
	_settings.bEditSimulatedSettings = bEditSimulatedSettings;
	_settings.bEditBallisticSettings = bEditBallisticSettings;
	_settings.bEditSinusoidalSettings = bEditSinusoidalSettings;
	
	if (projectileType == EProjectileType::Directional && directional)
	{
		directional->GetProjectileSettings(_settings);
	}
	else if ((projectileType == EProjectileType::Guided || projectileType == EProjectileType::FatGuided) && guided)
	{
		guided->GetProjectileSettings(_settings);
		if (bEditGuidedSettings)
			_settings.guidedSettings = guidedSettings;
	}
	else if (projectileType == EProjectileType::Simulated && simulated)
	{
		simulated->GetProjectileSettings(_settings);
		if (bEditSimulatedSettings)
			_settings.simulatedSettings = simulatedSettings;
	}
	else if (projectileType == EProjectileType::Ballistic && ballistic)
	{
		ballistic->GetProjectileSettings(_settings);
		if (bEditBallisticSettings)
			_settings.ballisticSettings = ballisticSettings;
	}
	else if (projectileType == EProjectileType::Sinusoidal && sinusoidal)
	{
		sinusoidal->GetProjectileSettings(_settings);
		if (bEditSinusoidalSettings)
			_settings.sinusoidalSettings = sinusoidalSettings;
	}
	
	if (bEditGlobalSettings)
		_settings.globalSettings = globalSettings;

	return _settings; 
}

#if WITH_EDITOR
void UP_DT_ShootDataTable::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.Property->GetName() == "directional" && directional)
	{
		bEditGlobalSettings = false;
		globalSettings = directional->GetProjectileGlobalSettings();
	}
	if (PropertyChangedEvent.Property->GetName() == "guided" && guided)
	{
		bEditGlobalSettings = false;
		bEditGuidedSettings = false;
		globalSettings = guided->GetProjectileGlobalSettings();
		guidedSettings = guided->GetProjectileGuidedSettings();
	}
	if (PropertyChangedEvent.Property->GetName() == "simulated" && simulated)
	{
		bEditGlobalSettings = false;
		bEditSimulatedSettings = false;
		globalSettings = simulated->GetProjectileGlobalSettings();
		simulatedSettings = simulated->GetProjectileSimulatedSettings();		
	}
	if (PropertyChangedEvent.Property->GetName() == "ballistic" && ballistic)
	{
		bEditGlobalSettings = false;
		bEditBallisticSettings = false;
		globalSettings = ballistic->GetProjectileGlobalSettings();
		ballisticSettings = ballistic->GetProjectileBallisticSettings();		
	}
	if (PropertyChangedEvent.Property->GetName() == "sinusoidal" && sinusoidal)
	{
		bEditGlobalSettings = false;
		bEditSinusoidalSettings = false;
		globalSettings = sinusoidal->GetProjectileGlobalSettings();
		sinusoidalSettings = sinusoidal->GetProjectileSinusoidalSettings();		
	}

	if (PropertyChangedEvent.Property->GetName() == "projectileType")
	{
		shootSettings.projectileType = projectileType;
	}

	bValidDirectional = directional && projectileType == EProjectileType::Directional;
	bValidGuided = guided && (projectileType == EProjectileType::Guided || projectileType == EProjectileType::FatGuided);
	bValidSimulated = simulated && projectileType == EProjectileType::Simulated;
	bValidBallistic = ballistic && projectileType == EProjectileType::Ballistic;
	bValidSinusoidal = sinusoidal && projectileType == EProjectileType::Sinusoidal;
	bValidGlobal = bValidDirectional || bValidGuided || bValidSimulated || bValidBallistic || bValidSinusoidal;

	if (!bValidGlobal)
		bEditGlobalSettings = false;
	if (!bValidGuided)
		bEditGuidedSettings = false;
	if (!bValidSimulated)
		bEditSimulatedSettings = false;
	if (!bValidBallistic)
		bEditBallisticSettings = false;
	if (!bValidSinusoidal)
		bEditSinusoidalSettings = false;
}

void UP_DT_ShootDataTable::SavePattern()
{
	FShootDataTableJSON _json = GetShootDataTableJSON();
	
	UM_SaveManager::Save(_json, Save_Pattern, GetName());
}
void UP_DT_ShootDataTable::LoadPattern()
{
	FShootDataTableJSON _json;
	const bool& _loadSuccess = UM_SaveManager::Load(_json, Save_Pattern, GetName());
	if (!_loadSuccess) return;

	LoadShootDataTableJSON(_json);
}

FShootDataTableJSON UP_DT_ShootDataTable::GetShootDataTableJSON() const
{
	FShootDataTableJSON _json;
	_json.InitBaseJSON(GetBaseDataTableJSON());
	_json.InitPatternJSON(GetPatternDataTableJSON());
	
	_json.shootSettings = shootSettings;
	_json.bShootPoint = bShootPoint;
	_json.shootPointSettings = shootPointSettings;
	_json.bShootLine = bShootLine;
	_json.shootLineSettings = shootLineSettings;
	_json.bShootCircle = bShootCircle;
	_json.shootCircleSettings = shootCircleSettings;
	_json.bShootArc = bShootArc;
	_json.shootArcSettings = shootArcSettings;
	_json.bShootCross = bShootCross;
	_json.shootCrossSettings = shootCrossSettings;
	_json.bShootRandom2D = bShootRandom2D;
	_json.shootRandom2DSettings = shootRandom2DSettings;
	_json.bShootRandom3D = bShootRandom3D;
	_json.shootRandom3DSettings = shootRandom3DSettings;
	_json.projectileSettings = GetProjectileSettings();
	_json.directional = directional;
	_json.guided = guided;
	_json.simulated = simulated;
	_json.ballistic = ballistic;
	_json.sinusoidal = sinusoidal;
	return _json;
}
void UP_DT_ShootDataTable::LoadShootDataTableJSON(const FShootDataTableJSON& _json)
{
	LoadBaseDataTableJSON(_json);
	LoadPatternDataTableJSON(_json);
	
	shootSettings = _json.shootSettings;
	bShootPoint = _json.bShootPoint;
	shootPointSettings = _json.shootPointSettings;
	bShootLine = _json.bShootLine;
	shootLineSettings = _json.shootLineSettings;
	bShootCircle = _json.bShootCircle;
	shootCircleSettings = _json.shootCircleSettings;
	bShootArc = _json.bShootArc;
	shootArcSettings = _json.shootArcSettings;
	bShootCross = _json.bShootCross;
	shootCrossSettings = _json.shootCrossSettings;
	bShootRandom2D = _json.bShootRandom2D;
	shootRandom2DSettings = _json.shootRandom2DSettings;
	bShootRandom3D = _json.bShootRandom3D;
	shootRandom3DSettings = _json.shootRandom3DSettings;

	directional = _json.directional;
	guided = _json.guided;
	simulated = _json.simulated;
	ballistic = _json.ballistic;
	sinusoidal = _json.sinusoidal;
	
	const FProjectileSettings _projectileSettings = _json.projectileSettings;
	projectileType = shootSettings.projectileType;
	bEditGlobalSettings = _projectileSettings.bEditGlobalSettings;
	globalSettings = _projectileSettings.globalSettings;
	bEditGuidedSettings = _projectileSettings.bEditGuidedSettings;
	guidedSettings = _projectileSettings.guidedSettings;
	bEditSimulatedSettings = _projectileSettings.bEditSimulatedSettings;
	simulatedSettings = _projectileSettings.simulatedSettings;
	bEditBallisticSettings = _projectileSettings.bEditBallisticSettings;
	ballisticSettings = _projectileSettings.ballisticSettings;
	bEditSinusoidalSettings = _projectileSettings.bEditSinusoidalSettings;
	sinusoidalSettings = _projectileSettings.sinusoidalSettings;
}

void UP_DT_ShootDataTable::InitializePatternData(FPatternData& _data) const
{
	Super::InitializePatternData(_data);
	_data.SetPatternDataTableType(EPatternDataTableType::PATTERN_SHOOT);

	_data.SetShootSettings(shootSettings);

	_data.SetShootPointState(bShootPoint);
	_data.SetShootLineState(bShootLine);
	_data.SetShootCircleState(bShootCircle);
	_data.SetShootArcState(bShootArc);
	_data.SetShootCrossState(bShootCross);
	_data.SetShootRandom2DState(bShootRandom2D);
	_data.SetShootRandom3DState(bShootRandom3D);
	
	_data.SetShootPointSettings(shootPointSettings);
	_data.SetShootLineSettings(shootLineSettings);
	_data.SetShootCircleSettings(shootCircleSettings);
	_data.SetShootArcSettings(shootArcSettings);
	_data.SetShootCrossSettings(shootCrossSettings);
	_data.SetShootRandom2DSettings(shootRandom2DSettings);
	_data.SetShootRandom3DSettings(shootRandom3DSettings);
	_data.SetProjectileShootSettings(GetProjectileSettings());
}
#endif