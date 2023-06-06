#include "Editor_OverrideWidgets.h"

#include "A_DT_ArenaDataTable.h"

#include "Components/HorizontalBoxSlot.h"
#include "Components/CanvasPanelSlot.h"

#include "IU_Macros.h"

#pragma region Misc Library
void UMiscLibrary::InitComboBoxFromEnum(const FString& _enumName, UComboBoxString* _comboBox)
{
	if (!_comboBox) return;
	
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *_enumName, true);
	if (!EnumPtr) return;

	_comboBox->ClearOptions();
	const int& _max = EnumPtr->GetMaxEnumValue();
	for (int i = 0; i < _max; i++)
		_comboBox->AddOption(EnumPtr->GetNameByIndex(i).ToString());
}
void UMiscLibrary::InitComboBoxFromNames(const TArray<FString>& _names, UComboBoxString* _comboBox)
{
	if (!_comboBox) return;
	
	_comboBox->ClearOptions();
	const int& _max = _names.Num();
	for (int i = 0; i < _max; i++)
		_comboBox->AddOption(_names[i]);
}

int UMiscLibrary::GetEnumValueFromString(const FString& _enumValue, const FString& _enumName)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *_enumName, true);
	return EnumPtr->GetValueByNameString(_enumValue);
}

void UMiscLibrary::SetCheckBoxState(UCheckBox* _checkBox, const bool& _state)
{
	if (!_checkBox) return;
	_checkBox->SetCheckedState(_state ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}
bool UMiscLibrary::GetCheckBoxValue(const UCheckBox* _checkBox)
{
	if (!_checkBox) return false;
	return _checkBox->GetCheckedState() == ECheckBoxState::Checked ? true : false;
}

void UMiscLibrary::SetWidgetVisibility(UWidget* _widget, const bool& _visibility)
{
	if (!_widget) return;
	_widget->SetVisibility(_visibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UMiscLibrary::SetSpinBoxFromVector(USpinBox* _x, USpinBox* _y, USpinBox* _z, const FVector& _vector)
{
	_x->SetValue(_vector.X);	
	_y->SetValue(_vector.Y);	
	_z->SetValue(_vector.Z);	
}
FVector UMiscLibrary::GetVectorFromSpinBox(USpinBox* _x, USpinBox* _y, USpinBox* _z)
{
	FVector _vector;
	_vector.X = _x->GetValue();
	_vector.Y = _y->GetValue();
	_vector.Z = _z->GetValue();
	return _vector;
}

void UMiscLibrary::SetSpinBoxFromRotator(USpinBox* _pitch, USpinBox* _yaw, USpinBox* _roll, const FRotator& _rotator)
{
	_pitch->SetValue(_rotator.Pitch);
	_yaw->SetValue(_rotator.Yaw);
	_roll->SetValue(_rotator.Roll);
}
FRotator UMiscLibrary::GetRotatorFromSpinBox(USpinBox* _pitch, USpinBox* _yaw, USpinBox* _roll)
{
	FRotator _rotator;
	_rotator.Pitch = _pitch->GetValue();
	_rotator.Yaw = _yaw->GetValue();
	_rotator.Roll = _roll->GetValue();
	return _rotator;
}
#pragma endregion 

#pragma region Serie Element Widget
void UEditor_SerieElementWidget::NativeConstruct()
{
	Super::NativeConstruct();
	serieOverrideButton->OnClicked.AddDynamic(this, &UEditor_SerieElementWidget::SerieElementSelected);
}

void UEditor_SerieElementWidget::SerieElementSelected()
{
	onSelected.Broadcast(this);
}

void UEditor_SerieElementWidget::InitializeSerieElementWidget(const int& _serieElementIndex, const TArray<FPatternElementData>& _patternsElementData)
{
	serieElementIndex = _serieElementIndex;
	const FString& _text = FString("Serie Element : ") + FString::FromInt(serieElementIndex);
	serieText->SetText(FText::FromString(_text));

	FString _info = FString();
	const int& _max = _patternsElementData.Num();
	for (int i = 0; i < _max; i++)
	{
		 _info += _patternsElementData[i].patternData.GetPatternName() + " / ";
	}
	_info.RemoveFromEnd(" / ");
	patternInfoText->SetText(FText::FromString(_info));
}
#pragma endregion 

#pragma region Serie Widget
void UEditor_SerieWidget::InitializeSerieWidget(const TArray<FSerieElementData>& _serieElementData)
{
	const int& _max = _serieElementData.Num();
	for (int i = 0; i < _max; i++)
	{
		//check type
		UEditor_SerieElementWidget* _elementWidget = CreateWidget<UEditor_SerieElementWidget>(this, serieElementWidgetType);
		_elementWidget->InitializeSerieElementWidget(i, _serieElementData[i].patternsElementData);
		_elementWidget->OnSelected().AddDynamic(this, &UEditor_SerieWidget::SerieElementSelected);
		serieVerticalBox->AddChildToVerticalBox(_elementWidget);	
	}		
}

void UEditor_SerieWidget::SerieElementSelected(UEditor_SerieElementWidget* _selection)
{
	onSerieElementSelected.Broadcast(_selection->Index());
}
#pragma endregion

#pragma region Serie Element Data Pattern Widget
void UEditor_SerieElementDataPatternWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}
void UEditor_SerieElementDataPatternWidget::InitializeWidget()
{
	editSpawnSettingsButton->OnClicked.AddDynamic(this, &UEditor_SerieElementDataPatternWidget::EditSpawnSettings);
	editBehaviorEnableButton->OnClicked.AddDynamic(this, &UEditor_SerieElementDataPatternWidget::EditBehaviorEnable);
	editBehaviorButton->OnClicked.AddDynamic(this, &UEditor_SerieElementDataPatternWidget::EditBehavior);
}

void UEditor_SerieElementDataPatternWidget::InitializeSerieElementDataPatternWidget(const int& _patternIndex, const FString& _patternName)
{
	patternIndex = _patternIndex;
	const FString& _text = FString("Pattern : ") + _patternName;
	patternText->SetText(FText::FromString(_text));
}

void UEditor_SerieElementDataPatternWidget::EditSpawnSettings()
{
	onEditSpawnSettings.Broadcast(patternIndex);
}
void UEditor_SerieElementDataPatternWidget::EditBehaviorEnable()
{
	onEditBehaviorEnable.Broadcast(patternIndex);
}
void UEditor_SerieElementDataPatternWidget::EditBehavior()
{
	onEditBehavior.Broadcast(patternIndex);
}
#pragma endregion

#pragma region Serie Element Data Widget
void UEditor_SerieElementDataWidget::InitializeSerieElementDataWidget(const int& _serieElementIndex, const FSerieElementData& _serieElementData)
{
	serieElementIndex = _serieElementIndex;
	serieElementOverride = _serieElementData;
	serieElementDataBackup = _serieElementData;

	const int& _max = _serieElementData.patternsElementData.Num();
	patternsVerticalBox->ClearChildren();
	for (int i = 0; i < _max; i++)
	{
		if (!patternWidgetType.Get())
		{
			LOG("UEditor_SerieElementDataWidget::InitializeSerieElementDataWidget => Pattern Widget Type is not valid.");
			break;
		}
		UEditor_SerieElementDataPatternWidget* _patternWidget = CreateWidget<UEditor_SerieElementDataPatternWidget>(this, patternWidgetType);
		_patternWidget->InitializeSerieElementDataPatternWidget(i, _serieElementData.patternsElementData[i].patternData.GetPatternName());
		_patternWidget->OnEditSpawnSettings().AddDynamic(this, &UEditor_SerieElementDataWidget::EditPatternSpawnSettings);
		_patternWidget->OnEditBehaviorEnable().AddDynamic(this, &UEditor_SerieElementDataWidget::EditPatternBehaviorEnable);
		_patternWidget->OnEditBehavior().AddDynamic(this, &UEditor_SerieElementDataWidget::EditPatternBehavior);
		patternsVerticalBox->AddChildToVerticalBox(_patternWidget);
	}
	
	delaySpinBox->SetValue(serieElementOverride.fDelay);
	waitCheckBox->SetCheckedState(serieElementOverride.bWaitEndOfPatterns ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void UEditor_SerieElementDataWidget::ResetSerieElementDataWidget(const FSerieElementData& _serieElementData)
{
	serieElementOverride = _serieElementData;
	
	delaySpinBox->SetValue(serieElementOverride.fDelay);
	waitCheckBox->SetCheckedState(serieElementOverride.bWaitEndOfPatterns ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void UEditor_SerieElementDataWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	InitializeWidget();
}
void UEditor_SerieElementDataWidget::InitializeWidget()
{
	saveButton->OnClicked.AddDynamic(this, &UEditor_SerieElementDataWidget::SaveSerieElementData);
	cancelButton->OnClicked.AddDynamic(this, &UEditor_SerieElementDataWidget::CancelSerieElementData);
	resetButton->OnClicked.AddDynamic(this, &UEditor_SerieElementDataWidget::ResetSerieElementData);
}

void UEditor_SerieElementDataWidget::SaveSerieElementData()
{
	serieElementOverride.fDelay = delaySpinBox->GetValue();
	serieElementOverride.bWaitEndOfPatterns = waitCheckBox->GetCheckedState() == ECheckBoxState::Checked ? true : false;
	
	onSaveSerieElementData.Broadcast(serieElementIndex, serieElementOverride);
}
void UEditor_SerieElementDataWidget::CancelSerieElementData()
{
	delaySpinBox->SetValue(serieElementDataBackup.fDelay);
	waitCheckBox->SetCheckedState(serieElementDataBackup.bWaitEndOfPatterns ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	serieElementOverride = serieElementDataBackup;
	
	onSaveSerieElementData.Broadcast(serieElementIndex, serieElementDataBackup);	
}
void UEditor_SerieElementDataWidget::ResetSerieElementData()
{
	onResetSerieElementData.Broadcast(serieElementIndex);
}

void UEditor_SerieElementDataWidget::EditPatternSpawnSettings(int _patternIndex)
{
	onEditPatternSpawnSettings.Broadcast(serieElementIndex, _patternIndex);
}
void UEditor_SerieElementDataWidget::EditPatternBehaviorEnable(int _patternIndex)
{
	onEditPatternBehaviorEnable.Broadcast(serieElementIndex, _patternIndex);	
}
void UEditor_SerieElementDataWidget::EditPatternBehavior(int _patternIndex)
{
	onEditPatternBehavior.Broadcast(serieElementIndex, _patternIndex);	
}
#pragma endregion

#pragma region Point Name Element Widget
void UEditor_PointNameElementWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}
void UEditor_PointNameElementWidget::InitializeWidget()
{
	removeButton->OnClicked.AddDynamic(this, &UEditor_PointNameElementWidget::RemoveElement);
}

void UEditor_PointNameElementWidget::InitializePointNameElementWidget(const int& _serieElementIndex, const TArray<FString>& _names, const FString& _defaultName)
{
	elementIndex = _serieElementIndex;
	
	const int& _max = _names.Num();
	for (int i = 0; i < _max; i++)
		nameCB->AddOption(_names[i]);

	nameCB->SetSelectedOption(_defaultName);
	if (UHorizontalBoxSlot* _slot = Cast<UHorizontalBoxSlot>(nameCB->Slot.Get()))
	{
		LOG("Slot ok");	
		_slot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	}
}

void UEditor_PointNameElementWidget::SetElementIndex(const int& _elementIndex)
{
	elementIndex = _elementIndex;
}
void UEditor_PointNameElementWidget::SetElementName(const FString& _name)
{
	nameCB->SetSelectedOption(_name);
}

void UEditor_PointNameElementWidget::RemoveElement()
{
	onRemoveElement.Broadcast(elementIndex);
}
#pragma endregion

#pragma region Spawn Settings Widget
void UEditor_SpawnSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
	InitializeComboBox();
}
void UEditor_SpawnSettingsWidget::InitializeWidget()
{
	saveButton->OnClicked.AddDynamic(this, &UEditor_SpawnSettingsWidget::SaveSpawnSettings);
	cancelButton->OnClicked.AddDynamic(this, &UEditor_SpawnSettingsWidget::CancelSpawnSettings);
	resetButton->OnClicked.AddDynamic(this, &UEditor_SpawnSettingsWidget::ResetSpawnSettings);

	addButton->OnClicked.AddDynamic(this, &UEditor_SpawnSettingsWidget::AddElementToNameList);
	clearButton->OnClicked.AddDynamic(this, &UEditor_SpawnSettingsWidget::ClearNameList);

	spawnPositionCB->OnSelectionChanged.AddDynamic(this, &UEditor_SpawnSettingsWidget::RefreshSpawnPosition);
	randomCB->OnCheckStateChanged.AddDynamic(this, &UEditor_SpawnSettingsWidget::RefreshRandom);
	spawnDirectionModeCB->OnSelectionChanged.AddDynamic(this, &UEditor_SpawnSettingsWidget::RefreshSpawnDirectionMode);
	spawnDirectionCB->OnSelectionChanged.AddDynamic(this, &UEditor_SpawnSettingsWidget::RefreshSpawnDirection);
}
void UEditor_SpawnSettingsWidget::InitializeComboBox() const
{
	InitComboBoxFromEnum("ESpawnSettings", spawnPositionCB);
	InitComboBoxFromEnum("ESpawnDirectionMode", spawnDirectionModeCB);
	InitComboBoxFromEnum("ESpawnSettings", spawnDirectionCB);
	
	//directionArenaPointNameCB & spawnArenaPointNameCB init in InitializeSpawnSettingsWidgetUtil
}

void UEditor_SpawnSettingsWidget::InitComboBoxFromEnum(const FString& _enumName, UComboBoxString* _comboBox) const
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *_enumName, true);
	if (!EnumPtr) return;

	_comboBox->ClearOptions();
	const int& _max = EnumPtr->GetMaxEnumValue();
	for (int i = 0; i < _max; i++)
		_comboBox->AddOption(EnumPtr->GetNameByIndex(i).ToString());
}
int UEditor_SpawnSettingsWidget::GetEnumValueFromString(const FString& _value, const FString& _enumName) const
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *_enumName, true);
	return EnumPtr->GetValueByNameString(_value); 
}
void UEditor_SpawnSettingsWidget::InitComboBoxFromNames(const TArray<FString>& _names, UComboBoxString* _comboBox) const
{
	_comboBox->ClearOptions();
	const int& _max = _names.Num();
	for (int i = 0; i < _max; i++)
		_comboBox->AddOption(_names[i]);
}

