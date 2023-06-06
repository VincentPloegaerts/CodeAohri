#pragma once

#include "CoreMinimal.h"
#include "HUD_Main.h"

#include "UI_Widget.h"

#include "HUD_Game.generated.h"

class AM_GameMode;
UCLASS()
class INSIDEUS_API AHUD_Game : public AHUD_Main
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGamePause);
	
	//Map of visible Game Widget 
	UPROPERTY()
	TMap<TEnumAsByte<EWidgetEnum>, UUI_Widget*> gameWidgetMap = { };
	
	UPROPERTY() 
	AM_GameMode* gameMode = nullptr;

	bool bMenuPause = false;
	
protected:
	UPROPERTY(BlueprintCallable, BlueprintReadWrite, BlueprintAssignable)
	FOnGamePause onGamePause = FOnGamePause();
	UPROPERTY(BlueprintCallable, BlueprintReadWrite, BlueprintAssignable)
	FOnGamePause onGameUnPause = FOnGamePause();
		
public:
	FORCEINLINE FOnGamePause& OnGamePause() { return onGamePause; }
	
protected:
	UFUNCTION(BlueprintImplementableEvent) void OnPause();

	bool CanPause() const;

	bool IsWidgetVisible(const EWidgetEnum& _widget) const;
	
public:
	AHUD_Game();
	
	virtual void InitializeHUD() override;
	virtual void ShowInitialWidget() override;
	
	UFUNCTION() void MenuPause();

	UFUNCTION() void ShowBossWidget();
	UFUNCTION() void HideBossWidget();

	UFUNCTION(BlueprintCallable) void ShowLoadScreen();
	UFUNCTION(BlueprintCallable) void HideLoadScreen();

	void ShowCinematicWidget();
	void HideCinematicWidget();
	
	void ShowGameWidgets() const;
	void HideGameWidgets() const;

	//Go back in all menu widget from heap and unpause the game
	void GoBackAllMenuWidget();
	
	UFUNCTION(BlueprintCallable) UUI_Widget* ShowGameWidget(const EWidgetEnum& _widgetEnum);
	UFUNCTION(BlueprintCallable) void HideGameWidget(const EWidgetEnum& _widgetEnum);

	UFUNCTION(BlueprintCallable) void ShowCredits(); 
	
private:
	virtual void InitializeInput() override;

	void ShowMenuPause();
	void HideMenuPause();

	UFUNCTION() void LockPlayer() const;

	UFUNCTION() void ReturnToMenu();
};
