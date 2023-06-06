#include "UI_CreditWidget.h"

#include "STRUCT_GameInput.h"

void UUI_CreditWidget::SetVisibility(ESlateVisibility InVisibility)
{
	if (InVisibility == ESlateVisibility::Visible)
		ResetScroll();
	
	Super::SetVisibility(InVisibility);
}

void UUI_CreditWidget::InitializeWidget()
{
	Super::InitializeWidget();

	if (UInputComponent* _component =  GetWorld()->GetFirstPlayerController()->InputComponent)
	{
		const FGameInput& _gameInput = FGameInput();
		FInputAxisBinding& _bind = _component->BindAxis(_gameInput.VerticalMovementAxis.AxisName, this, &UUI_CreditWidget::GamepadScroll);
		_bind.bConsumeInput = false;
	}
}

void UUI_CreditWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateScrolling(InDeltaTime);	
}

void UUI_CreditWidget::UpdateScrolling(const float& _deltaTime)
{
	if (CanScroll())
	{
		const float& _scrollOffset = _deltaTime * fScrollSpeed + CreditSB->GetScrollOffset();
		CreditSB->SetScrollOffset(_scrollOffset);
		
		if (IsScrollComplete())
		{
			Back();
			onCreditFinished.Broadcast();
		}
	}
}

void UUI_CreditWidget::ResetScroll()
{
	CreditSB->ScrollToStart();
}

void UUI_CreditWidget::GamepadScroll(float _axis)
{
	if (IsVisible())
	{
		fScrollSpeed = fNormalScrollSpeed + -_axis * fGamepadScrollSpeed;
	}
}
