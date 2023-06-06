#include "Editor_PatternBehaviorWidgets.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBoxSlot.h"

#include "Editor_OverrideWidgets.h"

#include "A_DT_ArenaDataTable.h"
#include "GPE_FloorDanger.h"
#include "GPE_Projectile.h"
#include "GPE_HuntRift.h"

#include "IU_Macros.h"

#pragma region PB Rotation
#pragma region Angle
void UEditor_PB_AngleWidget::InitializeAngleWidget(const FAngleSettings& _angleSettings)
{
	reverseCB->SetCheckedState(_angleSettings.bReverse ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	countSP->SetValue(_angleSettings.iCount);
	angleSP->SetValue(_angleSettings.fAngle);
	axisCB->SetSelectedOption(*UEnum::GetValueAsString<EAxisMode>(_angleSettings.eAxis));
}
FAngleSettings UEditor_PB_AngleWidget::GetAngleSettings() const
{
	FAngleSettings _angleSettings;
	_angleSettings.bReverse = UMiscLibrary::GetCheckBoxValue(reverseCB);
	_angleSettings.iCount = countSP->GetValue();
	_angleSettings.fAngle = angleSP->GetValue();
	_angleSettings.eAxis = TEnumAsByte<EAxisMode>(UMiscLibrary::GetEnumValueFromString(axisCB->GetSelectedOption(), "EAxisMode"));
	return _angleSettings;
}

void UEditor_PB_AngleWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}
void UEditor_PB_AngleWidget::InitializeWidget()
{
	UMiscLibrary::InitComboBoxFromEnum("EAxisMode", axisCB);
}
#pragma endregion

#pragma region Instant
void UEditor_PB_InstantWidget::InitializeInstantWidget(const FInstantSettings& _instantSettings)
{
	rotationSP->SetValue(_instantSettings.fRotation);
	axisCB->SetSelectedOption(UMiscLibrary::GetEnumValueAsString<EAxisMode>(_instantSettings.eAxis));
}
FInstantSettings UEditor_PB_InstantWidget::GetInstantSettings() const
{
	FInstantSettings _instantSettings;
	_instantSettings.fRotation = rotationSP->GetValue();
	_instantSettings.eAxis = TEnumAsByte<EAxisMode>(UMiscLibrary::GetEnumValueFromString(axisCB->GetSelectedOption(), "EAxisMode"));
	return _instantSettings;
}

void UEditor_PB_InstantWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}
void UEditor_PB_InstantWidget::InitializeWidget()
{
	UMiscLibrary::InitComboBoxFromEnum("EAxisMode", axisCB);
}
#pragma endregion

#pragma region Rotator
void UEditor_PB_RotatorWidget::InitializeRotatorWidget(const FRotatorSettings& _rotatorSettings)
{
	UMiscLibrary::SetCheckBoxState(clockwiseCB, _rotatorSettings.bClockWise);
	UMiscLibrary::SetCheckBoxState(pitchCB, _rotatorSettings.bPitchRotation);
	UMiscLibrary::SetCheckBoxState(yawCB, _rotatorSettings.bYawRotation);
	UMiscLibrary::SetCheckBoxState(rollCB, _rotatorSettings.bRollRotation);
}
FRotatorSettings UEditor_PB_RotatorWidget::GetRotatorSettings() const
{
	FRotatorSettings _rotatorSettings;
	_rotatorSettings.bClockWise = UMiscLibrary::GetCheckBoxValue(clockwiseCB);
	_rotatorSettings.bPitchRotation = UMiscLibrary::GetCheckBoxValue(pitchCB);
	_rotatorSettings.bYawRotation = UMiscLibrary::GetCheckBoxValue(yawCB);
	_rotatorSettings.bRollRotation = UMiscLibrary::GetCheckBoxValue(rollCB);
	return _rotatorSettings;
}
#pragma endregion

#pragma region Target
void UEditor_PB_TargetWidget::InitializeTargetWidget(const FTargetSettings& _targetSettings)
{
	UMiscLibrary::SetCheckBoxState(lerpCB, _targetSettings.bLerp);
	UMiscLibrary::SetCheckBoxState(lookPitchPlayerCB, _targetSettings.bLookPitchPlayer);
}
FTargetSettings UEditor_PB_TargetWidget::GetTargetSettings() const
{
	FTargetSettings _targetSettings;
	_targetSettings.bLerp = UMiscLibrary::GetCheckBoxValue(lerpCB);
	_targetSettings.bLookPitchPlayer = UMiscLibrary::GetCheckBoxValue(lookPitchPlayerCB);
	return _targetSettings;
}
#pragma endregion

#pragma region Rotation
void UEditor_PB_RotationWidget::InitializeRotationWidget(const FPatternData& _patternData)
{
	InitializeVisibility(_patternData);	
	
	const FRotationSettings& _rotationSettings = _patternData.GetRotationSettings();
	rotationSpeedSP->SetValue(_rotationSettings.fRotationSpeed);
	
	angleWidget->InitializeAngleWidget(_patternData.GetAngleSettings());
	instantWidget->InitializeInstantWidget(_patternData.GetInstantSettings());
	rotatorWidget->InitializeRotatorWidget(_patternData.GetRotatorSettings());
	targetWidget->InitializeTargetWidget(_patternData.GetTargetSettings());	
}
void UEditor_PB_RotationWidget::GetRotationOverride(FPatternData& _patternData)
{
	FRotationSettings _rotationSettings;
	_rotationSettings.fRotationSpeed = rotationSpeedSP->GetValue();
	
	_patternData.SetRotationSettings(_rotationSettings);
	_patternData.SetAngleSettings(angleWidget->GetAngleSettings());
	_patternData.SetInstantSettings(instantWidget->GetInstantSettings());
	_patternData.SetRotatorSettings(rotatorWidget->GetRotatorSettings());
	_patternData.SetTargetSettings(targetWidget->GetTargetSettings());
}

void UEditor_PB_RotationWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidgets();
}
void UEditor_PB_RotationWidget::InitializeWidgets()
{
	if (angleWidgetType.Get())
	{
		angleWidget = CreateWidget<UEditor_PB_AngleWidget>(this, angleWidgetType);
		rotationVB->AddChildToVerticalBox(angleWidget);
	}
	else
		LOG("Rotation Widget => Angle Widget Type is not valid.")	
	if (instantWidgetType.Get())
	{
		instantWidget = CreateWidget<UEditor_PB_InstantWidget>(this, instantWidgetType);
		rotationVB->AddChildToVerticalBox(instantWidget);
	}
	else
		LOG("Rotation Widget => Instant Widget Type is not valid.")
	
	if (rotatorWidgetType.Get())
	{
		rotatorWidget = CreateWidget<UEditor_PB_RotatorWidget>(this, rotatorWidgetType);
		rotationVB->AddChildToVerticalBox(rotatorWidget);
	}
	else
		LOG("Rotation Widget => Rotator Widget Type is not valid.")
	
	if (targetWidgetType.Get())
	{
		targetWidget = CreateWidget<UEditor_PB_TargetWidget>(this, targetWidgetType);
		rotationVB->AddChildToVerticalBox(targetWidget);
	}
	else
		LOG("Rotation Widget => Target Widget Type is not valid.")	
}

void UEditor_PB_RotationWidget::InitializeVisibility(const FPatternData& _patternData)
{
	const bool& _angle = _patternData.GetAngleState();
	const bool& _instant = _patternData.GetInstantState();
	const bool& _rotator = _patternData.GetRotatorState();
	const bool& _target = _patternData.GetTargetState();
	const bool& _all = _angle || _instant || _rotator || _target;
	
	UpdateWidget(angleWidget, _angle);
	UpdateWidget(instantWidget, _instant);
	UpdateWidget(rotatorWidget, _rotator);
	UpdateWidget(targetWidget, _target);

	UMiscLibrary::SetWidgetVisibility(rotationSpeedHB, _all);
}
void UEditor_PB_RotationWidget::UpdateWidget(UWidget* _widget, const bool& _visible) const
{
	UMiscLibrary::SetWidgetVisibility(_widget, _visible);
	if (_visible)
		rotationVB->AddChildToVerticalBox(_widget);
	else
		rotationVB->RemoveChild(_widget);
}
#pragma endregion
#pragma endregion

#pragma region PB Movement
#pragma region Movement On Axe
void UEditor_PB_OnAxeWidget::InitializeOnAxeWidget(const FMovementOnAxeSettings& _movementOnAxeSettings)
{
	repeatSP->SetValue(_movementOnAxeSettings.iRepeat);
	movementSpeedSP->SetValue(_movementOnAxeSettings.fMovementSpeed);
	distanceSP->SetValue(_movementOnAxeSettings.fDistance);
	axisCB->SetSelectedOption(*UEnum::GetValueAsString<EAxisMode>(_movementOnAxeSettings.axis));
}
FMovementOnAxeSettings UEditor_PB_OnAxeWidget::GetOnAxeSettings() const
{
	FMovementOnAxeSettings _onAxeSettings;
	_onAxeSettings.iRepeat = repeatSP->GetValue();
	_onAxeSettings.fMovementSpeed = movementSpeedSP->GetValue();
	_onAxeSettings.fDistance = distanceSP->GetValue();
	_onAxeSettings.axis = TEnumAsByte<EAxisMode>(UMiscLibrary::GetEnumValueFromString(axisCB->GetSelectedOption(), "EAxisMode"));
	return _onAxeSettings;
}

void UEditor_PB_OnAxeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}
void UEditor_PB_OnAxeWidget::InitializeWidget()
{
	UMiscLibrary::InitComboBoxFromEnum("EAxisMode", axisCB);
}
#pragma endregion

