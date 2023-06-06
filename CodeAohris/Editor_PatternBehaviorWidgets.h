#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "STRUCT_PatternData.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/ExpandableArea.h"
#include "Components/HorizontalBox.h"
#include "Components/SpinBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

#include "Editor_PatternBehaviorWidgets.generated.h"

class UEditor_PointNameElementWidget;
class UA_DT_ArenaDataTable;

/*ROTATION*/

//Class used to display Rotation Angle Settings
UCLASS()
class INSIDEUS_API UEditor_PB_AngleWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Components
	UPROPERTY(meta = (BindWidget))
	UCheckBox* reverseCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* countSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* angleSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* axisCB = nullptr;
#pragma endregion

public:
	void InitializeAngleWidget(const FAngleSettings& _angleSettings);
	FAngleSettings GetAngleSettings() const;

private:
	virtual void NativeConstruct() override;
	void InitializeWidget();
};

//Class used to display Rotation Instant Settings
UCLASS()
class INSIDEUS_API UEditor_PB_InstantWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Components
	UPROPERTY(meta = (BindWidget))
	USpinBox* rotationSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* axisCB = nullptr;
#pragma endregion

public:
	void InitializeInstantWidget(const FInstantSettings& _instantSettings);
	FInstantSettings GetInstantSettings() const;
	
private:
	virtual void NativeConstruct() override;
	void InitializeWidget();
};

//Class used to display Rotation Rotator Settings
UCLASS()
class INSIDEUS_API UEditor_PB_RotatorWidget : public UUserWidget
{
	GENERATED_BODY()

#pragma region Components
	UPROPERTY(meta = (BindWidget))
	UCheckBox* clockwiseCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* yawCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* pitchCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* rollCB = nullptr;
#pragma endregion

public:
	void InitializeRotatorWidget(const FRotatorSettings& _rotatorSettings);
	FRotatorSettings GetRotatorSettings() const;
};

//Class used to display Rotation Target Settings
UCLASS()
class INSIDEUS_API UEditor_PB_TargetWidget : public UUserWidget
{
	GENERATED_BODY()

#pragma region Components
	UPROPERTY(meta = (BindWidget))
	UCheckBox* lerpCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* lookPitchPlayerCB = nullptr;
#pragma endregion

public:
	void InitializeTargetWidget(const FTargetSettings& _targetSettings);
	FTargetSettings GetTargetSettings() const;
};

//Class used to display Rotation Settings widgets
UCLASS()
class INSIDEUS_API UEditor_PB_RotationWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_AngleWidget> angleWidgetType;
	UPROPERTY()
	UEditor_PB_AngleWidget* angleWidget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_InstantWidget> instantWidgetType;
	UPROPERTY()
	UEditor_PB_InstantWidget* instantWidget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_RotatorWidget> rotatorWidgetType;
	UPROPERTY()
	UEditor_PB_RotatorWidget* rotatorWidget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_TargetWidget> targetWidgetType;
	UPROPERTY()
	UEditor_PB_TargetWidget* targetWidget = nullptr;

#pragma region Components
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* rotationVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* rotationSpeedHB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* rotationSpeedSP = nullptr;
#pragma endregion

public:
	void InitializeRotationWidget(const FPatternData& _patternData);
	void GetRotationOverride(FPatternData& _patternData);
	
private:
	virtual void NativeConstruct() override;
	void InitializeWidgets();

	void InitializeVisibility(const FPatternData& _patternData);
	void UpdateWidget(UWidget* _widget, const bool& _visible) const;
};

/*MOVEMENT*/

//Class used to display Movement On Axe Settings
UCLASS()
class INSIDEUS_API UEditor_PB_OnAxeWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Components
	UPROPERTY(meta = (BindWidget))
	USpinBox* repeatSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* movementSpeedSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* distanceSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* axisCB = nullptr;
#pragma endregion

public:
	void InitializeOnAxeWidget(const FMovementOnAxeSettings& _movementOnAxeSettings);
	FMovementOnAxeSettings GetOnAxeSettings() const;

private:
	virtual void NativeConstruct() override;
	void InitializeWidget();
};

