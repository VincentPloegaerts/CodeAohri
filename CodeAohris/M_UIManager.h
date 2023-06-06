#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ENUM_WidgetEnum.h"
#include "ENUM_FloatingWidgetEnum.h"
#include "ENUM_HitFeedbackEnum.h"

#include "M_UIManager.generated.h"

class AUI_GameHUD;

class UUI_Widget;
class UUI_ConfirmWidget;

class AUI_FloatingWidget;

UCLASS()
class INSIDEUS_API AM_UIManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Settings")
	TMap<TEnumAsByte<EFloatingWidgetEnum>, TSubclassOf<AUI_FloatingWidget>> floatingWidgetTypes =
	{
		{FLOATING_WIDGET_DAMAGE, nullptr}
	};
	
public:
	AM_UIManager();
	
	void ShowDamage(const float& _damage, const EHitFeedbackEnum& _enum, const FVector& _position);
	
private:
	AUI_FloatingWidget* CreateFloatingWidget(const EFloatingWidgetEnum& _floatingWidgetEnum, const FVector& _location);

};