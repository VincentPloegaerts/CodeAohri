#include "Editor_OverrideSerieWidget.h"

#include "IU_Macros.h"
#include "Components/CanvasPanelSlot.h"

#include "A_DT_ArenaDataTable.h"

void UEditor_OverrideSerieWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	InitializeOverrideWidget();
	InitializeWidgets();
}

void UEditor_OverrideSerieWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	saveButton->OnClicked.RemoveDynamic(this, &UEditor_OverrideSerieWidget::SaveAllData);
	cancelButton->OnClicked.RemoveDynamic(this, &UEditor_OverrideSerieWidget::CancelAllData);
}

void UEditor_OverrideSerieWidget::InitializeOverrideData(const FSerieData& _toOverride, UA_DT_ArenaDataTable* _arenaDataTable)
{
	arenaDataTable = _arenaDataTable;
	
	serieDataBackup = _toOverride;
	serieDataOverride = _toOverride;

	if (!serieWidget) return;
	serieWidget->InitializeSerieWidget(serieDataOverride.serieElementDatas);

	OpenSerieWidget();
}

void UEditor_OverrideSerieWidget::InitializeOverrideWidget()
{
	saveButton->OnClicked.AddDynamic(this, &UEditor_OverrideSerieWidget::SaveAllData);
	cancelButton->OnClicked.AddDynamic(this, &UEditor_OverrideSerieWidget::CancelAllData);
}
void UEditor_OverrideSerieWidget::InitializeWidgets()
{
	if (serieWidgetType.Get())
	{
		serieWidget = CreateWidget<UEditor_SerieWidget>(this, serieWidgetType);
		serieWidget->OnSerieElementSelected().AddDynamic(this, &UEditor_OverrideSerieWidget::OpenSerieElementDataWidget);
		UCanvasPanelSlot* _slot = widgetsCanvas->AddChildToCanvas(serieWidget);
		_slot->SetOffsets(FMargin(0, 0, 0, 0));
		_slot->SetAnchors(FAnchors(0, 0, 1, 1));
	}
	else
		LOG("UEditor_OverrideSerieWidget::InitializeWidgets => Serie Widget Type is not valid.");

	if (serieElementDataWidgetType.Get())
	{
		serieElementDataWidget = CreateWidget<UEditor_SerieElementDataWidget>(this, serieElementDataWidgetType);
		UCanvasPanelSlot* _slot = widgetsCanvas->AddChildToCanvas(serieElementDataWidget);
		_slot->SetOffsets(FMargin(0, 0, 0, 0));
		_slot->SetAnchors(FAnchors(0, 0, 1, 1));
		
		serieElementDataWidget->BackButton()->OnClicked.AddDynamic(this, &UEditor_OverrideSerieWidget::CloseSerieElementDataWidget);
		serieElementDataWidget->OnSaveSerieElementData().AddDynamic(this, &UEditor_OverrideSerieWidget::SetSerieElementData);
		serieElementDataWidget->OnResetSerieElementData().AddDynamic(this, &UEditor_OverrideSerieWidget::ResetSerieElementData);

		serieElementDataWidget->OnEditPatternSpawnSettings().AddDynamic(this, &UEditor_OverrideSerieWidget::OpenSpawnSettingsWidget);
		serieElementDataWidget->OnEditPatternBehaviorEnable().AddDynamic(this, &UEditor_OverrideSerieWidget::OpenPatternEnableBehaviorWidget);
		serieElementDataWidget->OnEditPatternBehavior().AddDynamic(this, &UEditor_OverrideSerieWidget::OpenPatternBehaviorSettingsWidget);
	}
	else
		LOG("UEditor_OverrideSerieWidget::InitializeWidgets => Serie Element Data Widget Type is not valid.")

	if (spawnSettingsWidgetType.Get())
	{
		spawnSettingsWidget = CreateWidget<UEditor_SpawnSettingsWidget>(this, spawnSettingsWidgetType);
		UCanvasPanelSlot* _slot = widgetsCanvas->AddChildToCanvas(spawnSettingsWidget);
		_slot->SetOffsets(FMargin(0, 0, 0, 0));
		_slot->SetAnchors(FAnchors(0, 0, 1, 1));
		
		spawnSettingsWidget->BackButton()->OnClicked.AddDynamic(this, &UEditor_OverrideSerieWidget::CloseSpawnSettingsWidget);
		spawnSettingsWidget->OnSaveSpawnSettings().AddDynamic(this, &UEditor_OverrideSerieWidget::SetSpawnSettings);
		spawnSettingsWidget->OnResetSpawnSettings().AddDynamic(this, &UEditor_OverrideSerieWidget::ResetSpawnSettings);
	}
	else
		LOG("UEditor_OverrideSerieWidget::InitializeWidgets => Spawn Settings Widget Type is not valid.")

	if (patternEnableBehaviorWidgetType.Get())
	{
		patternEnableBehaviorWidget = CreateWidget<UEditor_PatternEnableBehaviorWidget>(this, patternEnableBehaviorWidgetType);
		UCanvasPanelSlot* _slot = widgetsCanvas->AddChildToCanvas(patternEnableBehaviorWidget);
		_slot->SetOffsets(FMargin(0, 0, 0, 0));
		_slot->SetAnchors(FAnchors(0, 0, 1, 1));
		
		patternEnableBehaviorWidget->BackButton()->OnClicked.AddDynamic(this, &UEditor_OverrideSerieWidget::ClosePatternEnableBehaviorWidget);
		patternEnableBehaviorWidget->OnSavePatternEnableBehavior().AddDynamic(this, &UEditor_OverrideSerieWidget::SetPatternEnableBehavior);
		patternEnableBehaviorWidget->OnResetPatternEnableBehavior().AddDynamic(this, &UEditor_OverrideSerieWidget::ResetPatternEnableBehavior);
	}
	else
		LOG("UEditor_OverrideSerieWidget::InitializeWidgets => Pattern Enable Behavior Widget Type is not valid.")

	if (patternBehaviorSettingsWidgetType.Get())
	{
		patternBehaviorSettingsWidget = CreateWidget<UEditor_PatternBehaviorSettingsWidget>(this, patternBehaviorSettingsWidgetType);
		UCanvasPanelSlot* _slot = widgetsCanvas->AddChildToCanvas(patternBehaviorSettingsWidget);
		_slot->SetOffsets(FMargin(0, 0, 0, 0));
		_slot->SetAnchors(FAnchors(0, 0, 1, 1));

		patternBehaviorSettingsWidget->BackButton()->OnClicked.AddDynamic(this, &UEditor_OverrideSerieWidget::ClosePatternBehaviorSettingsWidget);
		patternBehaviorSettingsWidget->OnSavePatternBehaviorSettings().AddDynamic(this, &UEditor_OverrideSerieWidget::SetPatternBehaviorSettings);
		patternBehaviorSettingsWidget->OnResetPatternBehaviorSettings().AddDynamic(this, &UEditor_OverrideSerieWidget::ResetPatternBehaviorSettings);
	}
	else
		LOG("UEditor_OverrideSerieWidget::InitializeWidgets => Pattern Behavior Settings Widget Type is not valid.")
	
	HideAllWidget();
}