#pragma region Movement To Target
void UEditor_PB_ToTargetWidget::InitializeToTargetWidgetUtil(const UA_DT_ArenaDataTable* _arenaDataTable)
{
	pointNames = _arenaDataTable->GetArenaSpawnPoint();

	UMiscLibrary::InitComboBoxFromNames(pointNames, arenaPointNameCB);
}
void UEditor_PB_ToTargetWidget::InitializeToTargetWidget(const FMovementToTargetSettings& _movementToTargetSettings)
{
	repeatSP->SetValue(_movementToTargetSettings.iRepeat);
	movementSpeedSP->SetValue(_movementToTargetSettings.fMovementSpeed);
	targetCB->SetSelectedOption(*UEnum::GetValueAsString<ESpawnSettings>(_movementToTargetSettings.target));
	arenaPointNameCB->SetSelectedOption(_movementToTargetSettings.arenaPointName);
}
FMovementToTargetSettings UEditor_PB_ToTargetWidget::GetToTargetSettings() const
{
	FMovementToTargetSettings _toTargetSettings;
	_toTargetSettings.iRepeat = repeatSP->GetValue();
	_toTargetSettings.fMovementSpeed = movementSpeedSP->GetValue();
	_toTargetSettings.target =  TEnumAsByte<ESpawnSettings>(UMiscLibrary::GetEnumValueFromString(targetCB->GetSelectedOption(), "ESpawnSettings"));
	_toTargetSettings.arenaPointName = arenaPointNameCB->GetSelectedOption();
	return _toTargetSettings;
}

void UEditor_PB_ToTargetWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}
void UEditor_PB_ToTargetWidget::InitializeWidget()
{
	targetCB->OnSelectionChanged.AddUniqueDynamic(this, &UEditor_PB_ToTargetWidget::UpdateArenaPointVisibility);
	
	UMiscLibrary::InitComboBoxFromEnum("ESpawnSettings",  targetCB);	
}

void UEditor_PB_ToTargetWidget::UpdateArenaPointVisibility(FString _option, ESelectInfo::Type _selectInfo)
{
	const ESpawnSettings& _value = TEnumAsByte<ESpawnSettings>(UMiscLibrary::GetEnumValueFromString(_option, "ESpawnSettings")); 

	arenaPointNameHB->SetVisibility(_value == ESpawnSettings::ARENA_POINT ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
#pragma endregion

#pragma region Movement Teleport
void UEditor_PB_TeleportWidget::InitializeTeleportWidget(const FMovementTeleportSettings& _movementTeleportSettings)
{
	xOffsetSP->SetValue(_movementTeleportSettings.vOffset.X);
	yOffsetSP->SetValue(_movementTeleportSettings.vOffset.Y);
	zOffsetSP->SetValue(_movementTeleportSettings.vOffset.Z);
	repeatSP->SetValue(_movementTeleportSettings.iRepeat);
	repeatRateSP->SetValue(_movementTeleportSettings.fRepeatRate);
}
FMovementTeleportSettings UEditor_PB_TeleportWidget::GetTeleportSettings() const
{
	FMovementTeleportSettings _teleportSettings;
	_teleportSettings.vOffset = FVector(xOffsetSP->GetValue(), yOffsetSP->GetValue(), zOffsetSP->GetValue());
	_teleportSettings.iRepeat = repeatSP->GetValue();
	_teleportSettings.fRepeatRate = repeatRateSP->GetValue();
	return _teleportSettings;
}
#pragma endregion

#pragma region Movement
void UEditor_PB_MovementWidget::InitializeMovementWidget(const FPatternData& _patternData, UA_DT_ArenaDataTable* _arenaDataTable)
{
	arenaDataTable = _arenaDataTable;
	
	InitializeVisibility(_patternData);
	
	onAxeWidget->InitializeOnAxeWidget(_patternData.GetMovementOnAxeSettings());
	toTargetWidget->InitializeToTargetWidgetUtil(arenaDataTable);
	toTargetWidget->InitializeToTargetWidget(_patternData.GetMovementToTargetSettings());
	teleportWidget->InitializeTeleportWidget(_patternData.GetMovementTeleportSettings());
}
void UEditor_PB_MovementWidget::GetMovementOverride(FPatternData& _patternData)
{
	_patternData.SetMovementOnAxeSettings(onAxeWidget->GetOnAxeSettings());
	_patternData.SetMovementToTargetSettings(toTargetWidget->GetToTargetSettings());
	_patternData.SetMovementTeleportSettings(teleportWidget->GetTeleportSettings());
}

void UEditor_PB_MovementWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidgets();
}
void UEditor_PB_MovementWidget::InitializeWidgets()
{
	if (onAxeWidgetType.Get())
	{
		onAxeWidget = CreateWidget<UEditor_PB_OnAxeWidget>(this, onAxeWidgetType);
		movementVB->AddChildToVerticalBox(onAxeWidget);
	}
	else
		LOG("Movement Widget => On Axe Widget Type is not valid.")
	
	if (toTargetWidgetType.Get())
	{
		toTargetWidget = CreateWidget<UEditor_PB_ToTargetWidget>(this, toTargetWidgetType);
		movementVB->AddChildToVerticalBox(toTargetWidget);
	}
	else
		LOG("Movement Widget => To Target Widget Type is not valid.")
	
	if (teleportWidgetType.Get())
	{
		teleportWidget = CreateWidget<UEditor_PB_TeleportWidget>(this, teleportWidgetType);
		movementVB->AddChildToVerticalBox(teleportWidget);
	}
	else
		LOG("Movement Widget => Teleport Widget Type is not valid.")
}

void UEditor_PB_MovementWidget::InitializeVisibility(const FPatternData& _patternData)
{
	const bool& _onAxe = _patternData.GetMovementOnAxeState();
	const bool& _toTarget = _patternData.GetMovementToTargetState();
	const bool& _teleport = _patternData.GetMovementTeleportState();
	
	UpdateWidget(onAxeWidget, _onAxe);
	UpdateWidget(toTargetWidget, _toTarget);
	UpdateWidget(teleportWidget, _teleport);

	UMiscLibrary::SetWidgetVisibility(this, _onAxe || _toTarget || _teleport);
}
void UEditor_PB_MovementWidget::UpdateWidget(UWidget* _widget, const bool& _visible) const
{
	UMiscLibrary::SetWidgetVisibility(_widget, _visible);
	if (_visible)
		movementVB->AddChildToVerticalBox(_widget);
	else
		movementVB->RemoveChild(_widget);
}

#pragma endregion
#pragma endregion

#pragma region Projectile
#pragma region Projectile Guided
void UEditor_ProjectileGuidedWidget::InitializeProjectileGuidedWidget(const FProjectileGuidedSettings& _settings)
{
	guidedSpeedSP->SetValue(_settings.fProjectileGuidedSpeed);
	rotationSpeedSP->SetValue(_settings.fRotationSpeed);
	rangeSP->SetValue(_settings.fRangeGuided);
	waitTimeSP->SetValue(_settings.fWaitTime);
}
void UEditor_ProjectileGuidedWidget::SetEditGuided(const bool& _edit)
{
	UMiscLibrary::SetCheckBoxState(editCB, _edit);
	EditChanged(_edit);
}

FProjectileGuidedSettings UEditor_ProjectileGuidedWidget::GetProjectileGuidedSettings() const
{
	FProjectileGuidedSettings _settings;
	_settings.fProjectileGuidedSpeed = guidedSpeedSP->GetValue();
	_settings.fRotationSpeed = rotationSpeedSP->GetValue();
	_settings.fRangeGuided = rangeSP->GetValue();
	_settings.fWaitTime = waitTimeSP->GetValue();
	return _settings;
}
bool UEditor_ProjectileGuidedWidget::GetEditGuided() const
{
	return UMiscLibrary::GetCheckBoxValue(editCB);
}

void UEditor_ProjectileGuidedWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}
void UEditor_ProjectileGuidedWidget::InitializeWidget()
{
	editCB->OnCheckStateChanged.AddUniqueDynamic(this, &UEditor_ProjectileGuidedWidget::EditChanged);
}

void UEditor_ProjectileGuidedWidget::EditChanged(bool _editState)
{
	UMiscLibrary::SetWidgetVisibility(settingsVB, _editState);
}
#pragma endregion 

#pragma region Projectile Simulated
void UEditor_ProjectileSimulatedWidget::InitializeProjectileSimulatedWidget(const FProjectileSimulatedSettings& _settings)
{
	minScaleSP->SetValue(_settings.fGravityScaleMin);
	maxScaleSP->SetValue(_settings.fGravityScaleMax);
}
void UEditor_ProjectileSimulatedWidget::SetEditSimulated(const bool& _edit)
{
	UMiscLibrary::SetCheckBoxState(editCB, _edit);
	EditChanged(_edit);
}

FProjectileSimulatedSettings UEditor_ProjectileSimulatedWidget::GetProjectileSimulatedSettings() const
{
	FProjectileSimulatedSettings _settings;
	_settings.fGravityScaleMin = minScaleSP->GetValue();
	_settings.fGravityScaleMax = maxScaleSP->GetValue();
	return _settings;
}
bool UEditor_ProjectileSimulatedWidget::GetEditSimulated() const
{
	return UMiscLibrary::GetCheckBoxValue(editCB);
}

void UEditor_ProjectileSimulatedWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}
void UEditor_ProjectileSimulatedWidget::InitializeWidget()
{
	editCB->OnCheckStateChanged.AddUniqueDynamic(this, &UEditor_ProjectileSimulatedWidget::EditChanged);
}

void UEditor_ProjectileSimulatedWidget::EditChanged(bool _editState)
{
	UMiscLibrary::SetWidgetVisibility(settingsVB, _editState);
}
#pragma endregion 

#pragma region Projectile Ballistic
void UEditor_ProjectileBallisticWidget::InitializeProjectileBallisticWidget(const FProjectileBallisticSettings& _settings)
{
	zOffsetSP->SetValue(_settings.fOffsetZ);	
	rangeSP->SetValue(_settings.fRangeBallistic);	
}

void UEditor_ProjectileBallisticWidget::SetEditBallistic(const bool& _edit)
{
	UMiscLibrary::SetCheckBoxState(editCB, _edit);
	EditChanged(_edit);
}

