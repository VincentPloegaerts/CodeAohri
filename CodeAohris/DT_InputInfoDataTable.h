#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DT_InputInfoDataTable.generated.h"

USTRUCT(BlueprintType)
struct FInputInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText InputDisplayName = FText();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* InputTexture = nullptr;
	
	FInputInfo() { }
	
	FInputInfo(const FText& _displayName)
	{
		InputDisplayName = _displayName;
	}
};

USTRUCT()
struct FInputInfoJSON
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FKey, FInputInfo> InputInfo = { };

	FInputInfoJSON() { }
	FInputInfoJSON(const TMap<FKey, FInputInfo>& _inputInfo)
	{
		InputInfo = _inputInfo;
	}
};

UCLASS(BlueprintType, Blueprintable)
class INSIDEUS_API UDT_InputInfoDataTable : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input Info")
	TMap<FKey, FInputInfo> InputInfo = { };

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Init")
	bool GetGamepadKeys = true;
	UPROPERTY(EditAnywhere, Category = "Init")
	bool GetKeyboardKeys = false;
	UPROPERTY(EditAnywhere, Category = "Init")
	bool UseKeyNameFilter = false;
	UPROPERTY(EditAnywhere, Category = "Init", meta =(EditCondition = "UseKeyNameFilter", EditConditionHides))
	FString KeyNameFilter = "Gamepad";
#endif
public:
	bool GetInputInfoForKey(const FKey& _key, FInputInfo& _info) const;
	UFUNCTION(BlueprintCallable) FInputInfo GetInputInfoForKey(const FKey& _key) const;

#if WITH_EDITOR
private:
	UFUNCTION(CallInEditor, Category = "Save") void Save();
	UFUNCTION(CallInEditor, Category = "Save") void Load();

	
	UFUNCTION(CallInEditor, Category = "Init") void GetKeys();
	UFUNCTION(CallInEditor, Category = "Init") void ResetKeys();
#endif
};