#pragma region Save
void UEditor_OverrideSerieWidget::SetSerieElementData(int _serieElementIndex, FSerieElementData _serieElementData)
{
	if (!serieDataOverride.serieElementDatas.IsValidIndex(_serieElementIndex)) return;
	
	serieDataOverride.serieElementDatas[_serieElementIndex] = _serieElementData;
}
void UEditor_OverrideSerieWidget::SetSpawnSettings(int _serieElementIndex, int _patternIndex, FSpawnSettings _spawnSettings)
{
	if (!serieDataOverride.serieElementDatas.IsValidIndex(_serieElementIndex)) return;
	if (!serieDataOverride.serieElementDatas[_serieElementIndex].patternsElementData.IsValidIndex(_patternIndex)) return;

	serieDataOverride.serieElementDatas[_serieElementIndex].patternsElementData[_patternIndex].spawnSettings = _spawnSettings;
}
void UEditor_OverrideSerieWidget::SetPatternEnableBehavior(int _serieElementIndex, int _patternIndex, FPatternData _patternData)
{
    if (!serieDataOverride.serieElementDatas.IsValidIndex(_serieElementIndex)) return;
    if (!serieDataOverride.serieElementDatas[_serieElementIndex].patternsElementData.IsValidIndex(_patternIndex)) return;

	FPatternData& _data = serieDataOverride.serieElementDatas[_serieElementIndex].patternsElementData[_patternIndex].patternData;
	
	//Rotation
	_data.SetAngleState(_patternData.GetAngleState());
	_data.SetInstantState(_patternData.GetInstantState());
	_data.SetRotatorState(_patternData.GetRotatorState());
	_data.SetTargetState(_patternData.GetTargetState());
	//Movement
	_data.SetMovementOnAxeState(_patternData.GetMovementOnAxeState());
	_data.SetMovementToTargetState(_patternData.GetMovementToTargetState());
	_data.SetMovementTeleportState(_patternData.GetMovementTeleportState());
	//Shoot
	_data.SetShootPointState(_patternData.GetShootPointState());
	_data.SetShootLineState(_patternData.GetShootLineState());
	_data.SetShootArcState(_patternData.GetShootArcState());
	_data.SetShootCircleState(_patternData.GetShootCircleState());
	_data.SetShootCrossState(_patternData.GetShootCrossState());
	_data.SetShootRandom2DState(_patternData.GetShootRandom2DState());
	_data.SetShootRandom3DState(_patternData.GetShootRandom3DState());
	//Laser
	_data.SetLaserState(_patternData.GetLaserState());
	//Wave
	_data.SetOverrideWave(_patternData.GetOverrideWave());
	//Zone
	_data.SetScalingState(_patternData.GetScalingState());
	_data.SetSpawnerDefaultState(_patternData.GetSpawnerDefaultState());
	_data.SetSpawnerConcentricState(_patternData.GetSpawnerConcentricState());
	_data.SetSpawnerMultiState(_patternData.GetSpawnerMultiState());
}
void UEditor_OverrideSerieWidget::SetPatternBehaviorSettings(int _serieElementIndex, int _patternIndex, FPatternData _patternData)
{
	if (!serieDataOverride.serieElementDatas.IsValidIndex(_serieElementIndex)) return;
	if (!serieDataOverride.serieElementDatas[_serieElementIndex].patternsElementData.IsValidIndex(_patternIndex)) return;

	FPatternData& _data = serieDataOverride.serieElementDatas[_serieElementIndex].patternsElementData[_patternIndex].patternData;
	
	//Rotation
	_data.SetRotationSettings(_patternData.GetRotationSettings());
	_data.SetAngleSettings(_patternData.GetAngleSettings());
	_data.SetInstantSettings(_patternData.GetInstantSettings());
	_data.SetRotatorSettings(_patternData.GetRotatorSettings());
	_data.SetTargetSettings(_patternData.GetTargetSettings());
	//Movement
	_data.SetMovementOnAxeSettings(_patternData.GetMovementOnAxeSettings());
	_data.SetMovementToTargetSettings(_patternData.GetMovementToTargetSettings());
	_data.SetMovementTeleportSettings(_patternData.GetMovementTeleportSettings());
	//Shoot
	_data.SetShootPointSettings(_patternData.GetShootPointSettings());
	_data.SetShootLineSettings(_patternData.GetShootLineSettings());
	_data.SetShootArcSettings(_patternData.GetShootArcSettings());
	_data.SetShootCircleSettings(_patternData.GetShootCircleSettings());
	_data.SetShootCrossSettings(_patternData.GetShootCrossSettings());
	_data.SetShootRandom2DSettings(_patternData.GetShootRandom2DSettings());
	_data.SetShootRandom3DSettings(_patternData.GetShootRandom3DSettings());
	//Projectile
	_data.SetProjectileShootSettings(_patternData.GetProjectileShootSettings());
	//Laser
	_data.SetLaserSettings(_patternData.GetLaserSettings());
	//Wave
	_data.SetWaveSettings(_patternData.GetWaveSettings());
	//Zone
	_data.SetScalingSettings(_patternData.GetScalingSettings());
	_data.SetSpawnerDefaultSettings(_patternData.GetSpawnerDefaultSettings());
	_data.SetSpawnerConcentricSettings(_patternData.GetSpawnerConcentricSettings());
	_data.SetSpawnerMultiSettings(_patternData.GetSpawnerMultiSettings());
	_data.SetScalingSettings(_patternData.GetScalingSettings());
	//GPE
	_data.SetGPEFloorDangerSettings(_patternData.GetGPEFloorDangerSettings());
	_data.SetGPEProjectileSettings(_patternData.GetGPEProjectileSettings());
	_data.SetGPEHuntRiftSettings(_patternData.GetGPEHuntRiftSettings());
	_data.SetEditFloorState(_patternData.GetEditFloorState());
	_data.SetEditProjectileState(_patternData.GetEditProjectileState());
	_data.SetEditHuntRiftState(_patternData.GetEditHuntRiftState());
	//ArenaCloser
	_data.SetArenaCloserSettings(_patternData.GetArenaCloserSettings());
}