//Class used to display Movement To Target Settings
UCLASS()
class INSIDEUS_API UEditor_PB_ToTargetWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Components
	UPROPERTY(meta = (BindWidget))
	USpinBox* repeatSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* movementSpeedSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* targetCB = nullptr;	
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* arenaPointNameCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* arenaPointNameHB = nullptr;
#pragma endregion

#pragma region Data
	TArray<FString> pointNames = { };
#pragma endregion

public:
	void InitializeToTargetWidgetUtil(const UA_DT_ArenaDataTable* _arenaDataTable);
	void InitializeToTargetWidget(const FMovementToTargetSettings& _movementToTargetSettings);
	FMovementToTargetSettings GetToTargetSettings() const;
private:
	virtual void NativeConstruct() override;
	void InitializeWidget();

	UFUNCTION() void UpdateArenaPointVisibility(FString _option, ESelectInfo::Type _selectInfo);
};

//Class used to display Movement Teleport Settings
UCLASS()
class INSIDEUS_API UEditor_PB_TeleportWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Components
	UPROPERTY(meta = (BindWidget))
	USpinBox* xOffsetSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* yOffsetSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* zOffsetSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* repeatSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* repeatRateSP = nullptr;
#pragma endregion

public:
	void InitializeTeleportWidget(const FMovementTeleportSettings& _movementTeleportSettings);
	FMovementTeleportSettings GetTeleportSettings() const;
};

//Class used to display Movement Settings widgets
UCLASS()
class INSIDEUS_API UEditor_PB_MovementWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_OnAxeWidget> onAxeWidgetType;
	UPROPERTY()
	UEditor_PB_OnAxeWidget* onAxeWidget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_ToTargetWidget> toTargetWidgetType;
	UPROPERTY()
	UEditor_PB_ToTargetWidget* toTargetWidget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_TeleportWidget> teleportWidgetType;
	UPROPERTY()
	UEditor_PB_TeleportWidget* teleportWidget = nullptr;

#pragma region Components
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* movementVB = nullptr;
#pragma endregion

#pragma region Data
	UPROPERTY()
	UA_DT_ArenaDataTable* arenaDataTable = nullptr;
#pragma endregion

public:
	void InitializeMovementWidget(const FPatternData& _patternData, UA_DT_ArenaDataTable* _arenaDataTable);
	void GetMovementOverride(FPatternData& _patternData);
	
private:
	virtual void NativeConstruct() override;
	void InitializeWidgets();

	void InitializeVisibility(const FPatternData& _patternData);
	void UpdateWidget(UWidget* _widget, const bool& _visible) const;
};

/*PROJECTILE*/

//Class used to display Projectile Guided Settings
UCLASS()
class INSIDEUS_API UEditor_ProjectileGuidedWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Components
	UPROPERTY(meta = (BindWidget))
	UCheckBox* editCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* settingsVB = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	USpinBox* guidedSpeedSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* rotationSpeedSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* rangeSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* waitTimeSP = nullptr;
#pragma endregion
	
public:
	void InitializeProjectileGuidedWidget(const FProjectileGuidedSettings& _settings);
	void SetEditGuided(const bool& _edit);
	
	FProjectileGuidedSettings GetProjectileGuidedSettings() const;
	bool GetEditGuided() const;

private:
	virtual void NativeConstruct() override;
	void InitializeWidget();

	UFUNCTION() void EditChanged(bool _editState);
};

//Class used to display Projectile Simulated Settings
UCLASS()
class INSIDEUS_API UEditor_ProjectileSimulatedWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Components
	UPROPERTY(meta = (BindWidget))
	UCheckBox* editCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* settingsVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* minScaleSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* maxScaleSP = nullptr;
#pragma endregion
	
public:
	void InitializeProjectileSimulatedWidget(const FProjectileSimulatedSettings& _settings);
	void SetEditSimulated(const bool& _edit);
	
	FProjectileSimulatedSettings GetProjectileSimulatedSettings() const;
	bool GetEditSimulated() const;

private:
	virtual void NativeConstruct() override;
	void InitializeWidget();

	UFUNCTION() void EditChanged(bool _editState);
};

//Class used to display Projectile Ballistic Settings
UCLASS()
class INSIDEUS_API UEditor_ProjectileBallisticWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Components
	UPROPERTY(meta = (BindWidget))
	UCheckBox* editCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* settingsVB = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	USpinBox* zOffsetSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* rangeSP = nullptr;
