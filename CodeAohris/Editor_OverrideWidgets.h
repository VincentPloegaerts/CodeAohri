#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "STRUCT_SerieElementData.h"

#include "Editor_PatternBehaviorWidgets.h"

#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"
#include "Components/CheckBox.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/SpinBox.h"

#include "Editor_OverrideWidgets.generated.h"

//
UCLASS()
class UMiscLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static void InitComboBoxFromEnum(const FString& _enumName, UComboBoxString* _comboBox);
	static void InitComboBoxFromNames(const TArray<FString>& _names, UComboBoxString* _comboBox);
	
	static void SetCheckBoxState(UCheckBox* _checkBox, const bool& _state);
	static bool GetCheckBoxValue(const UCheckBox* _checkBox);
	
	static void SetWidgetVisibility(UWidget* _widget, const bool& _visibility);
	
	static void SetSpinBoxFromVector(USpinBox* _x, USpinBox* _y, USpinBox* _z, const FVector& _vector);
	static FVector GetVectorFromSpinBox(USpinBox* _x, USpinBox* _y, USpinBox* _z);

	static void SetSpinBoxFromRotator(USpinBox* _pitch, USpinBox* _yaw, USpinBox* _roll, const FRotator& _rotator);
	static FRotator GetRotatorFromSpinBox(USpinBox* _pitch, USpinBox* _yaw, USpinBox* _roll);
	
	static int GetEnumValueFromString(const FString& _enumValue, const FString& _enumName);
	template<typename EnumType>
	static FString GetEnumValueAsString(EnumType _enum)
	{
		return *UEnum::GetValueAsString<EnumType>(_enum);
	}
};

//Class used for the display of a Serie Element
UCLASS()
class INSIDEUS_API UEditor_SerieElementWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelected, UEditor_SerieElementWidget*, _elementSelected);

#pragma region Components
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* serieText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* serieOverrideButton = nullptr;
	UPROPERTY(meta = (BindWidget)) 
	UTextBlock* patternInfoText = nullptr;
#pragma endregion 

#pragma region Data
	UPROPERTY()
	int serieElementIndex = 0;
#pragma endregion 
	
	UPROPERTY()
	FOnSelected onSelected;

public:
	FOnSelected& OnSelected() { return onSelected; }

	const int& Index() const { return serieElementIndex; }
	
private:
	virtual void NativeConstruct() override;

	UFUNCTION() void SerieElementSelected();
	
public:
	void InitializeSerieElementWidget(const int& _serieElementIndex, const TArray<FPatternElementData>& patternsElementData);
};

//Class used for the display of the serie list element 
UCLASS()
class INSIDEUS_API UEditor_SerieWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSerieElementSelected, int, _serieIndex);
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_SerieElementWidget> serieElementWidgetType;

#pragma region Components
protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* serieVerticalBox = nullptr;
#pragma endregion
	
	UPROPERTY()
	FOnSerieElementSelected onSerieElementSelected;

	
public:
	FORCEINLINE FOnSerieElementSelected& OnSerieElementSelected() { return onSerieElementSelected; }
	
public:
	void InitializeSerieWidget(const TArray<FSerieElementData>& _serieElementData);

private:
	UFUNCTION() void SerieElementSelected(UEditor_SerieElementWidget* _selection);
};

//Class used for the display of the Serie Element Data Pattern list
UCLASS()
class INSIDEUS_API UEditor_SerieElementDataPatternWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEditSpawnSettings, int, _patternIndex);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEditBehaviorEnable, int, _patternIndex);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEditBehavior, int, _patternIndex);

#pragma region Components
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* patternText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* editSpawnSettingsButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* editBehaviorEnableButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* editBehaviorButton = nullptr;
#pragma endregion

#pragma region Data
	UPROPERTY()
	int patternIndex = 0;
#pragma endregion

	UPROPERTY()
	FOnEditSpawnSettings onEditSpawnSettings;
	UPROPERTY()
	FOnEditBehaviorEnable onEditBehaviorEnable;
	UPROPERTY()
	FOnEditBehavior onEditBehavior;