void UEditor_OverrideSerieWidget::SaveAllData()
{
	onSaveAll.Broadcast(serieDataOverride);
}
void UEditor_OverrideSerieWidget::CancelAllData()
{
	serieDataOverride = serieDataBackup;
	
	onSaveAll.Broadcast(serieDataBackup);
	
	OpenSerieWidget();
}
#pragma endregion

#pragma region Reset
void UEditor_OverrideSerieWidget::ResetSerieElementData(int _serieElementIndex)
{
	if (!serieElementDataWidget) return;

	serieElementDataWidget->ResetSerieElementDataWidget(serieDataBackup.serieElementDatas[_serieElementIndex]);
}
void UEditor_OverrideSerieWidget::ResetSpawnSettings(int _serieElementIndex, int _patternIndex)
{
	if (!spawnSettingsWidget) return;
	
	spawnSettingsWidget->ResetSpawnSettingsWidget(serieDataBackup.serieElementDatas[_serieElementIndex].patternsElementData[_patternIndex].spawnSettings);
}
void UEditor_OverrideSerieWidget::ResetPatternEnableBehavior(int _serieElementIndex, int _patternIndex)
{
	if (!patternEnableBehaviorWidget) return;
	
	patternEnableBehaviorWidget->ResetPatternEnableBehaviorWidget(serieDataBackup.serieElementDatas[_serieElementIndex].patternsElementData[_patternIndex].patternData);
}
void UEditor_OverrideSerieWidget::ResetPatternBehaviorSettings(int _serieElementIndex, int _patternIndex)
{
	if (!patternBehaviorSettingsWidget) return;
	
	patternBehaviorSettingsWidget->ResetPatternBehaviorSettingsWidget(serieDataBackup.serieElementDatas[_serieElementIndex].patternsElementData[_patternIndex].patternData);
}
#pragma endregion

