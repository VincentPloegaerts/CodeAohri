#include "DT_InputDataTable.h"

#if WITH_EDITOR
#include "M_SaveManager.h"
#endif

#if WITH_EDITOR
void UDT_InputDataTable::ResetInput()
{
	GameInputAZERTY = FGameInput(false);
	GameInputQWERTY = FGameInput(true);
}

void UDT_InputDataTable::Save()
{
	FInputDataTableJSON _json = FInputDataTableJSON();
	_json.GameInputAZERTY = GameInputAZERTY;
	_json.GameInputQWERTY = GameInputQWERTY;

	UM_SaveManager::Save(_json, ESaveType::Save_Input, GetName());
}
void UDT_InputDataTable::Load()
{
	FInputDataTableJSON _json = FInputDataTableJSON();
	if (!UM_SaveManager::Load(_json, ESaveType::Save_Input, GetName())) return;
	
	GameInputAZERTY = _json.GameInputAZERTY;
	GameInputQWERTY = _json.GameInputQWERTY;
}
#endif