public:
	FORCEINLINE FOnEditSpawnSettings& OnEditSpawnSettings() { return onEditSpawnSettings; }
	FORCEINLINE FOnEditBehaviorEnable& OnEditBehaviorEnable() { return onEditBehaviorEnable; }
	FORCEINLINE FOnEditBehavior& OnEditBehavior() { return onEditBehavior; }
	
private:
	virtual void NativeConstruct() override;
	void InitializeWidget();
	
public:
	void InitializeSerieElementDataPatternWidget(const int& _patternIndex, const FString& _patternName);

private:
	UFUNCTION() void EditSpawnSettings();
	UFUNCTION() void EditBehaviorEnable();
	UFUNCTION() void EditBehavior();
};

//Class used for the display and override of a Serie Element Data
UCLASS()
class INSIDEUS_API UEditor_SerieElementDataWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSaveSerieElementData, int, _elementIndex, FSerieElementData, _serieElementData);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResetSerieElementData, int, _elementIndex);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEditPatternSpawnSettings, int, _serieIndex, int, _patternIndex);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEditPatternBehaviorEnable, int, _serieIndex, int, _patternIndex);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEditPatternBehavior, int, _serieIndex, int, _patternIndex);
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_SerieElementDataPatternWidget> patternWidgetType;

#pragma region Components
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* saveButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* cancelButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* resetButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* backButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* patternsVerticalBox = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* delaySpinBox = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* waitCheckBox = nullptr;
#pragma endregion

#pragma region Data
	UPROPERTY()
	int serieElementIndex = 0;
	
	UPROPERTY()
	FSerieElementData serieElementDataBackup;
	UPROPERTY()
	FSerieElementData serieElementOverride;
#pragma endregion
	
	UPROPERTY()
	FOnSaveSerieElementData onSaveSerieElementData;
	UPROPERTY()
	FOnResetSerieElementData onResetSerieElementData;

	UPROPERTY()
	FOnEditPatternSpawnSettings onEditPatternSpawnSettings;
	UPROPERTY()
	FOnEditPatternBehaviorEnable onEditPatternBehaviorEnable;	
	UPROPERTY()
	FOnEditPatternBehavior onEditPatternBehavior;

public:
	FORCEINLINE FOnSaveSerieElementData& OnSaveSerieElementData() { return onSaveSerieElementData; }
	FORCEINLINE FOnResetSerieElementData& OnResetSerieElementData() { return onResetSerieElementData; }

	FORCEINLINE FOnEditPatternSpawnSettings& OnEditPatternSpawnSettings() { return onEditPatternSpawnSettings; }
	FORCEINLINE FOnEditPatternBehaviorEnable& OnEditPatternBehaviorEnable() { return onEditPatternBehaviorEnable; }
	FORCEINLINE FOnEditPatternBehavior& OnEditPatternBehavior() { return onEditPatternBehavior; }
	
	FORCEINLINE UButton* BackButton() { return backButton; }

	FORCEINLINE const int& GetSerieElementIndex() const { return serieElementIndex; }

public:
	void InitializeSerieElementDataWidget(const int& _serieElementIndex, const FSerieElementData& _serieElementData);
	void ResetSerieElementDataWidget(const FSerieElementData& _serieElementData);
	
private:
	virtual void NativeConstruct() override;
	void InitializeWidget();

	//Save Serie Element Data 
	UFUNCTION() void SaveSerieElementData();
	//Cancel modified Element Data (since the Serie Element Data Widget have been Initialize)
	UFUNCTION() void CancelSerieElementData();
	//Reset all Element Data (since the Override Widget have been Initialize)
	UFUNCTION() void ResetSerieElementData();

	UFUNCTION() void EditPatternSpawnSettings(int _patternIndex);
	UFUNCTION() void EditPatternBehaviorEnable(int _patternIndex);
	UFUNCTION() void EditPatternBehavior(int _patternIndex);
};

//Class used for the RandomArenaPoint property
UCLASS()
class INSIDEUS_API UEditor_PointNameElementWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemoveElement, int, _elementIndex);

#pragma region Components
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* nameCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* removeButton = nullptr;
#pragma endregion

#pragma region Data
	UPROPERTY()
	int elementIndex = 0;
