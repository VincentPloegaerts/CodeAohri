#pragma once

#include "CoreMinimal.h"
#include "HUD_Main.h"
#include "HUD_Menu.generated.h"

UCLASS()
class INSIDEUS_API AHUD_Menu : public AHUD_Main
{
	GENERATED_BODY()

public:
	AHUD_Menu();
	
private:
	virtual void InitializeHUD() override;
	
	virtual void ShowInitialWidget() override;
};