void UEditor_SpawnSettingsWidget::InitializeComponents(const FSpawnSettings& _spawnSettings)
{
	spawnPositionCB->SetSelectedOption(*UEnum::GetValueAsString<ESpawnSettings>(_spawnSettings.eSpawnPosition));
	randomCB->SetCheckedState(_spawnSettings.bRandom ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	spawnArenaPointNameCB->SetSelectedOption(_spawnSettings.spawnArenaPointName);
	randomRangeSB->SetValue(_spawnSettings.fRandomRange);
	xOffsetSB->SetValue(_spawnSettings.vSpawnPositionOffset.X);
	yOffsetSB->SetValue(_spawnSettings.vSpawnPositionOffset.Y);
	zOffsetSB->SetValue(_spawnSettings.vSpawnPositionOffset.Z);
	spawnDirectionModeCB->SetSelectedOption(*UEnum::GetValueAsString<ESpawnDirectionMode>(_spawnSettings.eSpawnDirectionMode));
	spawnDirectionCB->SetSelectedOption(*UEnum::GetValueAsString<ESpawnSettings>(_spawnSettings.eSpawnDirection));
	directionArenaPointNameCB->SetSelectedOption(_spawnSettings.directionArenaPointName);
	localPitchSB->SetValue(_spawnSettings.rSpawnLocalDirectionOffset.Pitch);
	localYawSB->SetValue(_spawnSettings.rSpawnLocalDirectionOffset.Yaw);
	localRollSB->SetValue(_spawnSettings.rSpawnLocalDirectionOffset.Roll);
	xLookSB->SetValue(_spawnSettings.vLookTargetOffset.X);
	yLookSB->SetValue(_spawnSettings.vLookTargetOffset.Y);
	zLookSB->SetValue(_spawnSettings.vLookTargetOffset.Z);

	InitNameList(_spawnSettings.randomArenaPoint);
	
	RefreshDisplay();
}

void UEditor_SpawnSettingsWidget::InitNameList(const TArray<FString>& _names)
{
	ClearNameList();
	
	const int& _max = _names.Num();
	for	(int i = 0; i < _max; i++)
	{
		if (!pointNameElementWidgetType.Get())
		{
			LOG("UEditor_SpawnSettingsWidget::InitNameList => Point Name Element Widget Type is not valid.");
			break;
		}

		UEditor_PointNameElementWidget* _element = CreateWidget<UEditor_PointNameElementWidget>(this, pointNameElementWidgetType);
		nameListVB->AddChildToVerticalBox(_element);		
		_element->InitializePointNameElementWidget(i, pointNames, _names[i]);
		_element->OnRemoveElement().AddDynamic(this, &UEditor_SpawnSettingsWidget::RemoveElementFromList);
	}
}
TArray<FString> UEditor_SpawnSettingsWidget::GetNameList() const
{
	TArray<FString> _names;
	TArray<UWidget*> _children = nameListVB->GetAllChildren();
	
	const int& _max = _children.Num();
	for (int i = 0; i < _max; i++)
	{
		const UEditor_PointNameElementWidget* _element = Cast<UEditor_PointNameElementWidget>(_children[i]);
		if (!_element) continue;
		_names.Add(_element->GetElementName());
	}
	
	return _names;
}

void UEditor_SpawnSettingsWidget::InitializeSpawnSettingsWidget(int _serieElementIndex, int _patternIndex, const FSpawnSettings& _spawnSettings)
{
	serieElementIndex = _serieElementIndex;
	patternIndex = _patternIndex;
	
	spawnSettingsBackup = _spawnSettings;
	spawnSettingsOverride = _spawnSettings;

	InitializeComponents(spawnSettingsOverride);
}
void UEditor_SpawnSettingsWidget::InitializeSpawnSettingsWidgetUtil(const UA_DT_ArenaDataTable* _arenaDataTable)
{
	if (!_arenaDataTable) return;
	pointNames = _arenaDataTable->GetArenaSpawnPoint();
	InitComboBoxFromNames(pointNames, directionArenaPointNameCB);
	InitComboBoxFromNames(pointNames, spawnArenaPointNameCB);
}
void UEditor_SpawnSettingsWidget::ResetSpawnSettingsWidget(const FSpawnSettings& _spawnSettings)
{
	spawnSettingsOverride = _spawnSettings;
	InitializeComponents(spawnSettingsOverride);
}

void UEditor_SpawnSettingsWidget::SaveSpawnSettings()
{
	spawnSettingsOverride.eSpawnPosition = TEnumAsByte<ESpawnSettings>(GetEnumValueFromString(spawnPositionCB->GetSelectedOption(), "ESpawnSettings"));
	spawnSettingsOverride.bRandom = randomCB->GetCheckedState() == ECheckBoxState::Checked ? true : false;
	spawnSettingsOverride.spawnArenaPointName = spawnArenaPointNameCB->GetSelectedOption();
	spawnSettingsOverride.fRandomRange = randomRangeSB->GetValue();
	spawnSettingsOverride.vSpawnPositionOffset = FVector(xOffsetSB->GetValue(), yOffsetSB->GetValue(), zOffsetSB->GetValue());
	spawnSettingsOverride.eSpawnDirectionMode = TEnumAsByte<ESpawnDirectionMode>(GetEnumValueFromString(spawnDirectionModeCB->GetSelectedOption(), "ESpawnDirectionMode"));
	spawnSettingsOverride.eSpawnDirection = TEnumAsByte<ESpawnSettings>(GetEnumValueFromString(spawnDirectionCB->GetSelectedOption(), "ESpawnSettings"));
	spawnSettingsOverride.directionArenaPointName = directionArenaPointNameCB->GetSelectedOption();
	spawnSettingsOverride.rSpawnLocalDirectionOffset = FRotator(localPitchSB->GetValue(), localYawSB->GetValue(), localRollSB->GetValue());
	spawnSettingsOverride.vLookTargetOffset = FVector(xLookSB->GetValue(), yLookSB->GetValue(), zLookSB->GetValue());

	spawnSettingsOverride.randomArenaPoint = GetNameList();
	
	onSaveSpawnSettings.Broadcast(serieElementIndex, patternIndex, spawnSettingsOverride);
}
void UEditor_SpawnSettingsWidget::CancelSpawnSettings()
{
	spawnSettingsOverride = spawnSettingsBackup;
	InitializeSpawnSettingsWidget(serieElementIndex, patternIndex, spawnSettingsBackup);
	
	onSaveSpawnSettings.Broadcast(serieElementIndex, patternIndex, spawnSettingsOverride);
}
void UEditor_SpawnSettingsWidget::ResetSpawnSettings()
{
	onResetSpawnSettings.Broadcast(serieElementIndex, patternIndex);
}

void UEditor_SpawnSettingsWidget::AddElementToNameList()
{
	if (!pointNameElementWidgetType.Get())
	{
		LOG("UEditor_SpawnSettingsWidget::AddElementToNameList => Point Name Element Widget Type is not valid.");
		return;
	}

	UEditor_PointNameElementWidget* _element = CreateWidget<UEditor_PointNameElementWidget>(this, pointNameElementWidgetType);
	nameListVB->AddChildToVerticalBox(_element);
	_element->InitializePointNameElementWidget(nameListVB->GetChildrenCount() - 1, pointNames);
	_element->OnRemoveElement().AddDynamic(this, &UEditor_SpawnSettingsWidget::RemoveElementFromList);
}
void UEditor_SpawnSettingsWidget::RemoveElementFromList(int _elementIndex)
{
	nameListVB->RemoveChildAt(_elementIndex);
	
	const TArray<UWidget*>& _children = nameListVB->GetAllChildren();
	const int& _max = _children.Num();

	for (int i = 0; i< _max; i++)
	{
		UEditor_PointNameElementWidget* _element = Cast<UEditor_PointNameElementWidget>(_children[i]);
		if (!_element) continue;
		_element->SetElementIndex(i);
	}	
}
void UEditor_SpawnSettingsWidget::ClearNameList()
{
	nameListVB->ClearChildren();
}

void UEditor_SpawnSettingsWidget::RefreshDisplay()
{
	const bool& _randomState = randomCB->GetCheckedState() == ECheckBoxState::Checked ? true : false;
	const ESpawnSettings& _spawnPosition = TEnumAsByte<ESpawnSettings>(GetEnumValueFromString(spawnPositionCB->GetSelectedOption(), "ESpawnSettings"));
	const ESpawnDirectionMode& _spawnDirectionMode = TEnumAsByte<ESpawnDirectionMode>(GetEnumValueFromString(spawnDirectionModeCB->GetSelectedOption(), "ESpawnDirectionMode"));
	const ESpawnSettings& _spawnDirection = TEnumAsByte<ESpawnSettings>(GetEnumValueFromString(spawnDirectionCB->GetSelectedOption(), "ESpawnSettings"));

	const ESlateVisibility& _randomVisibility = _spawnPosition == ESpawnSettings::ARENA_POINT ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	const ESlateVisibility& _spawnArenaPointNameVisibility = _spawnPosition == ESpawnSettings::ARENA_POINT && !_randomState ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	const ESlateVisibility& _randomPointVisibility =  _spawnPosition == ESpawnSettings::ARENA_POINT && _randomState ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

	const ESlateVisibility& _spawnDirectionVisibility = _spawnDirectionMode ==  ESpawnDirectionMode::TARGET ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	const ESlateVisibility& _directionArenaPointNameVisibility = _spawnDirection == ESpawnSettings::ARENA_POINT && _spawnDirectionMode ==  ESpawnDirectionMode::TARGET ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	const ESlateVisibility& _localVisibility = _spawnDirection == ESpawnDirectionMode::DIRECTION ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	const ESlateVisibility& _lookVisibility = _spawnDirection == ESpawnDirectionMode::TARGET ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	
	randomHB->SetVisibility(_randomVisibility);
	spawnArenaPointNameHB->SetVisibility(_spawnArenaPointNameVisibility);
	randomArenaPointsHB->SetVisibility(_randomPointVisibility);
	spawnDirectionHB->SetVisibility(_spawnDirectionVisibility);
	directionArenaPointNameHB->SetVisibility(_directionArenaPointNameVisibility);
	spawnLocalDirectionOffsetHB->SetVisibility(_localVisibility);
	lookTargetOffsetHB->SetVisibility(_lookVisibility);
}

void UEditor_SpawnSettingsWidget::RefreshRandom(bool _randomState)
{
	const ESpawnSettings& _spawnPosition = TEnumAsByte<ESpawnSettings>(GetEnumValueFromString(spawnPositionCB->GetSelectedOption(), "ESpawnSettings"));

	const ESlateVisibility& _spawnArenaPointNameVisibility = _spawnPosition == ESpawnSettings::ARENA_POINT && !_randomState ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	const ESlateVisibility& _randomPointVisibility =  _spawnPosition == ESpawnSettings::ARENA_POINT && _randomState ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	
	spawnArenaPointNameHB->SetVisibility(_spawnArenaPointNameVisibility);
	randomArenaPointsHB->SetVisibility(_randomPointVisibility);
}
void UEditor_SpawnSettingsWidget::RefreshSpawnPosition(FString _option, ESelectInfo::Type _selectInfo)
{
	const ESpawnSettings& _spawnPosition = TEnumAsByte<ESpawnSettings>(GetEnumValueFromString(_option, "ESpawnSettings"));

	const ESlateVisibility& _randomVisibility = _spawnPosition == ESpawnSettings::ARENA_POINT ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	
	randomHB->SetVisibility(_randomVisibility);
	RefreshRandom(randomCB->GetCheckedState() == ECheckBoxState::Checked ? true : false);
}
void UEditor_SpawnSettingsWidget::RefreshSpawnDirectionMode(FString _option, ESelectInfo::Type _selectInfo)
{
	const ESpawnDirectionMode& _spawnDirectionMode = TEnumAsByte<ESpawnDirectionMode>(GetEnumValueFromString(spawnDirectionModeCB->GetSelectedOption(), "ESpawnDirectionMode"));

	const ESlateVisibility& _spawnDirectionVisibility = _spawnDirectionMode ==  ESpawnDirectionMode::TARGET ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	const ESlateVisibility& _spawnLocalDirectionVisibility =  _spawnDirectionMode ==  ESpawnDirectionMode::DIRECTION ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	const ESlateVisibility& _lookTargetOffsetVisibility = _spawnDirectionMode ==  ESpawnDirectionMode::TARGET ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

	spawnDirectionHB->SetVisibility(_spawnDirectionVisibility);
	spawnLocalDirectionOffsetHB->SetVisibility(_spawnLocalDirectionVisibility);
	lookTargetOffsetHB->SetVisibility(_lookTargetOffsetVisibility);
	
	RefreshSpawnDirection(spawnDirectionCB->GetSelectedOption(), ESelectInfo::Direct);
}
void UEditor_SpawnSettingsWidget::RefreshSpawnDirection(FString _option, ESelectInfo::Type _selectInfo)
{
	const ESpawnSettings& _spawnDirection = TEnumAsByte<ESpawnSettings>(GetEnumValueFromString(spawnDirectionCB->GetSelectedOption(), "ESpawnSettings"));
	const ESpawnDirectionMode& _spawnDirectionMode = TEnumAsByte<ESpawnDirectionMode>(GetEnumValueFromString(spawnDirectionModeCB->GetSelectedOption(), "ESpawnDirectionMode"));
	
	const ESlateVisibility& _directionArenaPointNameVisibility = _spawnDirectionMode ==  ESpawnDirectionMode::TARGET && _spawnDirection == ESpawnSettings::ARENA_POINT ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

	directionArenaPointNameHB->SetVisibility(_directionArenaPointNameVisibility);
}
#pragma endregion 

#pragma region Pattern Behavior Enable
void UEditor_PatternEnableBehaviorWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
}
void UEditor_PatternEnableBehaviorWidget::InitializeWidget()
{
	saveButton->OnClicked.AddDynamic(this, &UEditor_PatternEnableBehaviorWidget::SavePatternBehaviorEnable);
	cancelButton->OnClicked.AddDynamic(this, &UEditor_PatternEnableBehaviorWidget::CancelPatternBehaviorEnable);
	resetButton->OnClicked.AddDynamic(this, &UEditor_PatternEnableBehaviorWidget::ResetPatternBehaviorEnable);

	shootRandom2DCB->OnCheckStateChanged.AddUniqueDynamic(this, &UEditor_PatternEnableBehaviorWidget::Random2DChanged);
	shootRandom3DCB->OnCheckStateChanged.AddUniqueDynamic(this, &UEditor_PatternEnableBehaviorWidget::Random3DChanged);
}
void UEditor_PatternEnableBehaviorWidget::InitializeComponents(const FPatternData& _patternData)
{
	//Rotation
	UMiscLibrary::SetCheckBoxState(rotationAngleCB, _patternData.GetAngleState());
	UMiscLibrary::SetCheckBoxState(rotationInstantCB, _patternData.GetInstantState());
	UMiscLibrary::SetCheckBoxState(rotationRotatorCB, _patternData.GetRotatorState());
	UMiscLibrary::SetCheckBoxState(rotationTargetCB, _patternData.GetTargetState());
	//Movement
	UMiscLibrary::SetCheckBoxState(movementOnAxeCB, _patternData.GetMovementOnAxeState());
	UMiscLibrary::SetCheckBoxState(movementToTargetCB, _patternData.GetMovementToTargetState());
	UMiscLibrary::SetCheckBoxState(movementTeleportCB, _patternData.GetMovementTeleportState());
	//Shoot
	UMiscLibrary::SetCheckBoxState(shootPointCB, _patternData.GetShootPointState());
	UMiscLibrary::SetCheckBoxState(shootLineCB, _patternData.GetShootLineState());
	UMiscLibrary::SetCheckBoxState(shootArcCB, _patternData.GetShootArcState());
	UMiscLibrary::SetCheckBoxState(shootCircleCB, _patternData.GetShootCircleState());
	UMiscLibrary::SetCheckBoxState(shootCrossCB, _patternData.GetShootCrossState());
	UMiscLibrary::SetCheckBoxState(shootRandom2DCB, _patternData.GetShootRandom2DState());
	Random2DChanged(_patternData.GetShootRandom2DState());
	UMiscLibrary::SetCheckBoxState(shootRandom3DCB, _patternData.GetShootRandom3DState());
	Random3DChanged(_patternData.GetShootRandom3DState());
	//Laser
	UMiscLibrary::SetCheckBoxState(laserCB, _patternData.GetLaserState());
	//Wave
	UMiscLibrary::SetCheckBoxState(waveCB, _patternData.GetOverrideWave());
	//Zone
	UMiscLibrary::SetCheckBoxState(zoneScalingCB, _patternData.GetScalingState());
	UMiscLibrary::SetCheckBoxState(zoneSpawnerDefaultCB, _patternData.GetSpawnerDefaultState());
	UMiscLibrary::SetCheckBoxState(zoneSpawnerConcentricCB, _patternData.GetSpawnerConcentricState());
	UMiscLibrary::SetCheckBoxState(zoneSpawnerMultiCB, _patternData.GetSpawnerMultiState());
}