#pragma endregion

	UPROPERTY()	
	FOnRemoveElement onRemoveElement;

public:
	FORCEINLINE FOnRemoveElement& OnRemoveElement() { return onRemoveElement; }

	FORCEINLINE const int& GetElementIndex() const { return elementIndex; } 
    FORCEINLINE FString GetElementName() const { return nameCB->GetSelectedOption(); }
    
private:
	virtual void NativeConstruct() override;
	void InitializeWidget();

public:
	void InitializePointNameElementWidget(const int& _elementIndex, const TArray<FString>& _names, const FString& _defaultName = "");
	
	void SetElementIndex(const int& _elementIndex);
	void SetElementName(const FString& _name);
private:
	UFUNCTION() void RemoveElement();
};

//Class used for the display and override of the Spawn Settings 
UCLASS()
class INSIDEUS_API UEditor_SpawnSettingsWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSaveSpawnSettings, int, _serieElementIndex, int, _patternIndex, FSpawnSettings, _spawnSettings);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResetSpawnSettings, int, _serieElementIndex, int, _patternIndex);

	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PointNameElementWidget> pointNameElementWidgetType;

#pragma region Components
	//Visibility Management
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* randomHB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* spawnArenaPointNameHB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* randomArenaPointsHB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* spawnDirectionHB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* directionArenaPointNameHB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* spawnLocalDirectionOffsetHB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* lookTargetOffsetHB = nullptr;

	//Property
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* spawnPositionCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* randomCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* spawnArenaPointNameCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* randomRangeSB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* xOffsetSB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* yOffsetSB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* zOffsetSB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* spawnDirectionModeCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* spawnDirectionCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* directionArenaPointNameCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* localPitchSB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* localYawSB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* localRollSB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* xLookSB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* yLookSB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* zLookSB = nullptr;	

	//List Names
	UPROPERTY(meta = (BindWidget))	//Anchor
	UVerticalBox* nameListVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* addButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* clearButton = nullptr;
	
	//Save
	UPROPERTY(meta = (BindWidget))
	UButton* saveButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* cancelButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* resetButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* backButton = nullptr;
#pragma endregion

#pragma region Data
	TArray<FString> pointNames;
	
	int serieElementIndex = 0;
	int patternIndex = 0;
	
	FSpawnSettings spawnSettingsBackup;
	FSpawnSettings spawnSettingsOverride;
#pragma endregion

	UPROPERTY()
	FOnSaveSpawnSettings onSaveSpawnSettings;
	UPROPERTY()
	FOnResetSpawnSettings onResetSpawnSettings;
	
public:
	FORCEINLINE FOnSaveSpawnSettings& OnSaveSpawnSettings() { return onSaveSpawnSettings; }
	FORCEINLINE FOnResetSpawnSettings& OnResetSpawnSettings() { return onResetSpawnSettings; }
	

	FORCEINLINE UButton* BackButton() { return backButton; }
private:
	virtual void NativeConstruct() override;
	void InitializeWidget();
	void InitializeComboBox() const;

	void InitComboBoxFromEnum(const FString& _enumName, UComboBoxString* _comboBox) const;
	void InitComboBoxFromNames(const TArray<FString>& _names, UComboBoxString* _comboBox) const;
	int GetEnumValueFromString(const FString& _value, const FString& _enumName) const;

	void InitializeComponents(const FSpawnSettings& _spawnSettings);
	
	void InitNameList(const TArray<FString>& _names);
	TArray<FString> GetNameList() const;
	
public:
	void InitializeSpawnSettingsWidget(int _serieElementIndex, int _patternIndex, const FSpawnSettings& _spawnSettings);
	void InitializeSpawnSettingsWidgetUtil(const UA_DT_ArenaDataTable* arenaDataTable);
	void ResetSpawnSettingsWidget(const FSpawnSettings& _spawnSettings);
	
private:
	UFUNCTION() void SaveSpawnSettings();
	UFUNCTION() void CancelSpawnSettings();
	UFUNCTION() void ResetSpawnSettings();

	UFUNCTION() void AddElementToNameList();
	UFUNCTION() void RemoveElementFromList(int _elementIndex);
	UFUNCTION() void ClearNameList();


