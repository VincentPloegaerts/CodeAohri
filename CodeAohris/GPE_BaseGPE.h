#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GPE_BaseGPE.generated.h"

USTRUCT()
struct FGPE_BaseJson
{
	GENERATED_BODY()

	UPROPERTY()
	bool enable = true;
};

class US_GameSave;

UCLASS(Abstract)
class INSIDEUS_API AGPE_BaseGPE : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	bool enable = true;
	
	UPROPERTY(VisibleAnywhere, Category = "GPE | Values")
	int32 index = -1;
	UPROPERTY(VisibleAnywhere, Category = "GPE | Values")
	FString selfSaveJson = "";
	UPROPERTY(VisibleAnywhere, Category = "GPE | Values")
	US_GameSave* gameSave = nullptr;

public:
	FORCEINLINE void SetGameSave(US_GameSave* _gameSave) { gameSave = _gameSave; }

#if WITH_EDITOR
	FORCEINLINE void SetIndex(const int32& _index) { index = _index; }
#endif
	
public:
	AGPE_BaseGPE();

#pragma region GPE
	//Convert the given FString to UStruct to initialize GPE Values (Called by the GPE Manager at BeginPlay)
	virtual void InitializeGPE(const FString& _jsonStr);
	//Allow to disable the GPE (stop tick, hide in game, ...)
	virtual void DisableGPE();
	//Allow to enable the GPE (enable tick, show in game, ...)	
	virtual void EnableGPE();

	void SetEnableGPE(const bool _enable);
#pragma endregion

#pragma region Save
	//Get Json data, and save it in GameSave
	UFUNCTION() void SaveGPE();
	//Get Json data (converted to FString) and keep it for Reset GPE
	UFUNCTION() void SelfSaveGPE();
	//Reset the GPE with the Self Save Json
	UFUNCTION() void ResetGPE();
#pragma endregion 

#pragma region Json
protected:
	//virtual function, by default only get the Base Json values (enable/disable)
	virtual FString GetGPEJson() const;

	//Set the base value in the given Json
	void GetGPEBaseJson(FGPE_BaseJson& _json) const;
#pragma endregion 
};