void UEditor_PatternEnableBehaviorWidget::InitializePatternEnableBehaviorWidget(const int& _serieElementIndex, const int& _patternIndex, const FPatternData& _patternData)
{
	serieElementIndex = _serieElementIndex;
	patternIndex = _patternIndex;

	patternDataBackup = _patternData;
	patternDataOverride = _patternData;

	InitializeComponents(patternDataOverride);
	InitializeVisibility();
}
void UEditor_PatternEnableBehaviorWidget::ResetPatternEnableBehaviorWidget(const FPatternData& _patternData)
{
	patternDataOverride = _patternData;

	InitializeComponents(patternDataOverride);
}

void UEditor_PatternEnableBehaviorWidget::SavePatternBehaviorEnable()
{
	//Rotation
	patternDataOverride.SetAngleState(UMiscLibrary::GetCheckBoxValue(rotationAngleCB));
	patternDataOverride.SetInstantState(UMiscLibrary::GetCheckBoxValue(rotationInstantCB));
	patternDataOverride.SetRotatorState(UMiscLibrary::GetCheckBoxValue(rotationRotatorCB));
	patternDataOverride.SetTargetState(UMiscLibrary::GetCheckBoxValue(rotationTargetCB));
	//Movement
	patternDataOverride.SetMovementOnAxeState(UMiscLibrary::GetCheckBoxValue(movementOnAxeCB));
	patternDataOverride.SetMovementToTargetState(UMiscLibrary::GetCheckBoxValue(movementToTargetCB));
	patternDataOverride.SetMovementTeleportState(UMiscLibrary::GetCheckBoxValue(movementTeleportCB));
	//Shoot
	patternDataOverride.SetShootPointState(UMiscLibrary::GetCheckBoxValue(shootPointCB));
	patternDataOverride.SetShootLineState(UMiscLibrary::GetCheckBoxValue(shootLineCB));
	patternDataOverride.SetShootArcState(UMiscLibrary::GetCheckBoxValue(shootArcCB));
	patternDataOverride.SetShootCircleState(UMiscLibrary::GetCheckBoxValue(shootCircleCB));
	patternDataOverride.SetShootCrossState(UMiscLibrary::GetCheckBoxValue(shootCrossCB));
	patternDataOverride.SetShootRandom2DState(UMiscLibrary::GetCheckBoxValue(shootRandom2DCB));
	patternDataOverride.SetShootRandom3DState(UMiscLibrary::GetCheckBoxValue(shootRandom3DCB));
	//Laser
	patternDataOverride.SetLaserState(UMiscLibrary::GetCheckBoxValue(laserCB));
	//Wave
	patternDataOverride.SetOverrideWave(UMiscLibrary::GetCheckBoxValue(waveCB));
	//Zone
	patternDataOverride.SetScalingState(UMiscLibrary::GetCheckBoxValue(zoneScalingCB));
	patternDataOverride.SetSpawnerDefaultState(UMiscLibrary::GetCheckBoxValue(zoneSpawnerDefaultCB));
	patternDataOverride.SetSpawnerConcentricState(UMiscLibrary::GetCheckBoxValue(zoneSpawnerConcentricCB));
	patternDataOverride.SetSpawnerMultiState(UMiscLibrary::GetCheckBoxValue(zoneSpawnerMultiCB));	
	
	onSavePatternEnableBehavior.Broadcast(serieElementIndex, patternIndex, patternDataOverride);
}
void UEditor_PatternEnableBehaviorWidget::CancelPatternBehaviorEnable()
{
	patternDataOverride = patternDataBackup;
	InitializePatternEnableBehaviorWidget(serieElementIndex, patternIndex, patternDataBackup);
	
	onSavePatternEnableBehavior.Broadcast(serieElementIndex, patternIndex, patternDataBackup);
}
void UEditor_PatternEnableBehaviorWidget::ResetPatternBehaviorEnable()
{
	onResetPatternEnableBehavior.Broadcast(serieElementIndex, patternIndex);
}