#pragma region Visibility
	void RefreshDisplay();

	UFUNCTION() void RefreshRandom(bool _randomState);
	UFUNCTION() void RefreshSpawnPosition(FString _option, ESelectInfo::Type _selectInfo);
	UFUNCTION() void RefreshSpawnDirectionMode(FString _option, ESelectInfo::Type _selectInfo);
	UFUNCTION() void RefreshSpawnDirection(FString _option, ESelectInfo::Type _selectInfo);
#pragma endregion 
};

//Class used for the display and override of the Pattern Enable Behavior state
UCLASS()
class INSIDEUS_API UEditor_PatternEnableBehaviorWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSavePatternEnableBehavior, int, _serieElementIndex, int, _patternIndex, FPatternData, _patternData);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResetPatternEnableBehavior, int, _serieElementIndex, int, _patternIndex);

#pragma region Components
	#pragma region Pattern Data Properties
	//Rotation
	UPROPERTY(meta = (BindWidget))
	UCheckBox* rotationAngleCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* rotationInstantCB = nullptr;	
	UPROPERTY(meta = (BindWidget))
	UCheckBox* rotationRotatorCB = nullptr;	
	UPROPERTY(meta = (BindWidget))
	UCheckBox* rotationTargetCB = nullptr;
	//Movement
	UPROPERTY(meta = (BindWidget))
	UCheckBox* movementOnAxeCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* movementToTargetCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* movementTeleportCB = nullptr;
	//Shoot
	UPROPERTY(meta = (BindWidget))
	UCheckBox* shootPointCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* shootLineCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* shootArcCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* shootCircleCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* shootCrossCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* shootRandom2DCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* shootRandom3DCB = nullptr;
	//Laser
	UPROPERTY(meta = (BindWidget))
	UCheckBox* laserCB = nullptr;
	//Wave
	UPROPERTY(meta = (BindWidget))
	UCheckBox* waveCB = nullptr;	
	//Zone
	UPROPERTY(meta = (BindWidget))
	UCheckBox* zoneScalingCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* zoneSpawnerDefaultCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* zoneSpawnerConcentricCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* zoneSpawnerMultiCB = nullptr;
	//Arena Closer => Doesn't have boolean do display
	#pragma endregion

	//Visibility
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* shootRandom2DHB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* shootRandom3DHB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* movementVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* rotationVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* shootVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* laserHB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* waveHB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* zoneVB = nullptr;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* specificEnableVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* movementEnableVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* rotationEnableVB = nullptr;
	
	//Save
	UPROPERTY(meta = (BindWidget))
	UButton* saveButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* cancelButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* resetButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* backButton = nullptr;
#pragma endregion

#pragma region Data
	int serieElementIndex = 0;
	int patternIndex = 0;
	
	FPatternData patternDataBackup;
	FPatternData patternDataOverride;
#pragma endregion

	UPROPERTY()
	FOnSavePatternEnableBehavior onSavePatternEnableBehavior;
	UPROPERTY()
	FOnResetPatternEnableBehavior onResetPatternEnableBehavior;
	
public:
	FORCEINLINE FOnSavePatternEnableBehavior& OnSavePatternEnableBehavior() { return onSavePatternEnableBehavior; }
	FORCEINLINE FOnResetPatternEnableBehavior& OnResetPatternEnableBehavior() { return onResetPatternEnableBehavior; }

	FORCEINLINE UButton* BackButton() { return backButton; }

	FORCEINLINE const int& GetSerieElementIndex() const { return serieElementIndex; }
	
private:
	virtual void NativeConstruct() override;
	void InitializeWidget();
	void InitializeComponents(const FPatternData& _patternData);
	
public:
	void InitializePatternEnableBehaviorWidget(const int& _serieElementIndex, const int& _patternIndex, const FPatternData& _patternData);
	void ResetPatternEnableBehaviorWidget(const FPatternData& _patternData);
	
private:
	UFUNCTION() void SavePatternBehaviorEnable();
	UFUNCTION() void CancelPatternBehaviorEnable();
	UFUNCTION() void ResetPatternBehaviorEnable();

	UFUNCTION() void Random2DChanged(bool _state);
	UFUNCTION() void Random3DChanged(bool _state);

