#include "UI_OptionSoundWidget.h"

#include "M_GameInstance.h"
#include "S_PlayerSave.h"

void UUI_OptionSoundWidget::InitializeWidget()
{
	if (const UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
	{
		playerSave = _gi->GetPlayerSave();
	}

	if (playerSave)
	{
		masterSlider->OnValueChanged.AddUniqueDynamic(this, &UUI_OptionSoundWidget::ChangeMasterValue);
		musicSlider->OnValueChanged.AddUniqueDynamic(this, &UUI_OptionSoundWidget::ChangeMusicValue);
		ambientSlider->OnValueChanged.AddUniqueDynamic(this, &UUI_OptionSoundWidget::ChangeAmbientValue);
		sfxSlider->OnValueChanged.AddUniqueDynamic(this, &UUI_OptionSoundWidget::ChangeSFXValue);
		muteCB->OnCheckStateChanged.AddUniqueDynamic(this, &UUI_OptionSoundWidget::SwitchMute);

		const FSoundConfig& _config = playerSave->GetSoundConfig();
		
		masterSlider->SetValue(_config.GetMasterValue());
		OnMasterVolumeUpdate(_config.GetMasterValue());
		
		musicSlider->SetValue(_config.GetMusicValue());
		OnMusicVolumeUpdate(_config.GetMusicValue());
		
		ambientSlider->SetValue(_config.GetAmbientValue());
		OnAmbientVolumeUpdate(_config.GetAmbientValue());
		
		sfxSlider->SetValue(_config.GetSFXValue());
		OnSFXVolumeUpdate(_config.GetSFXValue());
		
		muteCB->SetIsChecked(_config.GetMute());
		OnMuteUpdate(_config.GetMute());
	}
}

void UUI_OptionSoundWidget::SetVisibility(ESlateVisibility InVisibility)
{
	if (GetVisibility() != ESlateVisibility::Hidden)
		if (InVisibility == ESlateVisibility::Hidden)
			if (playerSave)
				playerSave->PlayerSaveEdited();
	
	Super::SetVisibility(InVisibility);
}

void UUI_OptionSoundWidget::ChangeMasterValue(float _value)
{
	if (!playerSave) return;
	playerSave->EditSoundConfig().SetMasterValue(_value);
	OnMasterVolumeUpdate(_value);
}

void UUI_OptionSoundWidget::ChangeMusicValue(float _value)
{
	if (!playerSave) return;
	playerSave->EditSoundConfig().SetMusicValue(_value);	
	OnMusicVolumeUpdate(_value);
}

void UUI_OptionSoundWidget::ChangeAmbientValue(float _value)
{
	if (!playerSave) return;
	playerSave->EditSoundConfig().SetAmbientValue(_value);	
	OnAmbientVolumeUpdate(_value);
}

void UUI_OptionSoundWidget::ChangeSFXValue(float _value)
{
	if (!playerSave) return;
	playerSave->EditSoundConfig().SetSFXValue(_value);	
	OnSFXVolumeUpdate(_value);
}

void UUI_OptionSoundWidget::SwitchMute(bool _value)
{
	if (!playerSave) return;
	playerSave->EditSoundConfig().SetMute(_value);
	OnMuteUpdate(_value);
}

void UUI_OptionSoundWidget::InitializeFocus()
{
	masterSlider->SetKeyboardFocus();
}