#pragma endregion
	
public:
	void InitializeProjectileBallisticWidget(const FProjectileBallisticSettings& _settings);
	void SetEditBallistic(const bool& _edit);
	
	FProjectileBallisticSettings GetProjectileBallisticSettings() const;
	bool GetEditBallistic() const;
	
private:
	virtual void NativeConstruct() override;
	void InitializeWidget();

	UFUNCTION() void EditChanged(bool _editState);
};

//Class used to display Projectile Sinusoidal Settings
UCLASS()
class INSIDEUS_API UEditor_ProjectileSinusoidalWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Components
	UPROPERTY(meta = (BindWidget))
	UCheckBox* editCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* settingsVB = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	USpinBox* periodRangeSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* amplitudeSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* startGapSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* xRotation = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* yRotation = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* zRotation = nullptr;
#pragma endregion

public:
	void InitializeProjectileSinusoidalWidget(const FProjectileSinusoidalSettings& _settings);
	void SetEditSinusoidal(const bool& _editState);
	
	FProjectileSinusoidalSettings GetProjectileSinusoidalSettings() const;
	bool GetEditSinusoidal() const;
	
private:
	virtual void NativeConstruct() override;
	void InitializeWidget();

	UFUNCTION() void EditChanged(bool _editState);
};

//Class used to display Projectile Settings
UCLASS()
class INSIDEUS_API UEditor_ProjectileWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_ProjectileGuidedWidget> projectileGuidedWidgetType;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_ProjectileSimulatedWidget> projectileSimulatedWidgetType;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_ProjectileBallisticWidget> projectileBallisticWidgetType;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_ProjectileSinusoidalWidget> projectileSinusoidalWidgetType;

	UPROPERTY()
	UEditor_ProjectileGuidedWidget* projectileGuidedWidget = nullptr;
	UPROPERTY()
	UEditor_ProjectileSimulatedWidget* projectileSimulatedWidget = nullptr;
	UPROPERTY()
	UEditor_ProjectileBallisticWidget* projectileBallisticWidget = nullptr;
	UPROPERTY()
	UEditor_ProjectileSinusoidalWidget* projectileSinusoidalWidget = nullptr;

#pragma region Components	
	UPROPERTY(meta = (BindWidget))
	UCheckBox* editCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* damageSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* speedSP = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* settingsVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* specificSettingsVB = nullptr;
#pragma endregion

public:
	void InitializeProjectileWidget(const FProjectileSettings& _settings, const TEnumAsByte<EProjectileType>& _projectileType);
	FProjectileSettings GetProjectileSettings() const;

private:
	virtual void NativeConstruct() override;
	void InitializeWidget();
	void InitializeWidgets();

	void UpdateWidget(UWidget* _widget, const bool& _state);

	UFUNCTION() void EditChanged(bool _editState);
};

/*SHOOT*/

//Class used to display Shoot Point Settings
UCLASS()
class INSIDEUS_API UEditor_PB_ShootPointWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Components
	UPROPERTY(meta = (BindWidget))
	USpinBox* radiusSP = nullptr;
#pragma endregion

public:
	void InitializeShootPointWidget(const FShootPointSettings& _shootSettings);
	FShootPointSettings GetShootPointSettings() const;
};

//Class used to display Shoot Line Settings 
UCLASS()
class INSIDEUS_API UEditor_PB_ShootLineWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Components
	UPROPERTY(meta = (BindWidget))
	USpinBox* lengthSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* planCB = nullptr;
	#pragma endregion

public:
	void InitializeShootLineWidget(const FShootLineSettings& _shootSettings);
	FShootLineSettings GetShootLineSettings() const;

private:
	virtual void NativeConstruct() override;
	void InitializeWidget();
};

//Class used to display Shoot Arc Settings 
UCLASS()
class INSIDEUS_API UEditor_PB_ShootArcWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Components
	UPROPERTY(meta = (BindWidget))
	UCheckBox* keepPatternForwardCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* radiusSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* angleSP = nullptr;
#pragma endregion

public:
	void InitializeShootArcWidget(const FShootArcSettings& _shootSettings);
	FShootArcSettings GetShootArcSettings() const;
};

