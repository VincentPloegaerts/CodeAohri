#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "ENUM_WidgetEnum.h"

#include "HUD_Main.generated.h"

class UUI_Widget;

UCLASS(Abstract)
class INSIDEUS_API AHUD_Main : public AHUD
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKeyType);
	
protected:
	//Widget to create
	UPROPERTY(EditAnywhere, Category = "Settings")
	TMap<TEnumAsByte<EWidgetEnum>, TSubclassOf<UUI_Widget>> widgetTypes = { };

	//Map of created widgets
	UPROPERTY()
	TMap<TEnumAsByte<EWidgetEnum>, UUI_Widget*> widgets = { };
	
	//Heap of menu widget (used to GoBack on menu)
	UPROPERTY()
	TArray<UUI_Widget*> menuWidgetHeap = { };

	//Current Widget (Menu) that is visible
	UPROPERTY()
	UUI_Widget* visibleMenuWidget = nullptr;
	
	UPROPERTY()
	APlayerController* playerController = nullptr;

private:
	bool bGamepadKey = false;
	float mouseX = 0;
	float mouseY = 0;
	
	UPROPERTY()
	FOnKeyType onGamepadKey;
	UPROPERTY()
	FOnKeyType onKeyboardKey;
	
public:
	//Return a casted widget assigned to the given Enum (if Exist)
	template<class ClassType>
	ClassType* GetWidget(const EWidgetEnum& _widgetEnum)
	{
		return Cast<ClassType>(GetWidgetUI(_widgetEnum));
	}
	
	//Return default widget type ptr assigned to the given Enum (if exist)
	UUI_Widget* GetWidgetUI(const EWidgetEnum& _widgetEnum);
	//Show a widget assigned to the given Enum and return it
	void ShowMenuWidget(const EWidgetEnum& _widgetEnum, const bool _playShowAnimation = false);
	//Hide the current visible Widget and Show the previous one
	void GoBackMenu(const bool _playShowAnimation = false);
	//Show the Confirm Widget and return it
	class UUI_ConfirmWidget* GetConfirmWidget();
	//Show the Difficulty Menu Widget and return it
	class UUI_DifficultyMenuWidget* GetDifficultyWidget();
	//Show the PopUp Widget and return it
	class UUI_PopUpWidgetWidget* GetPopUpWidgetWidget();

	FORCEINLINE UUI_Widget* GetCurrentMenuWidget() const { return visibleMenuWidget; }
	FORCEINLINE const bool& IsGamepadVisual() const { return bGamepadKey; }
	FORCEINLINE FOnKeyType& OnGamepadKey() { return onGamepadKey; }
	FORCEINLINE FOnKeyType& OnKeyboardKey() { return onKeyboardKey; }
	
private:
	virtual void BeginPlay() override;
	void InitializeWidgets();

	//Check if a Widget assign to the given enum exist (Try to create if doesn't exist)  
	bool WidgetExist(const EWidgetEnum& _widgetEnum);
	//Create a Widget assign with the Type assign to the given enum 
	void CreateWidgetHUD(const EWidgetEnum& _widgetEnum);

	UFUNCTION() void UpdateKeyType(FKey _key);
	UFUNCTION() void UpdateKeyTypeFromMouse(float _axis);
	UFUNCTION() void CheckMouseCursor();

	void ToGamepad();
	void ToKeyboard();
	
protected:
	virtual void InitializeHUD();
	virtual void InitializeInput();
	
public:
	UFUNCTION(BlueprintCallable) virtual void ShowInitialWidget();
};
