#include "UI_GameInputWidget.h"

void UUI_GameInputWidget::RebindInputFailed(const FName& _inputName, const FKey& _failedKey, const FString& _reason)
{
	onRebindInputFailed.Broadcast(_inputName, _failedKey, _reason);
}