//Class used to display Shoot Circle Settings 
UCLASS()
class INSIDEUS_API UEditor_PB_ShootCircleWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Components
	UPROPERTY(meta = (BindWidget))
	USpinBox* radiusSP = nullptr;
	#pragma endregion

public:
	void InitializeShootCircleWidget(const FShootCircleSettings& _shootSettings);
	FShootCircleSettings GetShootCircleSettings() const;
};

//Class used to display Shoot Cross Settings 
UCLASS()
class INSIDEUS_API UEditor_PB_ShootCrossWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Components
	UPROPERTY(meta = (BindWidget))
	UCheckBox* keepPatternForwardCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* radiusSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* xAngleSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* yAngleSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* xProjectileSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* yProjectileSP = nullptr;
#pragma endregion

public:
	void InitializeShootCrossWidget(const FShootCrossSettings& _shootSettings);
	FShootCrossSettings GetShootCrossSettings() const;
};

//Class used to display Shoot Random2D Settings 
UCLASS()
class INSIDEUS_API UEditor_PB_ShootRandom2DWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Components
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* planCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* angleSP = nullptr;
#pragma endregion

public:
	void InitializeShootRandom2DWidget(const FShootRandom2DSettings& _shootSettings);
	FShootRandom2DSettings GetShootRandom2DSettings() const;

private:
	virtual void NativeConstruct() override;
	void InitializeWidget();
};

//Class used to display Shoot Random3D Settings 
UCLASS()
class INSIDEUS_API UEditor_PB_ShootRandom3DWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Components
	UPROPERTY(meta = (BindWidget))
	USpinBox* angleSP = nullptr;
	#pragma endregion

public:
	void InitializeShootRandom3DWidget(const FShootRandom3DSettings& _shootSettings);
	FShootRandom3DSettings GetShootRandom3DSettings() const;
};

//Class used to display Shoot Settings widgets
UCLASS()
class INSIDEUS_API UEditor_PB_ShootWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_ShootPointWidget> pointWidgetType;
	UPROPERTY()
	UEditor_PB_ShootPointWidget* pointWidget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_ShootLineWidget> lineWidgetType;
	UPROPERTY()
	UEditor_PB_ShootLineWidget* lineWidget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_ShootArcWidget> arcWidgetType;
	UPROPERTY()
	UEditor_PB_ShootArcWidget* arcWidget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_ShootCircleWidget> circleWidgetType;
	UPROPERTY()
	UEditor_PB_ShootCircleWidget* circleWidget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_ShootCrossWidget> crossWidgetType;
	UPROPERTY()
	UEditor_PB_ShootCrossWidget* crossWidget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_ShootRandom2DWidget> random2DWidgetType;
	UPROPERTY()
	UEditor_PB_ShootRandom2DWidget* random2DWidget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_ShootRandom3DWidget> random3DWidgetType;
	UPROPERTY()
	UEditor_PB_ShootRandom3DWidget* random3DWidget = nullptr;

	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_ProjectileWidget> projectileWidgetType;
	UPROPERTY()
	UEditor_ProjectileWidget* projectileWidget = nullptr;

#pragma region Components
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* shootVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* projectileVB = nullptr;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* fireQtyHB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* fireDurationHB = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* shootingModeCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* spawnAtOnceSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* fireQtySP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* fireDurationSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* fireRateSP = nullptr;
#pragma endregion

public:
	void InitializeShootWidget(const FPatternData& _patternData);
	void GetShootOverride(FPatternData& _patternData) const;
	
private:
	virtual void NativeConstruct() override;
	void InitializeWidget();
	void InitializeWidgets();

	void InitializeVisibility(const FPatternData& _patternData);
	void UpdateWidget(UWidget* _widget, const bool& _visible) const;

	UFUNCTION() void ShootModeChanged(FString _option, ESelectInfo::Type _selectionType);
};

/*LASER*/

//Class used to display Laser Settings
UCLASS()
class INSIDEUS_API UEditor_PB_LaserWidget : public UUserWidget
{
	GENERATED_BODY()

#pragma region Components
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* pivotCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* damageSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* lengthSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* radiusSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* durationSP = nullptr;
#pragma endregion

public:
	void InitializeLaserWidget(const FPatternData& _patternData);
	void GetLaserOverride(FPatternData& _patternData);
	
private:
	virtual void NativeConstruct() override;
	void InitializeWidget();
};