#pragma region Visibility
	void InitializeVisibility();
	
	void HideAll();

	void HidePanel(UPanelWidget* _panel) const;
	void ShowPanel(UPanelWidget* _panel) const;
#pragma endregion
};

//Class used for the display and override of the PatternBehavior Settings
UCLASS()
class INSIDEUS_API UEditor_PatternBehaviorSettingsWidget : public UUserWidget
{
	GENERATED_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSavePatternBehaviorSettings, int, _serieElementIndex, int, _patternIndex, FPatternData, _patternData);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResetPatternBehaviorSettings, int, _serieElementIndex, int, _patternIndex);

	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_RotationWidget> pb_RotationWidgetType;
	UPROPERTY()
	UEditor_PB_RotationWidget* rotationWidget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_MovementWidget> pb_MovementWidgetType;
	UPROPERTY()
	UEditor_PB_MovementWidget* movementWidget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_ShootWidget> pb_ShootWidgetType;
	UPROPERTY()
	UEditor_PB_ShootWidget* shootWidget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_LaserWidget> pb_LaserWidgetType;
	UPROPERTY()
	UEditor_PB_LaserWidget* laserWidget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_WaveWidget> pb_WaveWidgetType;
	UPROPERTY()
	UEditor_PB_WaveWidget* waveWidget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_ZoneWidget> pb_ZoneWidgetType;
	UPROPERTY()
	UEditor_PB_ZoneWidget* zoneWidget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_ArenaCloserWidget> pb_ArenaCloserWidgetType;
	UPROPERTY()
	UEditor_PB_ArenaCloserWidget* arenaCloserWidget = nullptr;

	#pragma region Components
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* rotationCanvas = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* movementCanvas = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* specificCanvas = nullptr;
	
	//Save
	UPROPERTY(meta = (BindWidget))
	UButton* saveButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* cancelButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* resetButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* backButton = nullptr;
#pragma endregion

#pragma region Data
	int serieElementIndex = 0;
	int patternIndex = 0;

	UPROPERTY()
	UA_DT_ArenaDataTable* arenaDataTable = nullptr;
	
	FPatternData patternDataBackup;
	FPatternData patternDataOverride;
#pragma endregion

	UPROPERTY()
	FOnSavePatternBehaviorSettings onSavePatternBehaviorSettings;
	UPROPERTY()
	FOnResetPatternBehaviorSettings onResetPatternBehaviorSettings;
	
public:
	FORCEINLINE FOnSavePatternBehaviorSettings& OnSavePatternBehaviorSettings() { return onSavePatternBehaviorSettings; }
	FORCEINLINE FOnResetPatternBehaviorSettings& OnResetPatternBehaviorSettings() { return onResetPatternBehaviorSettings; }
	
	FORCEINLINE UButton* BackButton() { return backButton; }

private:
	virtual void NativeConstruct() override;
	void InitializeWidget();
	void InitializeWidgets();

	void InitializePatternBehaviorWidgets(const FPatternData& _patternData);
	
public:
	void InitializePatternBehaviorSettingsWidget(const int& _serieElementIndex, const int& _patternIndex, const FPatternData& _patternData, UA_DT_ArenaDataTable* _arenaDataTable);
	void ResetPatternBehaviorSettingsWidget(const FPatternData& _patternData);

private:
	UFUNCTION() void SavePatternBehaviorSettings();
	UFUNCTION() void CancelPatternBehaviorSettings();
	UFUNCTION() void ResetPatternBehaviorSettings();

#pragma region Visibility
	void InitializeVisibility();
	
	void HideAll();

	void HideWidget(UWidget* _widget);
	void ShowWidget(UWidget* _widget);
#pragma endregion
};

/*Class Template
//Class used...
UCLASS()
class INSIDEUS_API UClassName : public UUserWidget
{
	GENERATED_BODY()
	
	//Widget Settings...
	
#pragma region Components
#pragma endregion

#pragma region Data
#pragma endregion

	//Event...
	
};
*/
