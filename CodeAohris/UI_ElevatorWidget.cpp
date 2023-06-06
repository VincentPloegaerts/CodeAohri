// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ElevatorWidget.h"

void UUI_ElevatorWidget::SetLeftKeyImage(const FKey& _key)
{
	leftKeyWidget->InitializeInputInfoWidget(_key);
	leftKeyWidget->SetRenderOpacity(1.0f);
}

void UUI_ElevatorWidget::SetRightKeyImage(const FKey& _key)
{
	rightKeyWidget->InitializeInputInfoWidget(_key);
	rightKeyWidget->SetRenderOpacity(1.0f);
}

void UUI_ElevatorWidget::SetGamepadKeyImage(const FKey& _key)
{
	gamepadWidget->InitializeInputInfoWidget(_key);
	gamepadWidget->SetRenderOpacity(1.0f);
}

void UUI_ElevatorWidget::HideLeftKeyImage()
{
	leftKeyWidget->SetRenderOpacity(0.0f);
}

void UUI_ElevatorWidget::HideRightKeyImage()
{
	rightKeyWidget->SetRenderOpacity(0.0f);
}

void UUI_ElevatorWidget::HideGamepadKeyImage()
{
	gamepadWidget->SetRenderOpacity(0.0f);
}