/*WAVE*/

//Class used to display Laser Settings
UCLASS()
class INSIDEUS_API UEditor_PB_WaveWidget : public UUserWidget
{
	GENERATED_BODY()

#pragma region Components
	UPROPERTY(meta = (BindWidget))
	USpinBox* speedSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* durationSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* damageSP = nullptr;
#pragma endregion

public:
	void InitializeWaveWidget(const FPatternData& _patternData);
	void GetWaveOverride(FPatternData& _patternData);
};

/*ZONE*/

//Class used to display Scaling Settings
UCLASS()
class INSIDEUS_API UEditor_PB_ScalingWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Components
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* scalingVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* useTimeHB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* useGoalHB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* goalScaleHB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* scaleTimeHB = nullptr;

	UPROPERTY(meta = (BindWidget))
	USpinBox* timeBeforeActiveSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* useTimeCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* useGoalCB = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	USpinBox* scaleTimeSP = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	USpinBox* xgoalScaleSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* ygoalScaleSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* zgoalScaleSP = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	USpinBox* xDefaultScaleSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* yDefaultScaleSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* zDefaultScaleSP = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	USpinBox* scaleSpeedSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* persistentTimeSP = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UCheckBox* endPatternCB = nullptr;
#pragma endregion

public:
	void InitializeScalingWidget(const FScalingSettings& _scalingSettings);
	FScalingSettings GetScalingSettings() const;

private:
	virtual void NativeConstruct() override;
	void InitializeWidget();

	UFUNCTION() void UseTimeChanged(bool _state);
	UFUNCTION() void UseGoalChanged(bool _state);
};

//Class used to display Spawner Default Settings
UCLASS()
class INSIDEUS_API UEditor_PB_SpawnerDefaultWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Components
	UPROPERTY(meta = (BindWidget))
	USpinBox* spawnQtySP = nullptr;
#pragma endregion

public:
	void InitializeSpawnerDefaultWidget(const FSpawnerDefaultSettings& _spawnerSettings);
	FSpawnerDefaultSettings GetSpawnerDefaultSettings() const;
};

//Class used to display Concentric Circle Element
UCLASS()
class INSIDEUS_API UEditor_ConcentricCircleWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemoveElement, int, _elementIndex);

#pragma region Components
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* indexText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* removeButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* quantitySP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* spacingSP = nullptr;
#pragma endregion

#pragma region Data
	int index = 0;
#pragma endregion

	UPROPERTY()	
	FOnRemoveElement onRemoveElement;
	
public:
	FORCEINLINE FOnRemoveElement& OnRemoveElement() { return onRemoveElement; }

private:
	virtual void NativeConstruct() override;
	void InitializeWidget();
	
	UFUNCTION() void RemoveElement();
	
public:
	void InitializeConcentricCircleWidget(const int& _index, const FConcentricCircleSettings& _settings);
	FConcentricCircleSettings GetConcentricCircleSettings() const;

	void SetElementIndex(const int& _index);
};

//Class used to display Spawner Concentric Settings
UCLASS()
class INSIDEUS_API UEditor_PB_SpawnerConcentricWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_ConcentricCircleWidget> concentricCircleWidgetType;

#pragma region Components
protected:
	UPROPERTY(meta = (BindWidget))
	UExpandableArea* concentricCircleArea = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* concentricCircleListVB = nullptr;	
	UPROPERTY(meta = (BindWidget))
	UButton* addButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* clearButton = nullptr;
#pragma endregion

public:
	void InitializeSpawnerConcentricWidget(const FSpawnerConcentricSettings& _spawnerSettings);
	FSpawnerConcentricSettings GetSpawnerConcentricSettings() const;

private:
	virtual void NativeConstruct() override;
	void InitializeWidget();

	void InitConcentricCircleList(const TArray<FConcentricCircleSettings>& _circles);
	TArray<FConcentricCircleSettings> GetConcentricCircleList() const;
	
	UFUNCTION() void AddElementToConcentricCircleList();
	UFUNCTION() void RemoveElementFromList(int _index);
	UFUNCTION() void ClearConcentricCircleList();
	
	UFUNCTION() void UpdateExpandableArea(UExpandableArea* Area, bool bIsExpanded);
};

