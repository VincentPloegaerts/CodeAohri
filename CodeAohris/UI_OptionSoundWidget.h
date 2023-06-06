#pragma once

#include "CoreMinimal.h"
#include "UI_UserWidget.h"

#include "Components/CheckBox.h"
#include "Components/Slider.h"

#include "UI_OptionSoundWidget.generated.h"

class US_PlayerSave;

UCLASS()
class INSIDEUS_API UUI_OptionSoundWidget : public UUI_UserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USlider* masterSlider = nullptr;
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	USlider* musicSlider = nullptr;
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	USlider* ambientSlider = nullptr;
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	USlider* sfxSlider = nullptr;
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UCheckBox* muteCB = nullptr;

	UPROPERTY()
	US_PlayerSave* playerSave = nullptr;
	
private:
	virtual void InitializeWidget() override;

public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

protected:
	UFUNCTION() void ChangeMasterValue(float _value);
	UFUNCTION() void ChangeMusicValue(float _value);
	UFUNCTION() void ChangeAmbientValue(float _value);
	UFUNCTION() void ChangeSFXValue(float _value);
	
	UFUNCTION() void SwitchMute(bool _value);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Sound") void OnMasterVolumeUpdate(float MasterVolume);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sound") void OnMusicVolumeUpdate(float MusicVolume);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sound") void OnAmbientVolumeUpdate(float AmbientVolume);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sound") void OnSFXVolumeUpdate(float SFXVolume);

	UFUNCTION(BlueprintImplementableEvent, Category = "Sound") void OnMuteUpdate(bool Mute);

public:
	virtual void InitializeFocus() override;
};
