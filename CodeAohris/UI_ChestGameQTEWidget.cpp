
#include "UI_ChestGameQTEWidget.h"
#if WITH_EDITOR
#include "IU_Macros.h"
#endif
#include "UI_GameInputInfoWidget.h"

void UUI_ChestGameQTEWidget::FillHorizontalBox(const TArray<FKey>& _keys)
{
	EmptyBox();

	if (keyWidgetType.Get())
	{
		const int& _max = _keys.Num();
		for (int i =0;i<_max;i++)
		{
			if (UUI_ChestQTEKeyWidget* _keyWidget = CreateWidget<UUI_ChestQTEKeyWidget>(this, keyWidgetType))
			{
				keyWidgets.Add(_keyWidget);
				_keyWidget->InitializeInputInfoWidget(_keys[i]);
				_keyWidget->ToInitKey();
				horizontalBox->AddChild(_keyWidget);	
			}
		}
	}
	else
	{
	#if WITH_EDITOR
		LOG("UUI_ChestGameQTEWidget Failed to create widget")
	#endif
	}
}

void UUI_ChestGameQTEWidget::EmptyBox()
{
	keyWidgets.Empty();
	horizontalBox->ClearChildren();
}

void UUI_ChestGameQTEWidget::UpdateCurrentQTEKey(const int& _index)
{
	if (_index >= keyWidgets.Num()) return;

	if (_index - 1 >= 0)
		keyWidgets[_index - 1]->ToClearedKey();
	keyWidgets[_index]->ToCurrentKey();
}

void UUI_ChestGameQTEWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetVisibility() == ESlateVisibility::Visible)
	{
		fTime -= InDeltaTime;
		timePB->SetPercent(fTime / fMaxTime);
	}	
}