FProjectileBallisticSettings UEditor_ProjectileBallisticWidget::GetProjectileBallisticSettings() const
{
	FProjectileBallisticSettings _settings;
	_settings.fOffsetZ = zOffsetSP->GetValue();
	_settings.fRangeBallistic = rangeSP->GetValue();
	return _settings;
}

bool UEditor_ProjectileBallisticWidget::GetEditBallistic() const
{
	return UMiscLibrary::GetCheckBoxValue(editCB);
}

void UEditor_ProjectileBallisticWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}
void UEditor_ProjectileBallisticWidget::InitializeWidget()
{
	editCB->OnCheckStateChanged.AddUniqueDynamic(this, &UEditor_ProjectileBallisticWidget::EditChanged);
}

void UEditor_ProjectileBallisticWidget::EditChanged(bool _editState)
{
	UMiscLibrary::SetWidgetVisibility(settingsVB, _editState);
}
#pragma endregion 

#pragma region Projectile Sinusoidal
void UEditor_ProjectileSinusoidalWidget::InitializeProjectileSinusoidalWidget(const FProjectileSinusoidalSettings& _settings)
{
	periodRangeSP->SetValue(_settings.fPeriodRange);
	amplitudeSP->SetValue(_settings.fAmplitude);
	startGapSP->SetValue(_settings.fStartGap);
	UMiscLibrary::SetSpinBoxFromRotator(xRotation, yRotation, zRotation, _settings.localRotation);
}
void UEditor_ProjectileSinusoidalWidget::SetEditSinusoidal(const bool& _editState)
{
	UMiscLibrary::SetCheckBoxState(editCB, _editState);
}

FProjectileSinusoidalSettings UEditor_ProjectileSinusoidalWidget::GetProjectileSinusoidalSettings() const
{
	FProjectileSinusoidalSettings _settings;
	_settings.fPeriodRange = periodRangeSP->GetValue();
	_settings.fAmplitude = amplitudeSP->GetValue();
	_settings.fStartGap = startGapSP->GetValue();
	_settings.localRotation = UMiscLibrary::GetRotatorFromSpinBox(xRotation, yRotation, zRotation);
	return _settings;
}
bool UEditor_ProjectileSinusoidalWidget::GetEditSinusoidal() const
{
	return UMiscLibrary::GetCheckBoxValue(editCB);
}

void UEditor_ProjectileSinusoidalWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}
void UEditor_ProjectileSinusoidalWidget::InitializeWidget()
{
	editCB->OnCheckStateChanged.AddUniqueDynamic(this, &UEditor_ProjectileSinusoidalWidget::EditChanged);
}

void UEditor_ProjectileSinusoidalWidget::EditChanged(bool _editState)
{
	UMiscLibrary::SetWidgetVisibility(settingsVB, _editState);
}
#pragma endregion 

#pragma region Projectile Global
void UEditor_ProjectileWidget::InitializeProjectileWidget(const FProjectileSettings& _settings, const TEnumAsByte<EProjectileType>& _projectileType)
{	
	const FProjectileGlobalSettings& _globalSettings = _settings.globalSettings;
	damageSP->SetValue(_globalSettings.fDamage);
	speedSP->SetValue(_globalSettings.fProjectileSpeed);

	projectileGuidedWidget->InitializeProjectileGuidedWidget(_settings.guidedSettings);
	projectileGuidedWidget->SetEditGuided(_settings.bEditGuidedSettings);
	
	projectileSimulatedWidget->InitializeProjectileSimulatedWidget(_settings.simulatedSettings);
	projectileSimulatedWidget->SetEditSimulated(_settings.bEditSimulatedSettings);
	
	projectileBallisticWidget->InitializeProjectileBallisticWidget(_settings.ballisticSettings);
	projectileBallisticWidget->SetEditBallistic(_settings.bEditBallisticSettings);
	
	projectileSinusoidalWidget->InitializeProjectileSinusoidalWidget(_settings.sinusoidalSettings);
	projectileSinusoidalWidget->SetEditSinusoidal(_settings.bEditSinusoidalSettings);
	
	const bool& _editGlobal = _settings.bEditGlobalSettings;
	UMiscLibrary::SetCheckBoxState(editCB, _editGlobal);
	EditChanged(_editGlobal);

	UpdateWidget(projectileGuidedWidget, _projectileType == EProjectileType::Guided);
	UpdateWidget(projectileSimulatedWidget, _projectileType == EProjectileType::Simulated);
	UpdateWidget(projectileBallisticWidget, _projectileType == EProjectileType::Ballistic);
	UpdateWidget(projectileSinusoidalWidget, _projectileType == EProjectileType::Sinusoidal);
}
FProjectileSettings UEditor_ProjectileWidget::GetProjectileSettings() const
{
	FProjectileSettings _settings;
	
	FProjectileGlobalSettings _global;
	_global.fDamage = damageSP->GetValue();
	_global.fProjectileSpeed = speedSP->GetValue();
	
	_settings.globalSettings = _global;
	_settings.bEditGlobalSettings = UMiscLibrary::GetCheckBoxValue(editCB);
	
	_settings.guidedSettings = projectileGuidedWidget->GetProjectileGuidedSettings();
	_settings.bEditGuidedSettings = projectileGuidedWidget->GetEditGuided();
	
	_settings.simulatedSettings = projectileSimulatedWidget->GetProjectileSimulatedSettings();
	_settings.bEditSimulatedSettings = projectileSimulatedWidget->GetEditSimulated();
	
	_settings.ballisticSettings = projectileBallisticWidget->GetProjectileBallisticSettings();
	_settings.bEditBallisticSettings = projectileBallisticWidget->GetEditBallistic();
	
	_settings.sinusoidalSettings = projectileSinusoidalWidget->GetProjectileSinusoidalSettings();
	_settings.bEditSinusoidalSettings = projectileSinusoidalWidget->GetEditSinusoidal();
	
	return _settings;
}

void UEditor_ProjectileWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
	InitializeWidgets();
}

void UEditor_ProjectileWidget::InitializeWidget()
{
	editCB->OnCheckStateChanged.AddUniqueDynamic(this, &UEditor_ProjectileWidget::EditChanged);
}
void UEditor_ProjectileWidget::InitializeWidgets()
{
	if (projectileGuidedWidgetType.Get())
	{
		projectileGuidedWidget = CreateWidget<UEditor_ProjectileGuidedWidget>(this, projectileGuidedWidgetType);
		specificSettingsVB->AddChildToVerticalBox(projectileGuidedWidget);
	}
	else
		LOG("UEditor_ProjectileWidget::InitializeWidgets => Projectile Guided Widget Type is not valid.");

	if (projectileSimulatedWidgetType.Get())
	{
		projectileSimulatedWidget = CreateWidget<UEditor_ProjectileSimulatedWidget>(this, projectileSimulatedWidgetType);
		specificSettingsVB->AddChildToVerticalBox(projectileSimulatedWidget);
	}
	else
		LOG("UEditor_ProjectileWidget::InitializeWidgets => Projectile Simulated Widget Type is not valid.");

	if (projectileBallisticWidgetType.Get())
	{
		projectileBallisticWidget = CreateWidget<UEditor_ProjectileBallisticWidget>(this, projectileBallisticWidgetType);
		specificSettingsVB->AddChildToVerticalBox(projectileBallisticWidget);
	}
	else
		LOG("UEditor_ProjectileWidget::InitializeWidgets => Projectile Ballistic Widget Type is not valid.");
	
	if (projectileSinusoidalWidgetType.Get())
	{
		projectileSinusoidalWidget = CreateWidget<UEditor_ProjectileSinusoidalWidget>(this, projectileSinusoidalWidgetType);
		specificSettingsVB->AddChildToVerticalBox(projectileSinusoidalWidget);
	}
	else
		LOG("UEditor_ProjectileWidget::InitializeWidgets => Projectile Sinusoidal Widget Type is not valid.");
	
}

void UEditor_ProjectileWidget::UpdateWidget(UWidget* _widget, const bool& _state)
{
	if (_state)
		specificSettingsVB->AddChildToVerticalBox(_widget);
	else
		specificSettingsVB->RemoveChild(_widget);
}

void UEditor_ProjectileWidget::EditChanged(bool _editState)
{
	UMiscLibrary::SetWidgetVisibility(settingsVB, _editState);
}
#pragma endregion 
#pragma endregion 

#pragma region PB Shoot
#pragma region ShootPoint
void UEditor_PB_ShootPointWidget::InitializeShootPointWidget(const FShootPointSettings& _shootSettings)
{
	radiusSP->SetValue(_shootSettings.fRadius);
}
FShootPointSettings UEditor_PB_ShootPointWidget::GetShootPointSettings() const
{
	FShootPointSettings _pointSettings;
	_pointSettings.fRadius = radiusSP->GetValue();
	return _pointSettings;
}
#pragma endregion

#pragma region ShootLine
void UEditor_PB_ShootLineWidget::InitializeShootLineWidget(const FShootLineSettings& _shootSettings)
{
	lengthSP->SetValue(_shootSettings.fLength);
	planCB->SetSelectedOption(*UEnum::GetValueAsString<EPlan>(_shootSettings.ePlan));
}
FShootLineSettings UEditor_PB_ShootLineWidget::GetShootLineSettings() const
{
	FShootLineSettings _lineSettings;
	_lineSettings.fLength = lengthSP->GetValue();
	_lineSettings.ePlan = TEnumAsByte<EPlan>(UMiscLibrary::GetEnumValueFromString(planCB->GetSelectedOption(), "EPlan"));
	return _lineSettings;
}

void UEditor_PB_ShootLineWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}
void UEditor_PB_ShootLineWidget::InitializeWidget()
{
	UMiscLibrary::InitComboBoxFromEnum("EPlan", planCB);
}
#pragma endregion