void UEditor_PatternEnableBehaviorWidget::Random2DChanged(bool _state)
{
	UMiscLibrary::SetWidgetVisibility(shootRandom3DHB, !_state);
}
void UEditor_PatternEnableBehaviorWidget::Random3DChanged(bool _state)
{
	UMiscLibrary::SetWidgetVisibility(shootRandom2DHB, !_state);
}

void UEditor_PatternEnableBehaviorWidget::InitializeVisibility()
{
	const EPatternDataTableType& _type = patternDataBackup.GetPatternDataTableType();

	HideAll();

	if (_type == PATTERN_SHOOT)
	{
		ShowPanel(rotationVB);
		ShowPanel(movementVB);
		ShowPanel(shootVB);

		rotationEnableVB->AddChildToVerticalBox(rotationVB);
		movementEnableVB->AddChildToVerticalBox(movementVB);
		specificEnableVB->AddChildToVerticalBox(shootVB);
	}

	if (_type == PATTERN_LASER)
	{
		ShowPanel(rotationVB);
		ShowPanel(movementVB);
		ShowPanel(laserHB);

		rotationEnableVB->AddChildToVerticalBox(rotationVB);
		movementEnableVB->AddChildToVerticalBox(movementVB);
		specificEnableVB->AddChildToVerticalBox(laserHB);
	}

	if (_type == PATTERN_WAVE)
	{
		ShowPanel(waveHB);

		specificEnableVB->AddChildToVerticalBox(waveHB);
	}

	if (_type == PATTERN_ZONE)
	{
		ShowPanel(rotationVB);
		ShowPanel(movementVB);
		ShowPanel(zoneVB);

		rotationEnableVB->AddChildToVerticalBox(rotationVB);
		movementEnableVB->AddChildToVerticalBox(movementVB);
		specificEnableVB->AddChildToVerticalBox(zoneVB);
	}

	if (_type == PATTERN_ARENA_CLOSER)
	{
		//Arena Closer doesn't have boolean to display		
	}
}

