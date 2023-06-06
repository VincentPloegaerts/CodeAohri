#include "UI_OptionGraphicWidget.h"

#include "HUD_Main.h"
#include "M_GameInstance.h"
#include "S_PlayerSave.h"

#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

#include "IU_Macros.h"

void UUI_OptionGraphicWidget::InitializeFocus()
{
	fullscreenCB->SetKeyboardFocus();
}

void UUI_OptionGraphicWidget::SetVisibility(ESlateVisibility InVisibility)
{
	if (GetVisibility() != ESlateVisibility::Hidden)
		if (InVisibility == ESlateVisibility::Hidden)
			if (playerSave)
				playerSave->PlayerSaveEdited();
	
	Super::SetVisibility(InVisibility);
}

#pragma region Init
void UUI_OptionGraphicWidget::InitializeWidget()
{	
	if (const UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
	{
		userSettings = _gi->GetGameUserSettings();
		playerSave = _gi->GetPlayerSave();
	}

	if (!userSettings || !playerSave) return;
	
	hud = GetWorld()->GetFirstPlayerController()->GetHUD<AHUD_Main>();
	
	InitializeResolutions();
	InitializeFullscreenMode();
	InitializeScalabilitySettings();
	InitializePlayerPreference(playerSave->GetPlayerConfig());
	InitializeMotionBlur(playerSave->GetGraphicConfig());

	exposureCalibrationButton->OnClicked.AddUniqueDynamic(this, &UUI_OptionGraphicWidget::ExposureCalibration);
	
	scalabilityHideButton->OnClicked.AddUniqueDynamic(this, &UUI_OptionGraphicWidget::SwitchScalabilityVisibility);
	scalabilitySB->SetVisibility(scalabilityVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	scalabilityHideText->SetText(FText::FromString(scalabilityVisible ? textVisible : textHidden));
}

void UUI_OptionGraphicWidget::InitializeResolutions()
{
	resolutionCB->OnSelectionChanged.AddUniqueDynamic(this, &UUI_OptionGraphicWidget::ChangeResolution);
	resolutionScaleSlider->OnValueChanged.AddUniqueDynamic(this, &UUI_OptionGraphicWidget::ResolutionScaleSliderChanged);
	
	resolutionScaleSlider->SetValue(userSettings->GetResolutionScaleNormalized());
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(resolutions);
	const int& _max = resolutions.Num();
	resolutionCB->ClearOptions();
	for (int i = 0; i < _max; i++)
	{
		const FIntPoint& _resolution = resolutions[i];
		resolutionCB->AddOption(GetResolutionString(_resolution));
	}

	FString _resolution = GetResolutionString(userSettings->GetScreenResolution());
	const int32 _resIndex = resolutionCB->FindOptionIndex(_resolution);

	if (_resIndex == -1)
		_resolution = GetResolutionString(userSettings->GetDesktopResolution());
	
	resolutionCB->SetSelectedOption(_resolution);
}
void UUI_OptionGraphicWidget::InitializeFullscreenMode()
{
	GetWorld()->GetGameViewport()->OnToggleFullscreen().AddUObject(this, &UUI_OptionGraphicWidget::OnFullscreenToggled);
	fullscreenCB->OnSelectionChanged.AddUniqueDynamic(this, &UUI_OptionGraphicWidget::ChangeFullscreenMode);
	
	fullscreenCB->ClearOptions();
	fullscreenCB->AddOption("FULLSCREEN");
	fullscreenCB->AddOption("BORDERLESS");
	fullscreenCB->AddOption("WINDOWED");
	
	const EWindowMode::Type& _mode = userSettings->GetFullscreenMode();
	fullscreenCB->SetSelectedIndex(_mode);
}
void UUI_OptionGraphicWidget::InitializeScalabilitySettings()
{
	benchmarkButton->OnClicked.AddUniqueDynamic(this, &UUI_OptionGraphicWidget::RunBenchmark);
	
	if (globalScalabilityWidgetType.Get())
	{
		const EScalabilityType& _globalType = EScalabilityType::Global_Quality;
		if (CreateScalabilityWidget(globalScalabilityWidgetType, _globalType))
			scalability[_globalType]->SetSpacerSize(FVector2D(0, fGlobalSpace));
	}
	
	for (const auto& _current : scalability)
	{
		if (_current.Value) continue;
		
		CreateScalabilityWidget(otherScalabilityWidgetType, _current.Key);
	}
}
void UUI_OptionGraphicWidget::InitializePlayerPreference(const FPlayerConfig& _config)
{
	hudSlider->OnValueChanged.AddUniqueDynamic(this, &UUI_OptionGraphicWidget::HUDSliderChanged);
	crosshairSlider->OnValueChanged.AddUniqueDynamic(this, &UUI_OptionGraphicWidget::CrosshairSliderChanged);
	
	crosshairSlider->SetValue(_config.GetSizeCrosshair());
	hudSlider->SetValue(_config.GetSizeHUD());
}
void UUI_OptionGraphicWidget::InitializeMotionBlur(const FGraphicsConfig& _graphicConfig)
{
	motionBlurCB->OnCheckStateChanged.AddUniqueDynamic(this, &UUI_OptionGraphicWidget::SetMotionBlurEnable);
	motionBlurSlider->OnValueChanged.AddUniqueDynamic(this, &UUI_OptionGraphicWidget::ChangeMotionBlurValue);
	
	const bool& _motionBlurEnable = _graphicConfig.GetMotionBlurEnable();
	motionBlurCB->SetIsChecked(_motionBlurEnable);
	motionBlurSlider->SetValue(_graphicConfig.GetMotionBlurValue());
	
	if (_motionBlurEnable)
		motionBlurValueHB->SetVisibility(ESlateVisibility::Visible);
	else
		motionBlurValueHB->SetVisibility(ESlateVisibility::Collapsed);
}

bool UUI_OptionGraphicWidget::CreateScalabilityWidget(const TSubclassOf<UUI_ScalabilityWidget>& _widgetType, const EScalabilityType& _scalabilityType)
{
	if (_widgetType.Get())
	{
		if (UUI_ScalabilityWidget* _widget = CreateWidget<UUI_ScalabilityWidget>(GetWorld(), otherScalabilityWidgetType))
		{
			_widget->InitializeScalabilityWidget(_scalabilityType, GetScalability(_scalabilityType));
			_widget->OnScalabilitySelect().AddUniqueDynamic(this, &UUI_OptionGraphicWidget::SetScalability);
			scalabilitySB->AddChild(_widget);
			scalability[_scalabilityType] = _widget;

			return true;
		}
	}
	return false;
}

#pragma endregion

void UUI_OptionGraphicWidget::ExposureCalibration()
{
	if (hud)
		hud->ShowMenuWidget(WIDGET_CALIBRATION);
}

#pragma region Scalability
void UUI_OptionGraphicWidget::SwitchScalabilityVisibility()
{
	scalabilityVisible = !scalabilityVisible;
	scalabilitySB->SetVisibility(scalabilityVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	scalabilityHideText->SetText(FText::FromString(scalabilityVisible ? textVisible : textHidden));
}

void UUI_OptionGraphicWidget::RunBenchmark()
{
	userSettings->RunHardwareBenchmark();
	userSettings->ApplyHardwareBenchmarkResults();

	for (auto _current : scalability)
	{
		const EScalabilityType& _scalabilityType = _current.Key;
		UUI_ScalabilityWidget* _widget = _current.Value;
		if (!_widget) continue;
		_widget->InitializeScalabilityWidget(_scalabilityType, GetScalability(_scalabilityType));
	}
}

void UUI_OptionGraphicWidget::SetScalability(const EScalabilityType& _type, int32 _value)
{
	if (!scalability.Contains(_type)) return;

	if (_type == Global_Quality)
	{
		userSettings->SetOverallScalabilityLevel(_value);
		for (auto _current : scalability)
		{
			const EScalabilityType& _scalabilityType = _current.Key;
			UUI_ScalabilityWidget* _widget = _current.Value;
			if (!_widget) continue;
			_widget->InitializeScalabilityWidget(_scalabilityType, _value);
		}
	}
	if (_type == View_Distance_Quality)
		userSettings->SetViewDistanceQuality(_value);
	else if (_type == Anti_Aliasing_Quality)
		userSettings->SetAntiAliasingQuality(_value);
	else if (_type == Shadow_Quality)
		userSettings->SetShadowQuality(_value);
	else if (_type == Illumination_Quality)
		userSettings->SetGlobalIlluminationQuality(_value);
	else if (_type == Reflection_Quality)
		userSettings->SetReflectionQuality(_value);
	else if (_type == Post_Process_Quality)
		userSettings->SetPostProcessingQuality(_value);
	else if (_type == Texture_Quality)
		userSettings->SetTextureQuality(_value);
	else if (_type == Effects_Quality)
		userSettings->SetVisualEffectQuality(_value);
	else if (_type == Foliage_Quality)
		userSettings->SetFoliageQuality(_value);
	else if (_type == Shading_Quality)
		userSettings->SetShadingQuality(_value);

	if (_type != Global_Quality)
	{
		scalability[Global_Quality]->InitializeScalabilityWidget(Global_Quality, -1);
	}
	
	userSettings->ApplySettings(false);
}
int32 UUI_OptionGraphicWidget::GetScalability(const EScalabilityType& _type) const
{
	if (_type == Global_Quality)
		return userSettings->GetOverallScalabilityLevel();
	if (_type == View_Distance_Quality)
		return userSettings->GetViewDistanceQuality();
	if (_type == Anti_Aliasing_Quality)
		return userSettings->GetAntiAliasingQuality();
	if (_type == Shadow_Quality)
		return userSettings->GetShadowQuality();
	if (_type == Illumination_Quality)
		return userSettings->GetGlobalIlluminationQuality();
	if (_type == Reflection_Quality)
		return userSettings->GetReflectionQuality();
	if (_type == Post_Process_Quality)
		return userSettings->GetPostProcessingQuality();
	if (_type == Texture_Quality)
		return userSettings->GetTextureQuality();
	if (_type == Effects_Quality)
		return userSettings->GetVisualEffectQuality();
	if (_type == Foliage_Quality)
		return userSettings->GetFoliageQuality();
	if (_type == Shading_Quality)
		return userSettings->GetShadingQuality();
	
	return -1;
}
#pragma endregion

#pragma region Resolution
void UUI_OptionGraphicWidget::ChangeResolution(FString _selection, ESelectInfo::Type _selectType)
{
	if (!resolutions.IsValidIndex(resolutionCB->GetSelectedIndex())) return;
	userSettings->SetScreenResolution(resolutions[resolutionCB->GetSelectedIndex()]);
	userSettings->ApplySettings(false);
	
	if (userSettings->GetFullscreenMode() == EWindowMode::Windowed)
		UpdateWindowedMode();
}
void UUI_OptionGraphicWidget::ResolutionScaleSliderChanged(float _value)
{
	userSettings->SetResolutionScaleNormalized(_value);
	userSettings->ApplySettings(false);
}
#pragma endregion

#pragma region Fullscreen Mode
void UUI_OptionGraphicWidget::ChangeFullscreenMode(FString _selection, ESelectInfo::Type _selectType)
{
	SetGameFullscreenMode(EWindowMode::ConvertIntToWindowMode(fullscreenCB->GetSelectedIndex()));
	ChangeResolution(resolutionCB->GetSelectedOption(), ESelectInfo::Direct);
	userSettings->ApplySettings(false);

	if (userSettings->GetFullscreenMode() == EWindowMode::Windowed)
		UpdateWindowedMode();
}

void UUI_OptionGraphicWidget::SetGameFullscreenMode(const EWindowMode::Type& _mode)
{
	if (_mode == EWindowMode::Fullscreen)
	{
		FDisplayMetrics _display;
		FDisplayMetrics::RebuildDisplayMetrics(_display);

		#if !WITH_EDITOR
		GEngine->GameViewport->GetWindow()->MoveWindowTo(FVector2D(_display.PrimaryDisplayWorkAreaRect.Left, 0));
		#endif
	}
	
	const bool& _displayRes = _mode != EWindowMode::WindowedFullscreen;
	resolutionHB->SetVisibility(_displayRes ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	
	userSettings->SetFullscreenMode(_mode);
}
void UUI_OptionGraphicWidget::UpdateWindowedMode()
{
	#if !WITH_EDITOR
	SWindow* _window = GEngine->GameViewport->GetWindow().Get();
	const FVector2D& _position = _window->GetPositionInScreen();
	const float& _titleBarY = _position.Y - 32;
	const float& _offsetY = _titleBarY > 0 ? 0 : _titleBarY; 
	_window->MoveWindowTo(_position - FVector2D(0, _offsetY));
	#endif
}

void UUI_OptionGraphicWidget::OnFullscreenToggled(bool _toggle)
{
	const EWindowMode::Type& _mode = userSettings->GetFullscreenMode();
	fullscreenCB->SetSelectedIndex(_mode);
}
#pragma endregion

#pragma region Player Preference
void UUI_OptionGraphicWidget::HUDSliderChanged(float _value)
{
	if (playerSave)
		playerSave->EditPlayerConfig().SetSizeHUD(_value);
}
void UUI_OptionGraphicWidget::CrosshairSliderChanged(float _value)
{
	if (playerSave)
		playerSave->EditPlayerConfig().SetSizeCrosshair(_value);
}
#pragma endregion

#pragma region Motion Blur
void UUI_OptionGraphicWidget::SetMotionBlurEnable(bool _motionBlurEnable)
{
	if (_motionBlurEnable)
		motionBlurValueHB->SetVisibility(ESlateVisibility::Visible);
	else
		motionBlurValueHB->SetVisibility(ESlateVisibility::Collapsed);
	
	playerSave->EditGraphicConfig().SetMotionBlurEnable(_motionBlurEnable);
}
void UUI_OptionGraphicWidget::ChangeMotionBlurValue(float _motionBlur)
{
	playerSave->EditGraphicConfig().SetMotionBlurValue(_motionBlur);
}
#pragma endregion
