#include "UI_ResolutionWidget.h"
#include "GameFramework/GameUserSettings.h"

void UUI_ResolutionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	resolutionButton->OnClicked.AddDynamic(this, &UUI_ResolutionWidget::ExecuteResolutionCallback);
}

FReply UUI_ResolutionWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if(InKeyEvent.GetKey() == confirmKey)
		ExecuteResolutionCallback();
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UUI_ResolutionWidget::ExecuteResolutionCallback()
{
	UGameUserSettings* _settings = UGameUserSettings::GetGameUserSettings();
	if (!_settings) return;

	FString _xString, _yString;
	resolution.Split("x", &_xString, &_yString);
	_settings->SetScreenResolution(FIntPoint(FCString::Atoi(*_xString), FCString::Atoi(*_yString) - 56));
	_settings->ApplySettings(false);
}

void UUI_ResolutionWidget::Init(const FString& _resolution, const FString& _ratio)
{
	resolution = _resolution;
	resolutionText->SetText(FText::FromString(_resolution));
	resolutionRatioText->SetText(FText::FromString(_ratio));
}

void UUI_ResolutionWidget::SetActiveStatus(const bool _status) const
{
	resolutionButton->SetStyle(_status ? hoverButtonStyle : normalButtonStyle);
}