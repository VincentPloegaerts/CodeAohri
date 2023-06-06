#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Editor_OverrideWidgets.h"

#include "STRUCT_SerieData.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"

#include "Editor_OverrideSerieWidget.generated.h"

class UA_DT_ArenaDataTable;

UCLASS()
class INSIDEUS_API UEditor_OverrideSerieWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveAll, FSerieData, _overrideData);

#pragma region Data
	UPROPERTY()
	UA_DT_ArenaDataTable* arenaDataTable = nullptr;
	
	FSerieData serieDataBackup;
	FSerieData serieDataOverride;
#pragma endregion

#pragma region Components
	UPROPERTY(meta = (BindWidget))	//Broadcast the (saved) Override Data 
	UButton* saveButton = nullptr; 
	UPROPERTY(meta = (BindWidget))	//Broadcast the Backup Data
	UButton* cancelButton = nullptr;

#pragma endregion

#pragma region Widget
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* widgetsCanvas = nullptr;

	#pragma region Serie
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_SerieWidget> serieWidgetType;
	UPROPERTY()
	UEditor_SerieWidget* serieWidget = nullptr;
	#pragma endregion

	#pragma region Serie Element Data
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_SerieElementDataWidget> serieElementDataWidgetType;
	UPROPERTY()
	UEditor_SerieElementDataWidget* serieElementDataWidget = nullptr;
	#pragma endregion 

	#pragma region Spawn Settings
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_SpawnSettingsWidget> spawnSettingsWidgetType;
	UPROPERTY()
	UEditor_SpawnSettingsWidget* spawnSettingsWidget = nullptr;
	#pragma endregion

	#pragma region Pattern Enable Behavior
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PatternEnableBehaviorWidget> patternEnableBehaviorWidgetType;
	UPROPERTY()
	UEditor_PatternEnableBehaviorWidget* patternEnableBehaviorWidget = nullptr;
	#pragma endregion

	#pragma region Pattern Behavior
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PatternBehaviorSettingsWidget> patternBehaviorSettingsWidgetType;
	UPROPERTY()
	UEditor_PatternBehaviorSettingsWidget* patternBehaviorSettingsWidget = nullptr;
	#pragma endregion
#pragma endregion

	UPROPERTY()
	FOnSaveAll onSaveAll;
public:
	FORCEINLINE FOnSaveAll& OnSaveAll() { return onSaveAll; }

public:
	//Called By the TN (or other for test only). First Function Called
	void InitializeOverrideData(const FSerieData& _toOverride, UA_DT_ArenaDataTable* _arenaDataTable = nullptr);
	
private:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	//Init event
	void InitializeOverrideWidget();
	//Create all required widget
	void InitializeWidgets();

#pragma region Save
	UFUNCTION() void SetSerieElementData(int _serieElementIndex, FSerieElementData _serieElementData);
	UFUNCTION() void SetSpawnSettings(int _serieElementIndex, int _patternIndex, FSpawnSettings _spawnSettings);
	UFUNCTION() void SetPatternEnableBehavior(int _serieElementIndex, int _patternIndex, FPatternData _patternData);
	UFUNCTION() void SetPatternBehaviorSettings(int _serieElementIndex, int _patternIndex, FPatternData _patternData);
	
	//Broadcast Override data
	UFUNCTION() void SaveAllData();
	//Broadcast Backup data and reset Override Data with Backup
	UFUNCTION() void CancelAllData();
#pragma endregion

#pragma region Reset
	UFUNCTION() void ResetSerieElementData(int _serieElementIndex);
	UFUNCTION() void ResetSpawnSettings(int _serieElementIndex, int _patternIndex);
	UFUNCTION() void ResetPatternEnableBehavior(int _serieElementIndex, int _patternIndex);
	UFUNCTION() void ResetPatternBehaviorSettings(int _serieElementIndex, int _patternIndex);
#pragma endregion

#pragma region Navigation
	UFUNCTION() void OpenSerieWidget();
		
	UFUNCTION() void OpenSerieElementDataWidget(int _serieElementIndex);
	UFUNCTION() void CloseSerieElementDataWidget();

	UFUNCTION() void OpenSpawnSettingsWidget(int _serieElementIndex, int _patternIndex);
	UFUNCTION() void CloseSpawnSettingsWidget();

	UFUNCTION() void OpenPatternEnableBehaviorWidget(int _serieElementIndex, int _patternIndex);
	UFUNCTION() void ClosePatternEnableBehaviorWidget();

	UFUNCTION() void OpenPatternBehaviorSettingsWidget(int _serieElementIndex, int _patternIndex);
	UFUNCTION() void ClosePatternBehaviorSettingsWidget();
#pragma endregion

#pragma region Visibility
	void HideAllWidget() const;
	
	void HideWidget(UWidget* _panel) const;
	void ShowWidget(UWidget* _panel) const;
#pragma  endregion
};