#pragma region ShootArc
void UEditor_PB_ShootArcWidget::InitializeShootArcWidget(const FShootArcSettings& _shootSettings)
{
	UMiscLibrary::SetCheckBoxState(keepPatternForwardCB, _shootSettings.bKeepPatternForward);
	radiusSP->SetValue(_shootSettings.fRadius);
	angleSP->SetValue(_shootSettings.fAngle);
}
FShootArcSettings UEditor_PB_ShootArcWidget::GetShootArcSettings() const
{
	FShootArcSettings _arcSettings;
	_arcSettings.bKeepPatternForward = UMiscLibrary::GetCheckBoxValue(keepPatternForwardCB);
	_arcSettings.fRadius = radiusSP->GetValue();
	_arcSettings.fAngle = angleSP->GetValue();
	return _arcSettings;
}
#pragma endregion

#pragma region ShootCircle
void UEditor_PB_ShootCircleWidget::InitializeShootCircleWidget(const FShootCircleSettings& _shootSettings)
{
	radiusSP->SetValue(_shootSettings.fRadius);
}
FShootCircleSettings UEditor_PB_ShootCircleWidget::GetShootCircleSettings() const
{
	FShootCircleSettings _circleSettings;
	_circleSettings.fRadius = radiusSP->GetValue();
	return _circleSettings;
}
#pragma endregion

#pragma region ShootCross
void UEditor_PB_ShootCrossWidget::InitializeShootCrossWidget(const FShootCrossSettings& _shootSettings)
{
	UMiscLibrary::SetCheckBoxState(keepPatternForwardCB, _shootSettings.bKeepPatternForward);
	radiusSP->SetValue(_shootSettings.fRadius);
	xAngleSP->SetValue(_shootSettings.fAngleX);
	yAngleSP->SetValue(_shootSettings.fAngleY);
	xProjectileSP->SetValue(_shootSettings.iProjectileOnX);
	yProjectileSP->SetValue(_shootSettings.iProjectileOnY);
}
FShootCrossSettings UEditor_PB_ShootCrossWidget::GetShootCrossSettings() const
{
	FShootCrossSettings _crossSettings;
	_crossSettings.bKeepPatternForward = UMiscLibrary::GetCheckBoxValue(keepPatternForwardCB);
	_crossSettings.fAngleX = xAngleSP->GetValue();
	_crossSettings.fAngleY = yAngleSP->GetValue();
	_crossSettings.iProjectileOnX = xProjectileSP->GetValue();
	_crossSettings.iProjectileOnY = yProjectileSP->GetValue();
	return _crossSettings;	
}
#pragma endregion

#pragma region Random2D
void UEditor_PB_ShootRandom2DWidget::InitializeShootRandom2DWidget(const FShootRandom2DSettings& _shootSettings)
{
	planCB->SetSelectedOption(*UEnum::GetValueAsString<EPlan>(_shootSettings.ePlan));
	angleSP->SetValue(_shootSettings.fAngle);
}
FShootRandom2DSettings UEditor_PB_ShootRandom2DWidget::GetShootRandom2DSettings() const
{
	FShootRandom2DSettings _random2DSettings;
	_random2DSettings.ePlan = TEnumAsByte<EPlan>(UMiscLibrary::GetEnumValueFromString(planCB->GetSelectedOption(), "EPlan"));
	_random2DSettings.fAngle = angleSP->GetValue();
	return _random2DSettings;
}

void UEditor_PB_ShootRandom2DWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}
void UEditor_PB_ShootRandom2DWidget::InitializeWidget()
{
	UMiscLibrary::InitComboBoxFromEnum("EPlan", planCB);
}
#pragma endregion

#pragma region Random3D
void UEditor_PB_ShootRandom3DWidget::InitializeShootRandom3DWidget(const FShootRandom3DSettings& _shootSettings)
{
	angleSP->SetValue(_shootSettings.fAngle);	
}
FShootRandom3DSettings UEditor_PB_ShootRandom3DWidget::GetShootRandom3DSettings() const
{
	FShootRandom3DSettings _random3DSettings;
	_random3DSettings.fAngle = angleSP->GetValue();
	return _random3DSettings;
}
#pragma endregion 

#pragma region Shoot 
void UEditor_PB_ShootWidget::InitializeShootWidget(const FPatternData& _patternData)
{
	InitializeVisibility(_patternData);	
	
	const FShootSettings& _shootSettings = _patternData.GetShootSettings();
	shootingModeCB->SetSelectedOption(*UEnum::GetValueAsString<EShootingMode>(_shootSettings.eShootingMode));
	spawnAtOnceSP->SetValue(_shootSettings.iSpawnAtOnce);
	fireQtySP->SetValue(_shootSettings.iFireQuantity);
	fireDurationSP->SetValue(_shootSettings.fFireDuration);
	fireRateSP->SetValue(_shootSettings.fFireRate);
	
	pointWidget->InitializeShootPointWidget(_patternData.GetShootPointSettings());
	lineWidget->InitializeShootLineWidget(_patternData.GetShootLineSettings());
	arcWidget->InitializeShootArcWidget(_patternData.GetShootArcSettings());
	circleWidget->InitializeShootCircleWidget(_patternData.GetShootCircleSettings());
	crossWidget->InitializeShootCrossWidget(_patternData.GetShootCrossSettings());
	random2DWidget->InitializeShootRandom2DWidget(_patternData.GetShootRandom2DSettings());
	random3DWidget->InitializeShootRandom3DWidget(_patternData.GetShootRandom3DSettings());

	projectileWidget->InitializeProjectileWidget(_patternData.GetProjectileShootSettings(), _shootSettings.projectileType);

	ShootModeChanged(shootingModeCB->GetSelectedOption(), ESelectInfo::Direct);
}
void UEditor_PB_ShootWidget::GetShootOverride(FPatternData& _patternData) const
{
	FShootSettings _shootSettings;
	_shootSettings.eShootingMode = TEnumAsByte<EShootingMode>(UMiscLibrary::GetEnumValueFromString(shootingModeCB->GetSelectedOption(), "EShootingMode"));
	_shootSettings.iSpawnAtOnce = spawnAtOnceSP->GetValue();
	_shootSettings.iFireQuantity = fireQtySP->GetValue();
	_shootSettings.fFireDuration = fireDurationSP->GetValue();
	_shootSettings.fFireRate = fireRateSP->GetValue();
	
	_patternData.SetShootSettings(_shootSettings);
	_patternData.SetShootPointSettings(pointWidget->GetShootPointSettings());
	_patternData.SetShootLineSettings(lineWidget->GetShootLineSettings());
	_patternData.SetShootArcSettings(arcWidget->GetShootArcSettings());
	_patternData.SetShootCircleSettings(circleWidget->GetShootCircleSettings());
	_patternData.SetShootCrossSettings(crossWidget->GetShootCrossSettings());
	_patternData.SetShootRandom2DSettings(random2DWidget->GetShootRandom2DSettings());
	_patternData.SetShootRandom3DSettings(random3DWidget->GetShootRandom3DSettings());

	_patternData.SetProjectileShootSettings(projectileWidget->GetProjectileSettings());
}

void UEditor_PB_ShootWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
	InitializeWidgets();
}
void UEditor_PB_ShootWidget::InitializeWidget()
{
	UMiscLibrary::InitComboBoxFromEnum("EShootingMode", shootingModeCB);
	shootingModeCB->OnSelectionChanged.AddUniqueDynamic(this, &UEditor_PB_ShootWidget::ShootModeChanged);
}
void UEditor_PB_ShootWidget::InitializeWidgets()
{
	if (pointWidgetType.Get())
	{
		pointWidget = CreateWidget<UEditor_PB_ShootPointWidget>(this, pointWidgetType);
		shootVB->AddChildToVerticalBox(pointWidget);
	}
	else
		LOG("Shoot Widget => Point Widget Type is not valid.")

	if (lineWidgetType.Get())
	{
		lineWidget = CreateWidget<UEditor_PB_ShootLineWidget>(this, lineWidgetType);
		shootVB->AddChildToVerticalBox(lineWidget);
	}
	else
		LOG("Shoot Widget => Line Widget Type is not valid.")

	if (arcWidgetType.Get())
	{
		arcWidget = CreateWidget<UEditor_PB_ShootArcWidget>(this, arcWidgetType);
		shootVB->AddChildToVerticalBox(arcWidget);
	}
	else
		LOG("Shoot Widget => Arc Widget Type is not valid.")

	if (circleWidgetType.Get())
	{
		circleWidget = CreateWidget<UEditor_PB_ShootCircleWidget>(this, circleWidgetType);
		shootVB->AddChildToVerticalBox(circleWidget);
	}
	else
		LOG("Shoot Widget => Circle Widget Type is not valid.")

	if (crossWidgetType.Get())
	{
		crossWidget = CreateWidget<UEditor_PB_ShootCrossWidget>(this, crossWidgetType);
		shootVB->AddChildToVerticalBox(crossWidget);
	}
	else
		LOG("Shoot Widget => Cross Widget Type is not valid.")

	if (random2DWidgetType.Get())
	{
		random2DWidget = CreateWidget<UEditor_PB_ShootRandom2DWidget>(this, random2DWidgetType);
		shootVB->AddChildToVerticalBox(random2DWidget);
	}
	else
		LOG("Shoot Widget => Random2D Widget Type is not valid.")

	if (random3DWidgetType.Get())
	{
		random3DWidget = CreateWidget<UEditor_PB_ShootRandom3DWidget>(this, random3DWidgetType);
		shootVB->AddChildToVerticalBox(random3DWidget);
	}
	else
		LOG("Shoot Widget => Random3D Widget Type is not valid.")

	if (projectileWidgetType.Get())
	{
		projectileWidget = CreateWidget<UEditor_ProjectileWidget>(this, projectileWidgetType);
		projectileVB->AddChildToVerticalBox(projectileWidget);
	}
	else
		LOG("Shoot Widget => Projectile Widget Type is not valid.")
}