void UEditor_PatternEnableBehaviorWidget::HideAll()
{
	HidePanel(rotationVB);
	HidePanel(movementVB);
	HidePanel(shootVB);
	HidePanel(laserHB);
	HidePanel(waveHB);
	HidePanel(zoneVB);

	movementEnableVB->ClearChildren();
	rotationEnableVB->ClearChildren();
	specificEnableVB->ClearChildren();
}

void UEditor_PatternEnableBehaviorWidget::HidePanel(UPanelWidget* _panel) const
{
	if (!_panel) return;

	_panel->SetVisibility(ESlateVisibility::Hidden);
}
void UEditor_PatternEnableBehaviorWidget::ShowPanel(UPanelWidget* _panel) const
{
	if (!_panel) return;

	_panel->SetVisibility(ESlateVisibility::Visible);
}
#pragma endregion

#pragma region Pattern Behavior Settings
void UEditor_PatternBehaviorSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeWidget();
	InitializeWidgets();	
}
void UEditor_PatternBehaviorSettingsWidget::InitializeWidget()
{
	saveButton->OnClicked.AddDynamic(this, &UEditor_PatternBehaviorSettingsWidget::SavePatternBehaviorSettings);
	cancelButton->OnClicked.AddDynamic(this, &UEditor_PatternBehaviorSettingsWidget::CancelPatternBehaviorSettings);
	resetButton->OnClicked.AddDynamic(this, &UEditor_PatternBehaviorSettingsWidget::ResetPatternBehaviorSettings);
}
void UEditor_PatternBehaviorSettingsWidget::InitializeWidgets()
{
	if (pb_RotationWidgetType.Get())
	{
		rotationWidget = CreateWidget<UEditor_PB_RotationWidget>(this, pb_RotationWidgetType);
		UCanvasPanelSlot* _slot = rotationCanvas->AddChildToCanvas(rotationWidget);
		_slot->SetOffsets(FMargin(0, 0, 0, 0));
		_slot->SetAnchors(FAnchors(0, 0, 1, 1));
	}
	else
		LOG("UEditor_PatternBehaviorSettingsWidget::InitializeWidgets => PB Rotation Widget Type is not valid.");
	
	if (pb_MovementWidgetType.Get())
	{
		movementWidget = CreateWidget<UEditor_PB_MovementWidget>(this, pb_MovementWidgetType);
		UCanvasPanelSlot* _slot = movementCanvas->AddChildToCanvas(movementWidget);
		_slot->SetOffsets(FMargin(0, 0, 0, 0));
		_slot->SetAnchors(FAnchors(0, 0, 1, 1));
	}
	else
		LOG("UEditor_PatternBehaviorSettingsWidget::InitializeWidgets => PB Movement Widget Type is not valid.");

	if (pb_ShootWidgetType.Get())
	{
		shootWidget = CreateWidget<UEditor_PB_ShootWidget>(this, pb_ShootWidgetType);
		UCanvasPanelSlot* _slot = specificCanvas->AddChildToCanvas(shootWidget);
		_slot->SetOffsets(FMargin(0, 0, 0, 0));
		_slot->SetAnchors(FAnchors(0, 0, 1, 1));
	}
	else
		LOG("UEditor_PatternBehaviorSettingsWidget::InitializeWidgets => PB Shoot Widget Type is not valid.");

	if (pb_LaserWidgetType.Get())
	{
		laserWidget = CreateWidget<UEditor_PB_LaserWidget>(this, pb_LaserWidgetType);
		UCanvasPanelSlot* _slot = specificCanvas->AddChildToCanvas(laserWidget);
		_slot->SetOffsets(FMargin(0, 0, 0, 0));
		_slot->SetAnchors(FAnchors(0, 0, 1, 1));
	}
	else
		LOG("UEditor_PatternBehaviorSettingsWidget::InitializeWidgets => PB Laser Widget Type is not valid.");

	if (pb_WaveWidgetType.Get())
	{
		waveWidget = CreateWidget<UEditor_PB_WaveWidget>(this, pb_WaveWidgetType);
		UCanvasPanelSlot* _slot = specificCanvas->AddChildToCanvas(waveWidget);
		_slot->SetOffsets(FMargin(0, 0, 0, 0));
		_slot->SetAnchors(FAnchors(0, 0, 1, 1));
	}
	else
		LOG("UEditor_PatternBehaviorSettingsWidget::InitializeWidgets => PB Wave Widget Type is not valid.");

	if (pb_ZoneWidgetType.Get())
	{
		zoneWidget = CreateWidget<UEditor_PB_ZoneWidget>(this, pb_ZoneWidgetType);
		UCanvasPanelSlot* _slot = specificCanvas->AddChildToCanvas(zoneWidget);
		_slot->SetOffsets(FMargin(0, 0, 0, 0));
		_slot->SetAnchors(FAnchors(0, 0, 1, 1));
	}
	else
		LOG("UEditor_PatternBehaviorSettingsWidget::InitializeWidgets => PB Zone Widget Type is not valid.");

	if (pb_ArenaCloserWidgetType.Get())
	{
		arenaCloserWidget = CreateWidget<UEditor_PB_ArenaCloserWidget>(this, pb_ArenaCloserWidgetType);
		UCanvasPanelSlot* _slot = specificCanvas->AddChildToCanvas(arenaCloserWidget);
		_slot->SetOffsets(FMargin(0, 0, 0, 0));
		_slot->SetAnchors(FAnchors(0, 0, 1, 1));
	}
	else
		LOG("UEditor_PatternBehaviorSettingsWidget::InitializeWidgets => PB Arena Closer Widget Type is not valid.");
}