//Class used to display Concentric Circle Element
UCLASS()
class INSIDEUS_API UEditor_MultiElementWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemoveElement, int, _elementIndex);

#pragma region Components
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* indexText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* removeButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* quantitySP = nullptr;
#pragma endregion

#pragma region Data
	int index = 0;
#pragma endregion

	UPROPERTY()	
	FOnRemoveElement onRemoveElement;
	
public:
	FORCEINLINE FOnRemoveElement& OnRemoveElement() { return onRemoveElement; }

private:
	virtual void NativeConstruct() override;
	void InitializeWidget();
	
	UFUNCTION() void RemoveElement();
	
public:
	void InitializeMultiElementWidget(const int& _index, const int& _quantity = 0);
	int GetMultiElementValue() const;

	void SetElementIndex(const int& _index);
};

//Class used to display Spawner Multi Settings
UCLASS()
class INSIDEUS_API UEditor_PB_SpawnerMultiWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_MultiElementWidget> multiElementWidgetType;

#pragma region Components
	UPROPERTY(meta = (BindWidget))
	USpinBox* rangeSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* minDistanceSP = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* multiElementListVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* addButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* clearButton = nullptr;
#pragma endregion

public:
	void InitializeSpawnerMultiWidget(const FSpawnerMultiSettings& _spawnerSettings);
	FSpawnerMultiSettings GetSpawnerMultiSettings() const;

private:
	virtual void NativeConstruct() override;
	void InitializeWidget();

	void InitMultiElementList(const TArray<unsigned int>& _elements);
	TArray<unsigned int> GetMultiElementList() const;

	UFUNCTION() void AddElementToMultiElementList();
	UFUNCTION() void RemoveElementFromList(int _index);
	UFUNCTION() void ClearMultiElementList();
};

//Class used to display Spawner Settings
UCLASS()
class INSIDEUS_API UEditor_PB_SpawnerWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Components
	UPROPERTY(meta = (BindWidget))
	UCheckBox* WaitCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* xOffsetSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* yOffsetSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* zOffsetSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* spawnRateSP = nullptr;
#pragma endregion

public:
	void InitializeSpawnerWidget(const FSpawnerSettings& _spawnerSettings);
	FSpawnerSettings GetSpawnerSettings() const;
};

//Class used to display GPE Projectile Settings
UCLASS()
class INSIDEUS_API UEditor_GPE_ProjectileWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Components
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* projectileVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* editCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* settingsVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* damageSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* moveUpCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* zOffsetSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* moveSpeedSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* previewTimeSP = nullptr;
#pragma endregion

public:
	void InitializeGPEProjectileWidget(const FGPE_ProjectileSettings& _gpeSettings, const bool& _editState);
	FGPE_ProjectileSettings GetGPEProjectileSettings() const;
	bool GetEditGPEProjectileState() const;

private:
	virtual void NativeConstruct() override;
	void InitializeWidget();

	UFUNCTION() void EditChanged(bool _editState);
};

//Class used to display GPE Floor Settings
UCLASS()
class INSIDEUS_API UEditor_GPE_FloorWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Components
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* floorVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* editCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* settingsVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* damageSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* previewTimeSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* persistentTimeSP = nullptr;
#pragma endregion

public:
	void InitializeGPEFloorWidget(const FGPE_FloorDangerSettings& _gpeSettings, const bool _editState);
	FGPE_FloorDangerSettings GetGPEFloorSettings() const;
	bool GetEditGPEFloorState() const;

private:
	virtual void NativeConstruct() override;
	void InitializeWidget();

	UFUNCTION() void EditChanged(bool _editState);
};

//Class used to display GPE Hunt Rift Settings
UCLASS()
class INSIDEUS_API UEditor_GPE_HuntRiftWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Components
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* huntRiftVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* editCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* settingsVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* damageSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* moveSpeedSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* followDurationSP = nullptr;
#pragma endregion