void UEditor_PB_ShootWidget::InitializeVisibility(const FPatternData& _patternData)
{
	const bool& _shootPoint = _patternData.GetShootPointState();
	const bool& _shootLine = _patternData.GetShootLineState();
	const bool& _shootArc = _patternData.GetShootArcState();
	const bool& _shootCircle = _patternData.GetShootCircleState();
	const bool& _shootCross = _patternData.GetShootCrossState();
	const bool& _shootRandom2D = _patternData.GetShootRandom2DState();
	const bool& _shootRandom3D = _patternData.GetShootRandom3DState();
	
	UpdateWidget(pointWidget, _shootPoint);
	UpdateWidget(lineWidget, _shootLine);
	UpdateWidget(arcWidget, _shootArc);
	UpdateWidget(circleWidget, _shootCircle);
	UpdateWidget(crossWidget, _shootCross);
	UpdateWidget(random2DWidget, _shootRandom2D);
	UpdateWidget(random3DWidget, _shootRandom3D);

	UMiscLibrary::SetWidgetVisibility(this, _shootPoint || _shootLine || _shootArc || _shootCircle || _shootCross || _shootRandom2D || _shootRandom3D);
}
void UEditor_PB_ShootWidget::UpdateWidget(UWidget* _widget, const bool& _visible) const
{
	UMiscLibrary::SetWidgetVisibility(_widget, _visible);
	if (_visible)
		shootVB->AddChildToVerticalBox(_widget);
	else
		shootVB->RemoveChild(_widget);
}

void UEditor_PB_ShootWidget::ShootModeChanged(FString _option, ESelectInfo::Type _selectionType)
{
	const EShootingMode& _mode = static_cast<EShootingMode>(UMiscLibrary::GetEnumValueFromString(_option, "EShootingMode"));
	if (_mode == EShootingMode::DURATION)
	{
		UMiscLibrary::SetWidgetVisibility(fireDurationHB, true);
		UMiscLibrary::SetWidgetVisibility(fireQtyHB, false);
	}
	if (_mode == EShootingMode::QUANTITY)
	{
		UMiscLibrary::SetWidgetVisibility(fireQtyHB, true);
		UMiscLibrary::SetWidgetVisibility(fireDurationHB, false);		
	}		
}
#pragma endregion
#pragma endregion

#pragma region PB Laser
void UEditor_PB_LaserWidget::InitializeLaserWidget(const FPatternData& _patternData)
{
	const FLaserSettings _laserSettings = _patternData.GetLaserSettings();
	pivotCB->SetSelectedOption(*UEnum::GetValueAsString<EPivot>(_laserSettings.laserPivot));
	damageSP->SetValue(_laserSettings.fLaserDamage);
	lengthSP->SetValue(_laserSettings.fLaserLength);
	radiusSP->SetValue(_laserSettings.fLaserRadius);
	durationSP->SetValue(_laserSettings.fLaserDuration);
}
void UEditor_PB_LaserWidget::GetLaserOverride(FPatternData& _patternData)
{
	FLaserSettings _laserSettings;
	_laserSettings.laserPivot = TEnumAsByte<EPivot>(UMiscLibrary::GetEnumValueFromString(pivotCB->GetSelectedOption(), "EPivot"));
	_laserSettings.fLaserDamage = damageSP->GetValue();
	_laserSettings.fLaserLength = lengthSP->GetValue();
	_laserSettings.fLaserRadius = radiusSP->GetValue();
	_laserSettings.fLaserDuration = durationSP->GetValue();
	_patternData.SetLaserSettings(_laserSettings);	
}

void UEditor_PB_LaserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}
void UEditor_PB_LaserWidget::InitializeWidget()
{
	UMiscLibrary::InitComboBoxFromEnum("EPivot", pivotCB);
}
#pragma endregion

#pragma region PB Wave
void UEditor_PB_WaveWidget::InitializeWaveWidget(const FPatternData& _patternData)
{
	const FWaveSettings& _waveSettings = _patternData.GetWaveSettings();
	speedSP->SetValue(_waveSettings.fWaveSpeed);
	durationSP->SetValue(_waveSettings.fWaveDuration);
	damageSP->SetValue(_waveSettings.fWaveDamage);
}
void UEditor_PB_WaveWidget::GetWaveOverride(FPatternData& _patternData)
{
	FWaveSettings _waveSettings;
	_waveSettings.fWaveSpeed = speedSP->GetValue();
	_waveSettings.fWaveDuration = durationSP->GetValue();
	_waveSettings.fWaveDamage = damageSP->GetValue();
	_patternData.SetWaveSettings(_waveSettings);
}
#pragma endregion

#pragma region PB Zone
#pragma region Scaling
void UEditor_PB_ScalingWidget::InitializeScalingWidget(const FScalingSettings& _scalingSettings)
{
	UMiscLibrary::SetCheckBoxState(useTimeCB, _scalingSettings.bUseTime);
	UMiscLibrary::SetCheckBoxState(useGoalCB, _scalingSettings.bUseGoal);
	UMiscLibrary::SetCheckBoxState(endPatternCB, _scalingSettings.bEndPatternOnCompleteScale);
	timeBeforeActiveSP->SetValue(_scalingSettings.fTimeBeforeActive);
	scaleTimeSP->SetValue(_scalingSettings.fScaleTime);
	scaleSpeedSP->SetValue(_scalingSettings.fScaleSpeed);
	persistentTimeSP->SetValue(_scalingSettings.fPersistantTime);
	UMiscLibrary::SetSpinBoxFromVector(xgoalScaleSP, ygoalScaleSP, zgoalScaleSP, _scalingSettings.vGoalScale);
	UMiscLibrary::SetSpinBoxFromVector(xDefaultScaleSP, yDefaultScaleSP, zDefaultScaleSP, _scalingSettings.vDefaultScale);


	scalingVB->RemoveChild(goalScaleHB);
	scalingVB->RemoveChild(scaleTimeHB);
	
	if (const bool& _useTime = UMiscLibrary::GetCheckBoxValue(useTimeCB))
		UseTimeChanged(_useTime);
	if (const bool& _useGoal = UMiscLibrary::GetCheckBoxValue(useGoalCB))
		UseGoalChanged(_useGoal);
}
FScalingSettings UEditor_PB_ScalingWidget::GetScalingSettings() const
{
	FScalingSettings _scalingSettings;
	_scalingSettings.bUseTime = UMiscLibrary::GetCheckBoxValue(useTimeCB);
	_scalingSettings.bUseGoal = UMiscLibrary::GetCheckBoxValue(useGoalCB);
	_scalingSettings.bEndPatternOnCompleteScale = UMiscLibrary::GetCheckBoxValue(endPatternCB);
	_scalingSettings.fTimeBeforeActive = timeBeforeActiveSP->GetValue();
	_scalingSettings.fScaleTime = scaleTimeSP->GetValue();
	_scalingSettings.fScaleSpeed = scaleSpeedSP->GetValue();
	_scalingSettings.fPersistantTime = persistentTimeSP->GetValue();
	_scalingSettings.vDefaultScale = UMiscLibrary::GetVectorFromSpinBox(xDefaultScaleSP, yDefaultScaleSP, zDefaultScaleSP);
	_scalingSettings.vGoalScale = UMiscLibrary::GetVectorFromSpinBox(xgoalScaleSP, ygoalScaleSP, ygoalScaleSP);
	return _scalingSettings;
}

void UEditor_PB_ScalingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}
void UEditor_PB_ScalingWidget::InitializeWidget()
{
	useGoalCB->OnCheckStateChanged.AddUniqueDynamic(this, &UEditor_PB_ScalingWidget::UseGoalChanged);
	useTimeCB->OnCheckStateChanged.AddUniqueDynamic(this, &UEditor_PB_ScalingWidget::UseTimeChanged);
}

void UEditor_PB_ScalingWidget::UseTimeChanged(bool _state)
{
	scalingVB->RemoveChild(useGoalHB);
	scalingVB->RemoveChild(useTimeHB);
	scalingVB->RemoveChild(goalScaleHB);
	scalingVB->RemoveChild(scaleTimeHB);
	
	if (_state)
	{
		scalingVB->AddChildToVerticalBox(useTimeHB);
		scalingVB->AddChildToVerticalBox(scaleTimeHB);
	}
	else
	{
		scalingVB->AddChildToVerticalBox(useTimeHB);		
		scalingVB->AddChildToVerticalBox(useGoalHB);		
	}
}
void UEditor_PB_ScalingWidget::UseGoalChanged(bool _state)
{
	scalingVB->RemoveChild(useGoalHB);
	scalingVB->RemoveChild(useTimeHB);
	scalingVB->RemoveChild(goalScaleHB);
	scalingVB->RemoveChild(scaleTimeHB);
	
	if (_state)
	{
		scalingVB->AddChildToVerticalBox(useGoalHB);
		scalingVB->AddChildToVerticalBox(goalScaleHB);
	}
	else
	{
		scalingVB->AddChildToVerticalBox(useTimeHB);		
		scalingVB->AddChildToVerticalBox(useGoalHB);
	}
}
#pragma endregion

#pragma region Spawner Default
void UEditor_PB_SpawnerDefaultWidget::InitializeSpawnerDefaultWidget(const FSpawnerDefaultSettings& _spawnerSettings)
{
	spawnQtySP->SetValue(_spawnerSettings.iSpawnQty);
}
FSpawnerDefaultSettings UEditor_PB_SpawnerDefaultWidget::GetSpawnerDefaultSettings() const
{
	FSpawnerDefaultSettings _spawnerSettings;
	_spawnerSettings.iSpawnQty = spawnQtySP->GetValue();
	return _spawnerSettings;
}
#pragma endregion

#pragma region Concentric Circle
void UEditor_ConcentricCircleWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}
void UEditor_ConcentricCircleWidget::InitializeWidget()
{
	removeButton->OnClicked.AddUniqueDynamic(this, &UEditor_ConcentricCircleWidget::RemoveElement);
}

void UEditor_ConcentricCircleWidget::RemoveElement()
{
	onRemoveElement.Broadcast(index);
}

