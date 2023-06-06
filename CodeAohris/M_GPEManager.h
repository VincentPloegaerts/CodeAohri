#pragma once

#include "CoreMinimal.h"
#include "GPE_BaseGPE.h"
#include "GameFramework/Actor.h"
#include "M_GPEManager.generated.h"

class AGPE_BaseGPE;
class US_GameSave;

UCLASS()
class INSIDEUS_API AM_GPEManager : public AActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveGPE);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckPointGPE);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResetGPE);
	UPROPERTY(VisibleAnywhere, Category = "GPE Manager")
		int saveIndex = -1;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBillboardComponent* billboard = nullptr;
#endif
	
	UPROPERTY(VisibleAnywhere, Category = "GPE Manager")
	TMap<int32, AGPE_BaseGPE*> gpe = { };
	
	UPROPERTY(EditAnywhere, Category = "GPE Manager EDITOR")
	TArray<TSubclassOf<AGPE_BaseGPE>> gpeToIgnore = { };
	
	//Event used to trigger GPE Save (save their values in Game Save)
	UPROPERTY()
	FOnSaveGPE onSaveGPE;
	//Event used to trigger GPE Save (save their values for themself)
	UPROPERTY()
	FOnCheckPointGPE onCheckPointGPE;
	//Event used to Reset GPE (on player respawn)
	UPROPERTY()
	FOnResetGPE onResetGPE;
	
public:	
	AM_GPEManager();

private:
	virtual void BeginPlay() override;

	UFUNCTION() void InitializeManager();
	void InitializeGPEFromGameSave();

	UFUNCTION() void OnCheckpoint(const bool _saveProgression);
	void BroadcastSaveGPE();
	void BroadcastCheckPointGPE();
	UFUNCTION() void BroadcastResetGPE();
	
#if WITH_EDITOR
private:
    UFUNCTION(CallInEditor, Category = "GPE Manager EDITOR") void InitializeGPE();
	bool IsToIgnore(const AGPE_BaseGPE* _gpe);
	
	UFUNCTION(CallInEditor, Category = "GPE Manager TEST") void SimulateSaveGPE();
	UFUNCTION(CallInEditor, Category = "GPE Manager TEST") void SimulateCheckPointGPE();
	UFUNCTION(CallInEditor, Category = "GPE Manager TEST") void SimulateResetGPE();
#endif
};