void UEditor_PatternBehaviorSettingsWidget::InitializePatternBehaviorWidgets(const FPatternData& _patternData)
{	
	rotationWidget->InitializeRotationWidget(_patternData);
	movementWidget->InitializeMovementWidget(_patternData, arenaDataTable);
	shootWidget->InitializeShootWidget(_patternData);
	laserWidget->InitializeLaserWidget(_patternData);
	waveWidget->InitializeWaveWidget(_patternData);
	zoneWidget->InitializeZoneWidget(_patternData);
	arenaCloserWidget->InitializeArenaCloserWidget(_patternData, arenaDataTable);
}

void UEditor_PatternBehaviorSettingsWidget::InitializePatternBehaviorSettingsWidget(const int& _serieElementIndex, const int& _patternIndex, const FPatternData& _patternData, UA_DT_ArenaDataTable* _arenaDataTable)
{
	serieElementIndex = _serieElementIndex;
	patternIndex = _patternIndex;

	arenaDataTable = _arenaDataTable;
	
	patternDataBackup = _patternData;
	patternDataOverride = _patternData;

	InitializePatternBehaviorWidgets(patternDataOverride);
	InitializeVisibility();
}
void UEditor_PatternBehaviorSettingsWidget::ResetPatternBehaviorSettingsWidget(const FPatternData& _patternData)
{
	patternDataOverride = _patternData;

	//Rotation
	patternDataOverride.SetAngleState(patternDataBackup.GetAngleState());
	patternDataOverride.SetInstantState(patternDataBackup.GetInstantState());
	patternDataOverride.SetRotatorState(patternDataBackup.GetRotatorState());
	patternDataOverride.SetTargetState(patternDataBackup.GetTargetState());
	//Movement
	patternDataOverride.SetMovementOnAxeState(patternDataBackup.GetMovementOnAxeState());
	patternDataOverride.SetMovementToTargetState(patternDataBackup.GetMovementToTargetState());
	patternDataOverride.SetMovementTeleportState(patternDataBackup.GetMovementTeleportState());
	//Shoot
	patternDataOverride.SetShootPointState(patternDataBackup.GetShootPointState());
	patternDataOverride.SetShootLineState(patternDataBackup.GetShootLineState());
	patternDataOverride.SetShootArcState(patternDataBackup.GetShootArcState());
	patternDataOverride.SetShootCircleState(patternDataBackup.GetShootCircleState());
	patternDataOverride.SetShootCrossState(patternDataBackup.GetShootCrossState());
	patternDataOverride.SetShootRandom2DState(patternDataBackup.GetShootRandom2DState());
	patternDataOverride.SetShootRandom3DState(patternDataBackup.GetShootRandom3DState());
	//Laser
	patternDataOverride.SetLaserState(patternDataBackup.GetLaserState());
	//Wave
	patternDataOverride.SetOverrideWave(patternDataBackup.GetOverrideWave());
	//Zone
	patternDataOverride.SetScalingState(patternDataBackup.GetScalingState());
	patternDataOverride.SetSpawnerDefaultState(patternDataBackup.GetSpawnerDefaultState());
	patternDataOverride.SetSpawnerConcentricState(patternDataBackup.GetSpawnerConcentricState());
	patternDataOverride.SetSpawnerMultiState(patternDataBackup.GetSpawnerMultiState());	

	InitializePatternBehaviorWidgets(patternDataOverride);
	InitializeVisibility();
}