void UEditor_ConcentricCircleWidget::InitializeConcentricCircleWidget(const int& _index, const FConcentricCircleSettings& _settings)
{
	SetElementIndex(_index);
	quantitySP->SetValue(_settings.quantity);
	spacingSP->SetValue(_settings.spacing);
}
FConcentricCircleSettings UEditor_ConcentricCircleWidget::GetConcentricCircleSettings() const
{
	FConcentricCircleSettings _settings;
	_settings.quantity = quantitySP->GetValue();
	_settings.spacing = spacingSP->GetValue();
	return _settings;
}

void UEditor_ConcentricCircleWidget::SetElementIndex(const int& _index)
{
	index = _index;
	const FString& _str = FString("Circle : ") + FString::FromInt(index);
	indexText->SetText(FText::FromString(_str));
}
#pragma endregion

#pragma region Spawner Concentric
void UEditor_PB_SpawnerConcentricWidget::InitializeSpawnerConcentricWidget(const FSpawnerConcentricSettings& _spawnerSettings)
{
	InitConcentricCircleList(_spawnerSettings.circleSettings);
}
FSpawnerConcentricSettings UEditor_PB_SpawnerConcentricWidget::GetSpawnerConcentricSettings() const
{
	FSpawnerConcentricSettings _spawnerSettings;
	_spawnerSettings.circleSettings = GetConcentricCircleList();
	return _spawnerSettings;
}

void UEditor_PB_SpawnerConcentricWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}
void UEditor_PB_SpawnerConcentricWidget::InitializeWidget()
{
	addButton->OnClicked.AddUniqueDynamic(this, &UEditor_PB_SpawnerConcentricWidget::AddElementToConcentricCircleList);
	clearButton->OnClicked.AddUniqueDynamic(this, &UEditor_PB_SpawnerConcentricWidget::ClearConcentricCircleList);
	concentricCircleArea->OnExpansionChanged.AddUniqueDynamic(this, &UEditor_PB_SpawnerConcentricWidget::UpdateExpandableArea);
}

void UEditor_PB_SpawnerConcentricWidget::InitConcentricCircleList(const TArray<FConcentricCircleSettings>& _circles)
{
	ClearConcentricCircleList();

	const int& _max = _circles.Num();
	for	(int i = 0; i < _max; i++)
	{
		if (!concentricCircleWidgetType.Get())
		{
			LOG("UEditor_PB_SpawnerConcentricWidget::InitConcentricCircleList => Concentric Circle Widget Type is not valid.");
			break;
		}

		UEditor_ConcentricCircleWidget* _element = CreateWidget<UEditor_ConcentricCircleWidget>(this, concentricCircleWidgetType);
		concentricCircleListVB->AddChildToVerticalBox(_element);		
		_element->InitializeConcentricCircleWidget(i, _circles[i]);
		_element->OnRemoveElement().AddDynamic(this, &UEditor_PB_SpawnerConcentricWidget::RemoveElementFromList);
	}
}
TArray<FConcentricCircleSettings> UEditor_PB_SpawnerConcentricWidget::GetConcentricCircleList() const
{
	TArray<FConcentricCircleSettings> _circles;
	TArray<UWidget*> _children = concentricCircleListVB->GetAllChildren();

	const int& _max = _children.Num();
	for (int i = 0; i < _max; i++)
	{
		const UEditor_ConcentricCircleWidget* _element = Cast<UEditor_ConcentricCircleWidget>(_children[i]);
		if (!_element) continue;
		_circles.Add(_element->GetConcentricCircleSettings());
	}
	
	return _circles;
}

void UEditor_PB_SpawnerConcentricWidget::AddElementToConcentricCircleList()
{
	if (!concentricCircleWidgetType.Get())
	{
		LOG("UEditor_PB_SpawnerConcentricWidget::AddElementToConcentricCircleList => Concentric Circle Widget Type is not valid.");
		return;
	}

	UEditor_ConcentricCircleWidget* _element = CreateWidget<UEditor_ConcentricCircleWidget>(this, concentricCircleWidgetType);
	concentricCircleListVB->AddChildToVerticalBox(_element);
	_element->InitializeConcentricCircleWidget(concentricCircleListVB->GetChildrenCount() - 1, FConcentricCircleSettings());
	_element->OnRemoveElement().AddDynamic(this, &UEditor_PB_SpawnerConcentricWidget::RemoveElementFromList);

	UpdateExpandableArea(concentricCircleArea, concentricCircleArea->GetIsExpanded());
}
void UEditor_PB_SpawnerConcentricWidget::RemoveElementFromList(int _index)
{
	concentricCircleListVB->RemoveChildAt(_index);
	
	const TArray<UWidget*>& _children = concentricCircleListVB->GetAllChildren();
	const int& _max = _children.Num();

	for (int i = 0; i< _max; i++)
	{
		UEditor_ConcentricCircleWidget* _element = Cast<UEditor_ConcentricCircleWidget>(_children[i]);
		if (!_element) continue;
		_element->SetElementIndex(i);
	}
	
	UpdateExpandableArea(concentricCircleArea, concentricCircleArea->GetIsExpanded());
}
void UEditor_PB_SpawnerConcentricWidget::ClearConcentricCircleList()
{
	concentricCircleListVB->ClearChildren();
	UpdateExpandableArea(concentricCircleArea, concentricCircleArea->GetIsExpanded());
}

void UEditor_PB_SpawnerConcentricWidget::UpdateExpandableArea(UExpandableArea* Area, bool bIsExpanded)
{
	UCanvasPanelSlot* _slot = Cast<UCanvasPanelSlot>(Area->Slot.Get());
	if (!_slot) return;
	
	const int& _max = GetConcentricCircleList().Num() > 2 ? 2 : GetConcentricCircleList().Num();

	if (bIsExpanded)
		_slot->SetSize(FVector2D(480, _max * 150 + 50));
	else
		_slot->SetSize(FVector2D(480, 50));
}
#pragma endregion

#pragma region Multi Element Widget
void UEditor_MultiElementWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}
void UEditor_MultiElementWidget::InitializeWidget()
{
	removeButton->OnClicked.AddUniqueDynamic(this, &UEditor_MultiElementWidget::RemoveElement);
}

void UEditor_MultiElementWidget::RemoveElement()
{
	onRemoveElement.Broadcast(index);	
}

void UEditor_MultiElementWidget::InitializeMultiElementWidget(const int& _index, const int& _quantity)
{
	SetElementIndex(_index);
	quantitySP->SetValue(_quantity);
}
int UEditor_MultiElementWidget::GetMultiElementValue() const
{
	return quantitySP->GetValue();
}

void UEditor_MultiElementWidget::SetElementIndex(const int& _index)
{
	index = _index;
	const FString& _str = FString("Spawn ") + FString::FromInt(index) + FString(" : ");
	indexText->SetText(FText::FromString(_str));
}
#pragma endregion 

#pragma region Spawner Multi
void UEditor_PB_SpawnerMultiWidget::InitializeSpawnerMultiWidget(const FSpawnerMultiSettings& _spawnerSettings)
{
	InitMultiElementList(_spawnerSettings.quantityBySpawn);
	rangeSP->SetValue(_spawnerSettings.fRange);
	minDistanceSP->SetValue(_spawnerSettings.fMinDistance);
}
FSpawnerMultiSettings UEditor_PB_SpawnerMultiWidget::GetSpawnerMultiSettings() const
{
	FSpawnerMultiSettings _spawnerSettings;
	_spawnerSettings.quantityBySpawn = GetMultiElementList();
	_spawnerSettings.fRange = rangeSP->GetValue();
	_spawnerSettings.fMinDistance = minDistanceSP->GetValue();
	return _spawnerSettings;
}

void UEditor_PB_SpawnerMultiWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}
void UEditor_PB_SpawnerMultiWidget::InitializeWidget()
{
	addButton->OnClicked.AddUniqueDynamic(this, &UEditor_PB_SpawnerMultiWidget::AddElementToMultiElementList);
	clearButton->OnClicked.AddUniqueDynamic(this, &UEditor_PB_SpawnerMultiWidget::ClearMultiElementList);
}

void UEditor_PB_SpawnerMultiWidget::InitMultiElementList(const TArray<unsigned int>& _elements)
{
	ClearMultiElementList();

	const int& _max = _elements.Num();
	for	(int i = 0; i < _max; i++)
	{
		if (!multiElementWidgetType.Get())
		{
			LOG("UEditor_PB_SpawnerMultiWidget::InitMultiElementList => Multi Element Widget Type is not valid.");
			break;
		}

		UEditor_MultiElementWidget* _element = CreateWidget<UEditor_MultiElementWidget>(this, multiElementWidgetType);
		multiElementListVB->AddChildToVerticalBox(_element);		
		_element->InitializeMultiElementWidget(i, _elements[i]);
		_element->OnRemoveElement().AddDynamic(this, &UEditor_PB_SpawnerMultiWidget::RemoveElementFromList);
	}
}
TArray<unsigned int> UEditor_PB_SpawnerMultiWidget::GetMultiElementList() const
{
	TArray<unsigned int> _elements;
	TArray<UWidget*> _children = multiElementListVB->GetAllChildren();

	const int& _max = _children.Num();
	for (int i = 0; i < _max; i++)
	{
		const UEditor_MultiElementWidget* _element = Cast<UEditor_MultiElementWidget>(_children[i]);
		if (!_element) continue;
		_elements.Add(_element->GetMultiElementValue());
	}
	return _elements;
}

void UEditor_PB_SpawnerMultiWidget::AddElementToMultiElementList()
{
	if (!multiElementWidgetType.Get())
	{
		LOG("UEditor_PB_SpawnerMultiWidget::InitMultiElementList => Multi Element Widget Type is not valid.");
		return;
	}

	UEditor_MultiElementWidget* _element = CreateWidget<UEditor_MultiElementWidget>(this, multiElementWidgetType);
	multiElementListVB->AddChildToVerticalBox(_element);
	_element->InitializeMultiElementWidget(multiElementListVB->GetChildrenCount() - 1);
	_element->OnRemoveElement().AddDynamic(this, &UEditor_PB_SpawnerMultiWidget::RemoveElementFromList);
}
void UEditor_PB_SpawnerMultiWidget::RemoveElementFromList(int _index)
{
	multiElementListVB->RemoveChildAt(_index);
	
	const TArray<UWidget*>& _children = multiElementListVB->GetAllChildren();
	const int& _max = _children.Num();

	for (int i = 0; i< _max; i++)
	{
		UEditor_MultiElementWidget* _element = Cast<UEditor_MultiElementWidget>(_children[i]);
		if (!_element) continue;
		_element->SetElementIndex(i);
	}	
}
void UEditor_PB_SpawnerMultiWidget::ClearMultiElementList()
{
	multiElementListVB->ClearChildren();
}
#pragma endregion

