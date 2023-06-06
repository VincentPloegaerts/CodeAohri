#include "M_PostProcessVolume.h"

#include "HUD_Main.h"
#include "M_GameInstance.h"
#include "S_PlayerSave.h"
#include "UI_OptionCalibrationWidget.h"

AM_PostProcessVolume::AM_PostProcessVolume()
{
	Settings.bOverride_AutoExposureMethod = true;
	Settings.AutoExposureMethod = EAutoExposureMethod::AEM_Manual;
	Settings.bOverride_AutoExposureBias = true;
	Settings.AutoExposureBias = 10;
	bUnbound = true;
}

void AM_PostProcessVolume::BeginPlay()
{
	Super::BeginPlay();

	InitializePostProcess();
}

void AM_PostProcessVolume::InitializePostProcess()
{
	if (const UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
	{
		playerSave = _gi->GetPlayerSave();
	}

	if (playerSave)
	{
		if (AHUD_Main* _hud = GetWorld()->GetFirstPlayerController()->GetHUD<AHUD_Main>())
		{
			if (UUI_OptionCalibrationWidget* _widget = _hud->GetWidget<UUI_OptionCalibrationWidget>(WIDGET_CALIBRATION))
			{
				Settings.AutoExposureBias = playerSave->GetGraphicConfig().GetExposureValue();
				_widget->OnBrightnessUpdated().AddUniqueDynamic(this, &AM_PostProcessVolume::UpdateExposure);
			}
		}
	}
}

void AM_PostProcessVolume::UpdateExposure(float _value)
{
	if (playerSave)
	{
		Settings.AutoExposureBias = _value;
		playerSave->EditGraphicConfig().SetExposureValue(_value);
	}
}