public:
	void InitializeGPEHuntRiftWidget(const FGPE_HuntRiftSettings& _gpeSettings, const bool& _editState);
	FGPE_HuntRiftSettings GetGPEHuntRiftSettings() const;
	bool GetEditGPEHuntRiftState() const;

private:
	virtual void NativeConstruct() override;
	void InitializeWidget();

	UFUNCTION() void EditChanged(bool _editState);
};

//Class used to display Zone Settings
UCLASS()
class INSIDEUS_API UEditor_PB_ZoneWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_SpawnerWidget> spawnerWidgetType;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_SpawnerDefaultWidget> spawnerDefaultWidgetType;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_SpawnerConcentricWidget> spawnerConcentricWidgetType;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_SpawnerMultiWidget> spawnerMultiWidgetType;

	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_GPE_ProjectileWidget> gpeProjectileWidgetType;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_GPE_FloorWidget> gpeFloorWidgetType;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_GPE_HuntRiftWidget> gpeHuntRiftWidgetType;

	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PB_ScalingWidget> scalingWidgetType;
	
	UPROPERTY()
	UEditor_PB_SpawnerWidget* spawnerWidget = nullptr;
	UPROPERTY()
	UEditor_PB_SpawnerDefaultWidget* spawnerDefaultWidget = nullptr;
	UPROPERTY()
	UEditor_PB_SpawnerConcentricWidget* spawnerConcentricWidget = nullptr;
	UPROPERTY()
	UEditor_PB_SpawnerMultiWidget* spawnerMultiWidget = nullptr;
	
	UPROPERTY()
	UEditor_GPE_ProjectileWidget* gpeProjectileWidget = nullptr;
	UPROPERTY()
	UEditor_GPE_FloorWidget* gpeFloorWidget = nullptr;
	UPROPERTY()
	UEditor_GPE_HuntRiftWidget* gpeHuntRiftWidget = nullptr;
	
	UPROPERTY()
	UEditor_PB_ScalingWidget* scalingWidget = nullptr;

#pragma region Components
protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* zoneVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* spawnerVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* gpeVB = nullptr;
#pragma endregion

private:
	virtual void NativeConstruct() override;
	void InitializeWidgets();

	void InitializeVisibility(const FPatternData& _patternData);
	
	void UpdateSpawnerWidget(UWidget* _widget, const bool& _visible) const;
	void UpdateGPEWidget(UWidget* _widget, const bool& _visible) const;
	void UpdateZoneWidget(UWidget* _widget, const bool& _visible) const;
	
public:
	void InitializeZoneWidget(const FPatternData& _patternData);
	void GetZoneOverride(FPatternData& _patternData) const;
};

/*ARENA CLOSER*/

//Class used to display Arena Closer Settings
UCLASS()
class INSIDEUS_API UEditor_PB_ArenaCloserWidget : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UEditor_PointNameElementWidget> pointNameElementWidgetType;

#pragma region Components
	UPROPERTY(meta = (BindWidget))
	UCheckBox* closePlayerSliceCB = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* damageSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* timeBeforeCloseSP = nullptr;
	UPROPERTY(meta = (BindWidget))
	USpinBox* closeTimeSP = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* arenaSliceListVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* addButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* clearButton = nullptr;
#pragma endregion

#pragma region Data
	UPROPERTY()
	UA_DT_ArenaDataTable* arenaDataTable = nullptr;
#pragma endregion

private:
	virtual void NativeConstruct() override;
	void InitializeWidget();

	void InitArenaSliceNames(const TArray<FString>& _names);
	TArray<FString> GetArenaSliceNames() const;
	
	UFUNCTION() void AddArenaSliceElementToList();
	UFUNCTION() void RemoveArenaSliceElementFromList(int _index);
	UFUNCTION() void ClearArenaSliceList();
	
public:
	void InitializeArenaCloserWidget(const FPatternData& _patternData, UA_DT_ArenaDataTable* _arenaDataTable);
	void GetArenaCloserOverride(FPatternData& _patternData) const;
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


/*Si il faut afficher certains TSubclassOf

#include "Components/SinglePropertyView.h"

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AP_Projectile> projectileType;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	USinglePropertyView* propertyViewTest = nullptr;

+ dans le Pre Construct -> propertyView -> set object (self) -> Set property name (nom de la property) 
*/