#pragma region Spawner
void UEditor_PB_SpawnerWidget::InitializeSpawnerWidget(const FSpawnerSettings& _spawnerSettings)
{
	UMiscLibrary::SetCheckBoxState(WaitCB, _spawnerSettings.bWaitEndOfGPE);
	const FVector& _offset = _spawnerSettings.spawnOffset;
	xOffsetSP->SetValue(_offset.X);
	yOffsetSP->SetValue(_offset.Y);
	zOffsetSP->SetValue(_offset.Z);
	spawnRateSP->SetValue(_spawnerSettings.fSpawnRate);
}
FSpawnerSettings UEditor_PB_SpawnerWidget::GetSpawnerSettings() const
{
	FSpawnerSettings _spawnerSettings;
	_spawnerSettings.bWaitEndOfGPE = UMiscLibrary::GetCheckBoxValue(WaitCB);
	_spawnerSettings.spawnOffset = FVector(xOffsetSP->GetValue(), yOffsetSP->GetValue(), zOffsetSP->GetValue());
	_spawnerSettings.fSpawnRate = spawnRateSP->GetValue();
	return _spawnerSettings;
}
#pragma endregion

#pragma region GPE Projectile
void UEditor_GPE_ProjectileWidget::InitializeGPEProjectileWidget(const FGPE_ProjectileSettings& _gpeSettings, const bool& _editState)
{
	UMiscLibrary::SetCheckBoxState(editCB, _editState);
	EditChanged(_editState);
	damageSP->SetValue(_gpeSettings.fDamage);
	UMiscLibrary::SetCheckBoxState(moveUpCB, _gpeSettings.bMoveUp);
	zOffsetSP->SetValue(_gpeSettings.fZOffset);
	moveSpeedSP->SetValue(_gpeSettings.fMoveSpeed);
	previewTimeSP->SetValue(_gpeSettings.fPreviewTime);
}
FGPE_ProjectileSettings UEditor_GPE_ProjectileWidget::GetGPEProjectileSettings() const
{
	FGPE_ProjectileSettings _gpeSettings;
	_gpeSettings.fDamage = damageSP->GetValue();
	_gpeSettings.bMoveUp = UMiscLibrary::GetCheckBoxValue(moveUpCB);
	_gpeSettings.fZOffset = zOffsetSP->GetValue();
	_gpeSettings.fMoveSpeed = moveSpeedSP->GetValue();
	_gpeSettings.fPreviewTime = previewTimeSP->GetValue();
	return _gpeSettings;
}
bool UEditor_GPE_ProjectileWidget::GetEditGPEProjectileState() const
{
	return UMiscLibrary::GetCheckBoxValue(editCB);
}

void UEditor_GPE_ProjectileWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}
void UEditor_GPE_ProjectileWidget::InitializeWidget()
{
	editCB->OnCheckStateChanged.AddUniqueDynamic(this, &UEditor_GPE_ProjectileWidget::EditChanged);
}

void UEditor_GPE_ProjectileWidget::EditChanged(bool _editState)
{
	settingsVB->SetVisibility(_editState ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	if (_editState)
		projectileVB->AddChildToVerticalBox(settingsVB);
	else
		projectileVB->RemoveChild(settingsVB);
}
#pragma endregion

#pragma region GPE Floor
void UEditor_GPE_FloorWidget::InitializeGPEFloorWidget(const FGPE_FloorDangerSettings& _gpeSettings, const bool _editState)
{
	UMiscLibrary::SetCheckBoxState(editCB, _editState);
	EditChanged(_editState);
	damageSP->SetValue(_gpeSettings.fDamage);
	previewTimeSP->SetValue(_gpeSettings.fPreviewTime);
	persistentTimeSP->SetValue(_gpeSettings.fPersistantTime);
}
FGPE_FloorDangerSettings UEditor_GPE_FloorWidget::GetGPEFloorSettings() const
{
	FGPE_FloorDangerSettings _gpeSettings;
	_gpeSettings.fDamage = damageSP->GetValue();
	_gpeSettings.fPreviewTime = previewTimeSP->GetValue();
	_gpeSettings.fPersistantTime = persistentTimeSP->GetValue();
	return _gpeSettings;
}
bool UEditor_GPE_FloorWidget::GetEditGPEFloorState() const
{
	return UMiscLibrary::GetCheckBoxValue(editCB);
}

void UEditor_GPE_FloorWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}
void UEditor_GPE_FloorWidget::InitializeWidget()
{
	editCB->OnCheckStateChanged.AddUniqueDynamic(this, &UEditor_GPE_FloorWidget::EditChanged);
}

void UEditor_GPE_FloorWidget::EditChanged(bool _editState)
{
	settingsVB->SetVisibility(_editState ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	if (_editState)
		floorVB->AddChildToVerticalBox(settingsVB);
	else
		floorVB->RemoveChild(settingsVB);
}
#pragma endregion

#pragma region GPE HuntRift
void UEditor_GPE_HuntRiftWidget::InitializeGPEHuntRiftWidget(const FGPE_HuntRiftSettings& _gpeSettings, const bool& _editState)
{
	UMiscLibrary::SetCheckBoxState(editCB, _editState);
	EditChanged(_editState);
	damageSP->SetValue(_gpeSettings.fDamage);
	moveSpeedSP->SetValue(_gpeSettings.fMoveSpeed);
	followDurationSP->SetValue(_gpeSettings.fFollowDuration);
}
FGPE_HuntRiftSettings UEditor_GPE_HuntRiftWidget::GetGPEHuntRiftSettings() const
{
	FGPE_HuntRiftSettings _gpeSettings;
	_gpeSettings.fDamage = damageSP->GetValue();
	_gpeSettings.fMoveSpeed = moveSpeedSP->GetValue();
	_gpeSettings.fFollowDuration = followDurationSP->GetValue();
	return _gpeSettings;
}
bool UEditor_GPE_HuntRiftWidget::GetEditGPEHuntRiftState() const
{
	return UMiscLibrary::GetCheckBoxValue(editCB);
}

void UEditor_GPE_HuntRiftWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();	
}
void UEditor_GPE_HuntRiftWidget::InitializeWidget()
{
	editCB->OnCheckStateChanged.AddUniqueDynamic(this, &UEditor_GPE_HuntRiftWidget::EditChanged);
}

void UEditor_GPE_HuntRiftWidget::EditChanged(bool _editState)
{
	settingsVB->SetVisibility(_editState ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	if (_editState)
		huntRiftVB->AddChildToVerticalBox(settingsVB);
	else
		huntRiftVB->RemoveChild(settingsVB);
}
#pragma endregion

#pragma region Zone
void UEditor_PB_ZoneWidget::InitializeZoneWidget(const FPatternData& _patternData)
{
	InitializeVisibility(_patternData);

	spawnerWidget->InitializeSpawnerWidget(_patternData.GetSpawnerSettings());
	spawnerDefaultWidget->InitializeSpawnerDefaultWidget(_patternData.GetSpawnerDefaultSettings());
	spawnerConcentricWidget->InitializeSpawnerConcentricWidget(_patternData.GetSpawnerConcentricSettings());
	spawnerMultiWidget->InitializeSpawnerMultiWidget(_patternData.GetSpawnerMultiSettings());
	
	gpeProjectileWidget->InitializeGPEProjectileWidget(_patternData.GetGPEProjectileSettings(), _patternData.GetEditProjectileState());
	gpeFloorWidget->InitializeGPEFloorWidget(_patternData.GetGPEFloorDangerSettings(), _patternData.GetEditFloorState());
	gpeHuntRiftWidget->InitializeGPEHuntRiftWidget(_patternData.GetGPEHuntRiftSettings(), _patternData.GetEditHuntRiftState());

	scalingWidget->InitializeScalingWidget(_patternData.GetScalingSettings());
}
void UEditor_PB_ZoneWidget::GetZoneOverride(FPatternData& _patternData) const
{
	_patternData.SetSpawnerSettings(spawnerWidget->GetSpawnerSettings());
	_patternData.SetSpawnerDefaultSettings(spawnerDefaultWidget->GetSpawnerDefaultSettings());
	_patternData.SetSpawnerConcentricSettings(spawnerConcentricWidget->GetSpawnerConcentricSettings());
	_patternData.SetSpawnerMultiSettings(spawnerMultiWidget->GetSpawnerMultiSettings());

	_patternData.SetGPEProjectileSettings(gpeProjectileWidget->GetGPEProjectileSettings());
	_patternData.SetGPEFloorDangerSettings(gpeFloorWidget->GetGPEFloorSettings());
	_patternData.SetGPEHuntRiftSettings(gpeHuntRiftWidget->GetGPEHuntRiftSettings());
	
	_patternData.SetEditProjectileState(gpeProjectileWidget->GetEditGPEProjectileState());
	_patternData.SetEditFloorState(gpeFloorWidget->GetEditGPEFloorState());
	_patternData.SetEditHuntRiftState(gpeHuntRiftWidget->GetEditGPEHuntRiftState());

	_patternData.SetScalingSettings(scalingWidget->GetScalingSettings());
}

void UEditor_PB_ZoneWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidgets();
}
void UEditor_PB_ZoneWidget::InitializeWidgets()
{
	if (spawnerWidgetType.Get())
	{
		spawnerWidget = CreateWidget<UEditor_PB_SpawnerWidget>(this, spawnerWidgetType);
		spawnerVB->AddChildToVerticalBox(spawnerWidget);
	}
	else
		LOG("Zone Widget => Spawner Widget Type is not valid.")

	if (spawnerDefaultWidgetType.Get())
	{
		spawnerDefaultWidget = CreateWidget<UEditor_PB_SpawnerDefaultWidget>(this, spawnerDefaultWidgetType);
		spawnerVB->AddChildToVerticalBox(spawnerDefaultWidget);
	}
	else
		LOG("Zone Widget => Spawner Default Widget Type is not valid.")

	if (spawnerConcentricWidgetType.Get())
	{
		spawnerConcentricWidget = CreateWidget<UEditor_PB_SpawnerConcentricWidget>(this, spawnerConcentricWidgetType);
		spawnerVB->AddChildToVerticalBox(spawnerWidget);
	}
	else
		LOG("Zone Widget => Spawner Concentric Widget Type is not valid.")

	if (spawnerMultiWidgetType.Get())
	{
		spawnerMultiWidget = CreateWidget<UEditor_PB_SpawnerMultiWidget>(this, spawnerMultiWidgetType);
		spawnerVB->AddChildToVerticalBox(spawnerWidget);
	}
	else
		LOG("Zone Widget => Spawner Multi Widget Type is not valid.")

	if (gpeProjectileWidgetType.Get())
	{
		gpeProjectileWidget = CreateWidget<UEditor_GPE_ProjectileWidget>(this, gpeProjectileWidgetType);
		gpeVB->AddChildToVerticalBox(gpeProjectileWidget);
	}
	else
		LOG("Zone Widget => GPE Projectile Widget Type is not valid.")

	if (gpeFloorWidgetType.Get())
	{
		gpeFloorWidget = CreateWidget<UEditor_GPE_FloorWidget>(this, gpeFloorWidgetType);
		gpeVB->AddChildToVerticalBox(gpeFloorWidget);
	}
	else
		LOG("Zone Widget => GPE Floor Widget Type is not valid.")

	if (gpeHuntRiftWidgetType.Get())
	{
		gpeHuntRiftWidget = CreateWidget<UEditor_GPE_HuntRiftWidget>(this, gpeHuntRiftWidgetType);
		gpeVB->AddChildToVerticalBox(gpeHuntRiftWidget);
	}
	else
		LOG("Zone Widget => GPE Hunt Rift Widget Type is not valid.")
	
	if (scalingWidgetType.Get())
	{
		scalingWidget = CreateWidget<UEditor_PB_ScalingWidget>(this, scalingWidgetType);
		zoneVB->AddChildToVerticalBox(scalingWidget);
	}
	else
		LOG("Zone Widget => Scaling Widget Type is not valid.")
}