#pragma region Navigation
void UEditor_OverrideSerieWidget::OpenSerieWidget()
{
	HideAllWidget();
	ShowWidget(serieWidget);
}

void UEditor_OverrideSerieWidget::OpenSerieElementDataWidget(int _serieElementIndex)
{
	if (!serieElementDataWidget) return;
	
	HideAllWidget();
	serieElementDataWidget->InitializeSerieElementDataWidget(_serieElementIndex, serieDataOverride.serieElementDatas[_serieElementIndex]);
	ShowWidget(serieElementDataWidget);
}
void UEditor_OverrideSerieWidget::CloseSerieElementDataWidget()
{
	OpenSerieWidget();
}

void UEditor_OverrideSerieWidget::OpenSpawnSettingsWidget(int _serieElementIndex, int _patternIndex)
{
	if (!spawnSettingsWidget) return;
	
	HideAllWidget();
	spawnSettingsWidget->InitializeSpawnSettingsWidgetUtil(arenaDataTable);
	spawnSettingsWidget->InitializeSpawnSettingsWidget(_serieElementIndex, _patternIndex, serieDataOverride.serieElementDatas[_serieElementIndex].patternsElementData[_patternIndex].spawnSettings);
	ShowWidget(spawnSettingsWidget);
}
void UEditor_OverrideSerieWidget::CloseSpawnSettingsWidget()
{
	OpenSerieElementDataWidget(serieElementDataWidget->GetSerieElementIndex());
}

void UEditor_OverrideSerieWidget::OpenPatternEnableBehaviorWidget(int _serieElementIndex, int _patternIndex)
{
	if (!patternEnableBehaviorWidget) return;
	
	HideAllWidget();
	patternEnableBehaviorWidget->InitializePatternEnableBehaviorWidget(_serieElementIndex, _patternIndex, serieDataOverride.serieElementDatas[_serieElementIndex].patternsElementData[_patternIndex].patternData);
	ShowWidget(patternEnableBehaviorWidget);
}
void UEditor_OverrideSerieWidget::ClosePatternEnableBehaviorWidget()
{
	OpenSerieElementDataWidget(patternEnableBehaviorWidget->GetSerieElementIndex());
}

void UEditor_OverrideSerieWidget::OpenPatternBehaviorSettingsWidget(int _serieElementIndex, int _patternIndex)
{
	if (!patternBehaviorSettingsWidget) return;
	HideAllWidget();
	
	patternBehaviorSettingsWidget->InitializePatternBehaviorSettingsWidget(_serieElementIndex, _patternIndex, serieDataOverride.serieElementDatas[_serieElementIndex].patternsElementData[_patternIndex].patternData, arenaDataTable);
	ShowWidget(patternBehaviorSettingsWidget);
}
void UEditor_OverrideSerieWidget::ClosePatternBehaviorSettingsWidget()
{
	OpenSerieElementDataWidget(patternEnableBehaviorWidget->GetSerieElementIndex());	
}
#pragma endregion

#pragma region Visibility
void UEditor_OverrideSerieWidget::HideAllWidget() const
{
	HideWidget(serieWidget);
	HideWidget(serieElementDataWidget);
	HideWidget(spawnSettingsWidget);
	HideWidget(patternEnableBehaviorWidget);
	HideWidget(patternBehaviorSettingsWidget);	
}

void UEditor_OverrideSerieWidget::HideWidget(UWidget* _panel) const
{
	if (!_panel) return;
	_panel->SetVisibility(ESlateVisibility::Hidden);
}
void UEditor_OverrideSerieWidget::ShowWidget(UWidget* _panel) const
{
	if (!_panel) return;
	_panel->SetVisibility(ESlateVisibility::Visible);
}
#pragma endregion