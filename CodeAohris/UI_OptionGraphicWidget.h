#pragma once

#include "CoreMinimal.h"
#include "UI_UserWidget.h"

#include "UI_ScalabilityWidget.h"

#include "STRUCT_GraphicsConfig.h"
#include "STRUCT_PlayerConfig.h"

#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/HorizontalBox.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"

#include "GameFramework/GameUserSettings.h"

#include "UI_OptionGraphicWidget.generated.h"

class AHUD_Main;

UCLASS()
class INSIDEUS_API UUI_OptionGraphicWidget : public UUI_UserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* benchmarkButton = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* exposureCalibrationButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UScrollBox* scalabilitySB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* scalabilityVB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* fullscreenHB = nullptr;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* resolutionHB = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* fullscreenCB = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* resolutionCB = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USlider* resolutionScaleSlider = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USlider* hudSlider = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USlider* crosshairSlider = nullptr;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCheckBox* motionBlurCB = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* motionBlurValueHB = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USlider* motionBlurSlider = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* scalabilityHideButton = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* scalabilityHideText = nullptr;
	
private:
	UPROPERTY(EditAnywhere, Category = "Settings")
	FString textHidden = "Show Scalability";
	UPROPERTY(EditAnywhere, Category = "Settings")
	FString textVisible = "Hide Scalability";
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	float fGlobalSpace = 10.0f;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UUI_ScalabilityWidget> globalScalabilityWidgetType;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UUI_ScalabilityWidget> otherScalabilityWidgetType;
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	TMap<TEnumAsByte<EScalabilityType>, UUI_ScalabilityWidget*> scalability =
	{
		{ Global_Quality, nullptr },
		{ View_Distance_Quality, nullptr },
		{ Anti_Aliasing_Quality, nullptr },
		{ Shadow_Quality, nullptr },
		{ Illumination_Quality, nullptr },
		{ Reflection_Quality, nullptr },
		{ Post_Process_Quality, nullptr },
		{ Texture_Quality, nullptr },
		{ Effects_Quality, nullptr },
		{ Foliage_Quality, nullptr },
		{ Shading_Quality, nullptr },
	};

	UPROPERTY()
	bool scalabilityVisible = false;
	
	UPROPERTY()
	AHUD_Main* hud = nullptr;
	UPROPERTY()
	TArray<FIntPoint> resolutions;
	UPROPERTY()
	UGameUserSettings* userSettings = nullptr;
	UPROPERTY()
	class US_PlayerSave* playerSave = nullptr;

public:
	FORCEINLINE FString GetResolutionString(const FIntPoint& _resolution) const { return FString::FromInt(_resolution.X) + FString("x") + FString::FromInt(_resolution.Y); } 

public:
	void InitializeFocus();

	virtual void SetVisibility(ESlateVisibility InVisibility) override;
	
private:
	#pragma region Init
	virtual void InitializeWidget() override;
	
	void InitializeResolutions();
	void InitializeFullscreenMode();
	void InitializeScalabilitySettings();
	void InitializePlayerPreference(const FPlayerConfig& _config);
	void InitializeMotionBlur(const FGraphicsConfig& _playerAccessibility);

	bool CreateScalabilityWidget(const TSubclassOf<UUI_ScalabilityWidget>& _widgetType, const EScalabilityType& _scalabilityType);
	#pragma endregion

	UFUNCTION() void ExposureCalibration();

	#pragma region Scalability
	UFUNCTION() void SwitchScalabilityVisibility();
	
	UFUNCTION() void RunBenchmark();

	//Set Scalability Value in UserSettings depending on ScalabilityType
	UFUNCTION() void SetScalability(const EScalabilityType& _type, int32 _value);
	//Get Scalability Value from UserSettings assign to the ScalabilityType 
	int32 GetScalability(const EScalabilityType& _type) const;
	#pragma endregion

	#pragma region Resolution
	//Change Game resolution on player ComboBox choice
	UFUNCTION() void ChangeResolution(FString _selection, ESelectInfo::Type _selectType);
	UFUNCTION() void ResolutionScaleSliderChanged(float _value);
	#pragma endregion

	#pragma region Fullscreen Mode
	//Set Fullscreen Mode & Update Resolution then apply UserSettings
	UFUNCTION() void ChangeFullscreenMode(FString _selection, ESelectInfo::Type _selectType);

	//Update Window Position to first screen if required (Fullscreen), then set FullscreenMode in UserSettings
	void SetGameFullscreenMode(const EWindowMode::Type& _mode);
	//Update Game Window position on screen (avoid Window bar to be to high on screen)
	void UpdateWindowedMode();
	
	UFUNCTION() void OnFullscreenToggled(bool _toggle);
	#pragma endregion

	#pragma region Player Preference
	UFUNCTION() void HUDSliderChanged(float _value);
	UFUNCTION() void CrosshairSliderChanged(float _value);
	#pragma endregion

	#pragma region Motion Blur
	UFUNCTION() void SetMotionBlurEnable(bool _motionBlurEnable);
	UFUNCTION() void ChangeMotionBlurValue(float _motionBlur);
	#pragma endregion
};
