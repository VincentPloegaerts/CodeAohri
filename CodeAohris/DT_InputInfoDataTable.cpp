#include "DT_InputInfoDataTable.h"

#include "M_SaveManager.h"
#include "Kismet/KismetInputLibrary.h"

bool UDT_InputInfoDataTable::GetInputInfoForKey(const FKey& _key, FInputInfo& _info) const
{
	if (!InputInfo.Contains(_key)) return false;

	_info = InputInfo[_key];
	return true;
}

FInputInfo UDT_InputInfoDataTable::GetInputInfoForKey(const FKey& _key) const
{
	if (!InputInfo.Contains(_key)) return FInputInfo(_key.GetDisplayName());

	return InputInfo[_key];
}

#if WITH_EDITOR
void UDT_InputInfoDataTable::Save()
{
	const FInputInfoJSON& _json = FInputInfoJSON(InputInfo);

	UM_SaveManager::Save(_json, ESaveType::Save_Input, GetName());
}
void UDT_InputInfoDataTable::Load()
{
	FInputInfoJSON _json = FInputInfoJSON();
	if (!UM_SaveManager::Load(_json, ESaveType::Save_Input, GetName())) return;
	
	InputInfo = _json.InputInfo;
}

void UDT_InputInfoDataTable::GetKeys()
{
	TArray<FKey> _keys;
	EKeys::GetAllKeys(_keys);

	for (const FKey& _key : _keys)
	{
		if (UseKeyNameFilter)
			if (!_key.GetFName().ToString().Contains(KeyNameFilter)) continue;
		
		if (_key.IsGamepadKey() && GetGamepadKeys && !InputInfo.Contains(_key))
			InputInfo.Add(_key, FInputInfo(_key.GetDisplayName()));

		if (UKismetInputLibrary::Key_IsKeyboardKey(_key) && GetKeyboardKeys && !InputInfo.Contains(_key))
			InputInfo.Add(_key, FInputInfo(_key.GetDisplayName()));
	}
}

void UDT_InputInfoDataTable::ResetKeys()
{
	InputInfo.Empty();

	GetKeys();
}
#endif
