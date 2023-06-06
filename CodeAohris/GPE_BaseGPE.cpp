#include "GPE_BaseGPE.h"

#include "IU_Macros.h"
#include "S_GameSave.h"

#include "JsonObjectConverter.h"

AGPE_BaseGPE::AGPE_BaseGPE()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGPE_BaseGPE::InitializeGPE(const FString& _jsonStr)
{
	FGPE_BaseJson _json;
	FJsonObjectConverter::JsonObjectStringToUStruct(_jsonStr, &_json);
	
	SetEnableGPE(_json.enable);

	selfSaveJson = _jsonStr;
}
void AGPE_BaseGPE::DisableGPE()
{
	enable = false;
	SetActorTickEnabled(false);
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
}
void AGPE_BaseGPE::EnableGPE()
{
	enable = true;
	SetActorTickEnabled(true);
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
}

void AGPE_BaseGPE::SetEnableGPE(const bool _enable)
{
	if (_enable)
		EnableGPE();
	else
		DisableGPE();
}

void AGPE_BaseGPE::SaveGPE()
{
	selfSaveJson = GetGPEJson();

	if (gameSave)
		gameSave->AddGPEData(index, selfSaveJson);
}
void AGPE_BaseGPE::SelfSaveGPE()
{
	selfSaveJson = GetGPEJson();
}
void AGPE_BaseGPE::ResetGPE()
{
	InitializeGPE(selfSaveJson);
}

FString AGPE_BaseGPE::GetGPEJson() const
{
	FGPE_BaseJson _json;
	GetGPEBaseJson(_json);
	
	FString _jsonStr = "";
	FJsonObjectConverter::UStructToJsonObjectString(_json, _jsonStr);
	
	return _jsonStr;
}

void AGPE_BaseGPE::GetGPEBaseJson(FGPE_BaseJson& _json) const
{
	_json.enable = enable;
}
