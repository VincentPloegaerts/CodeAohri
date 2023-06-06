#pragma once
#include "ENUM_SaveType.generated.h"

UENUM(BlueprintType)
enum ESaveType
{
	Save_Cache UMETA(DisplayName = "Cache"),
	Save_Projectile UMETA(DisplayName = "Projectile"),
	Save_Pattern UMETA(DisplayName = "Pattern"),
	Save_Serie UMETA(DisplayName = "Serie"),
	Save_BehaviorTree_Task UMETA(DisplayName = "BehaviorTree_Task"),
	Save_UIManager UMETA(DisplayName = "UIManager"),
	Save_Player UMETA(DisplayName = "Player"),
	Save_CritPoint UMETA(DisplayName = "Crit Point"),
	Save_Input UMETA(DisplayName = "Input")	
};