void UEditor_PatternBehaviorSettingsWidget::SavePatternBehaviorSettings()
{
	rotationWidget->GetRotationOverride(patternDataOverride);
	movementWidget->GetMovementOverride(patternDataOverride);
	shootWidget->GetShootOverride(patternDataOverride);
	laserWidget->GetLaserOverride(patternDataOverride);
	waveWidget->GetWaveOverride(patternDataOverride);
	zoneWidget->GetZoneOverride(patternDataOverride);
	arenaCloserWidget->GetArenaCloserOverride(patternDataOverride);
	
	onSavePatternBehaviorSettings.Broadcast(serieElementIndex, patternIndex, patternDataOverride);	
}
void UEditor_PatternBehaviorSettingsWidget::CancelPatternBehaviorSettings()
{
	patternDataOverride = patternDataBackup;
	InitializePatternBehaviorSettingsWidget(serieElementIndex, patternIndex, patternDataBackup, arenaDataTable);
	
	onSavePatternBehaviorSettings.Broadcast(serieElementIndex, patternIndex, patternDataBackup);
}
void UEditor_PatternBehaviorSettingsWidget::ResetPatternBehaviorSettings()
{
	onResetPatternBehaviorSettings.Broadcast(serieElementIndex, patternIndex);
}

void UEditor_PatternBehaviorSettingsWidget::InitializeVisibility()
{
	const EPatternDataTableType& _type = patternDataBackup.GetPatternDataTableType();

	HideAll();

	if (_type == PATTERN_SHOOT)
	{
		ShowWidget(rotationWidget);
		ShowWidget(movementWidget);
		ShowWidget(shootWidget);
	}

	if (_type == PATTERN_LASER)
	{
		ShowWidget(rotationWidget);
		ShowWidget(movementWidget);
		ShowWidget(laserWidget);
	}

	if (_type == PATTERN_WAVE)
	{
		ShowWidget(waveWidget);
	}

	if (_type == PATTERN_ZONE)
	{
		ShowWidget(rotationWidget);
		ShowWidget(movementWidget);
		ShowWidget(zoneWidget);
	}

	if (_type == PATTERN_ARENA_CLOSER)
	{
		ShowWidget(arenaCloserWidget);
	}
}

void UEditor_PatternBehaviorSettingsWidget::HideAll()
{
	HideWidget(rotationWidget);
	HideWidget(movementWidget);
	HideWidget(shootWidget);
	HideWidget(laserWidget);
	HideWidget(waveWidget);
	HideWidget(zoneWidget);
	HideWidget(arenaCloserWidget);
}

void UEditor_PatternBehaviorSettingsWidget::HideWidget(UWidget* _widget)
{
	if (!_widget) return;
	_widget->SetVisibility(ESlateVisibility::Hidden);
}
void UEditor_PatternBehaviorSettingsWidget::ShowWidget(UWidget* _widget)
{
	if (!_widget) return;
	_widget->SetVisibility(ESlateVisibility::Visible);
}
#pragma endregion 
