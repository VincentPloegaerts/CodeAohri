#include "M_UIManager.h"

#include "UI_FloatingWidget.h"
#include "UI_DamageFloatingWidget.h"

#include "IU_Macros.h"

#include "UI_Widget.h"

AM_UIManager::AM_UIManager()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void AM_UIManager::ShowDamage(const float& _damage, const EHitFeedbackEnum& _enum, const FVector& _position)
{
	if(const AUI_DamageFloatingWidget* _damageFloatingWidget = Cast<AUI_DamageFloatingWidget>(CreateFloatingWidget(FLOATING_WIDGET_DAMAGE, _position)))
		_damageFloatingWidget->SetType(_damage, _enum);
}

AUI_FloatingWidget* AM_UIManager::CreateFloatingWidget(const EFloatingWidgetEnum& _floatingWidgetEnum, const FVector& _location)
{
	if (!floatingWidgetTypes.Contains(_floatingWidgetEnum)) return nullptr;
	if (const TSubclassOf<AUI_FloatingWidget>& _type = floatingWidgetTypes[FLOATING_WIDGET_DAMAGE])
		return GetWorld()->SpawnActor<AUI_FloatingWidget>(_type, _location, GetActorRotation());
	else
	{
		LOG("AM_UIManager::CreateFloatingWidget => Floating Widget Type is not valid for %s", *UEnum::GetValueAsString<EFloatingWidgetEnum>(_floatingWidgetEnum));
		return nullptr;
	}
}