void UEditor_PB_ZoneWidget::InitializeVisibility(const FPatternData& _patternData)
{
	const bool& _spawnerDefault = _patternData.GetSpawnerDefaultState();
	const bool& _spawnerConcentric = _patternData.GetSpawnerConcentricState();
	const bool& _spawnerMulti = _patternData.GetSpawnerMultiState();
	
	UpdateSpawnerWidget(spawnerDefaultWidget, _spawnerDefault);
	UpdateSpawnerWidget(spawnerConcentricWidget, _spawnerConcentric);
	UpdateSpawnerWidget(spawnerMultiWidget, _spawnerMulti);

	const TSubclassOf<AGPE_Zone>& _type = _patternData.GetSpawnerSettings().zoneType;
	
	const bool& _gpeProjectile = _type.Get()->IsChildOf(AGPE_Projectile::StaticClass());
	const bool& _gpeFloor = _type.Get()->IsChildOf(AGPE_FloorDanger::StaticClass());
	const bool& _gpeHuntRift = _type.Get()->IsChildOf(AGPE_HuntRift::StaticClass());
	
	UpdateGPEWidget(gpeProjectileWidget, _gpeProjectile);
	UpdateGPEWidget(gpeFloorWidget, _gpeFloor);
	UpdateGPEWidget(gpeHuntRiftWidget, _gpeHuntRift);

	const bool& _scaling = _patternData.GetScalingState();
	
	UpdateZoneWidget(scalingWidget, _scaling);
}
void UEditor_PB_ZoneWidget::UpdateSpawnerWidget(UWidget* _widget, const bool& _visible) const
{
	UMiscLibrary::SetWidgetVisibility(_widget, _visible);
	if (_visible)
		spawnerVB->AddChildToVerticalBox(_widget);
	else
		spawnerVB->RemoveChild(_widget);
}
void UEditor_PB_ZoneWidget::UpdateGPEWidget(UWidget* _widget, const bool& _visible) const
{
	UMiscLibrary::SetWidgetVisibility(_widget, _visible);
	if (_visible)
		gpeVB->AddChildToVerticalBox(_widget);
	else
		gpeVB->RemoveChild(_widget);
}
void UEditor_PB_ZoneWidget::UpdateZoneWidget(UWidget* _widget, const bool& _visible) const
{
	UMiscLibrary::SetWidgetVisibility(_widget, _visible);
	if (_visible)
		zoneVB->AddChildToVerticalBox(_widget);
	else
		zoneVB->RemoveChild(_widget);
}
#pragma endregion
#pragma endregion

#pragma region Arena Closer
void UEditor_PB_ArenaCloserWidget::InitializeArenaCloserWidget(const FPatternData& _patternData, UA_DT_ArenaDataTable* _arenaDataTable)
{
	arenaDataTable = _arenaDataTable;
	
	FArenaCloserSettings _settings = _patternData.GetArenaCloserSettings();
	FSliceSettings _sliceSettings = _settings.sliceSettings;
	UMiscLibrary::SetCheckBoxState(closePlayerSliceCB, _settings.bClosePlayerSlice);
	damageSP->SetValue(_sliceSettings.fDamage);
	timeBeforeCloseSP->SetValue(_sliceSettings.fTimeBeforeClose);
	closeTimeSP->SetValue(_sliceSettings.fCloseTime);
	
	InitArenaSliceNames(_settings.arenaSlices);
}
void UEditor_PB_ArenaCloserWidget::GetArenaCloserOverride(FPatternData& _patternData) const
{
	FArenaCloserSettings _settings;
	FSliceSettings _sliceSettings;
	_sliceSettings.fDamage = damageSP->GetValue();
	_sliceSettings.fTimeBeforeClose = timeBeforeCloseSP->GetValue();
	_sliceSettings.fCloseTime = closeTimeSP->GetValue();
	
	_settings.sliceSettings = _sliceSettings;
	_settings.bClosePlayerSlice = UMiscLibrary::GetCheckBoxValue(closePlayerSliceCB);
	_settings.arenaSlices = GetArenaSliceNames();

	_patternData.SetArenaCloserSettings(_settings);
}

void UEditor_PB_ArenaCloserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}
void UEditor_PB_ArenaCloserWidget::InitializeWidget()
{
	addButton->OnClicked.AddUniqueDynamic(this, &UEditor_PB_ArenaCloserWidget::AddArenaSliceElementToList);
	clearButton->OnClicked.AddUniqueDynamic(this, &UEditor_PB_ArenaCloserWidget::ClearArenaSliceList);
}

void UEditor_PB_ArenaCloserWidget::InitArenaSliceNames(const TArray<FString>& _names)
{
	ClearArenaSliceList();

	const int& _max = _names.Num();
	for	(int i = 0; i < _max; i++)
	{
		if (!pointNameElementWidgetType.Get())
		{
			LOG("UEditor_PB_ArenaCloserWidget::InitArenaSliceNames => Point Name Element Widget Type is not valid.");
			break;
		}

		UEditor_PointNameElementWidget* _element = CreateWidget<UEditor_PointNameElementWidget>(this, pointNameElementWidgetType);
		arenaSliceListVB->AddChildToVerticalBox(_element);		
		_element->InitializePointNameElementWidget(i, arenaDataTable->GetSliceNames(), _names[i]);
		_element->OnRemoveElement().AddDynamic(this, &UEditor_PB_ArenaCloserWidget::RemoveArenaSliceElementFromList);
	}
}
TArray<FString> UEditor_PB_ArenaCloserWidget::GetArenaSliceNames() const
{
	TArray<FString> _names;
	TArray<UWidget*> _children = arenaSliceListVB->GetAllChildren();

	const int& _max = _children.Num();
	for (int i = 0; i < _max; i++)
	{
		const UEditor_PointNameElementWidget* _element = Cast<UEditor_PointNameElementWidget>(_children[i]);
		if (!_element) continue;
		_names.Add(_element->GetElementName());
	}
	return _names;
}

void UEditor_PB_ArenaCloserWidget::AddArenaSliceElementToList()
{
	if (!pointNameElementWidgetType.Get())
	{
		LOG("UEditor_PB_SpawnerMultiWidget::InitMultiElementList => Multi Element Widget Type is not valid.");
		return;
	}

	UEditor_PointNameElementWidget* _element = CreateWidget<UEditor_PointNameElementWidget>(this, pointNameElementWidgetType);
	arenaSliceListVB->AddChildToVerticalBox(_element);	
	_element->InitializePointNameElementWidget(arenaSliceListVB->GetChildrenCount() - 1, arenaDataTable->GetSliceNames());
	_element->OnRemoveElement().AddDynamic(this, &UEditor_PB_ArenaCloserWidget::RemoveArenaSliceElementFromList);
}
void UEditor_PB_ArenaCloserWidget::RemoveArenaSliceElementFromList(int _index)
{
	arenaSliceListVB->RemoveChildAt(_index);
	
	const TArray<UWidget*>& _children = arenaSliceListVB->GetAllChildren();
	const int& _max = _children.Num();

	for (int i = 0; i< _max; i++)
	{
		UEditor_PointNameElementWidget* _element = Cast<UEditor_PointNameElementWidget>(_children[i]);
		if (!_element) continue;
		_element->SetElementIndex(i);
	}	
}
void UEditor_PB_ArenaCloserWidget::ClearArenaSliceList()
{
	arenaSliceListVB->ClearChildren();
}
#pragma endregion