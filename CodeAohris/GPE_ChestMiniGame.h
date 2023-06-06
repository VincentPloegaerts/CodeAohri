#pragma once
#include "CoreMinimal.h"
#include "HUD_Game.h"
#include "IU_Player.h"
#include "STRUCT_InputData.h"
#include "UI_ChestGameMashWidget.h"
#include "Components/ActorComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "GPE_ChestMiniGame.generated.h"


class UUI_ChestGameQTEWidget;
class UInputSettings;
class AGPE_Chest;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INSIDEUS_API UGPE_ChestMiniGame : public UActorComponent
{
	GENERATED_BODY()
#pragma region Properties
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameEnd,bool,_succeeded);
	UPROPERTY(EditAnywhere,Category="Minigame")
		float failDamage = 10.0f;
	UPROPERTY(EditAnywhere,Category="Minigame|Camera")
		float cameraPitch = 20.0f;
	UPROPERTY(EditAnywhere,Category="Minigame|Camera")
		float cameraYaw = 0.0f;
#pragma region Mash
	UPROPERTY(EditAnywhere,Category="Minigame|Mash|Values")
		int inputQuantityNeeded = 50;
	UPROPERTY(VisibleAnywhere,Category="Minigame|Mash|Values")
		int inputCount = 0;
	UPROPERTY(EditAnywhere,Category="Minigame|Mash|Values")
		float timeToMash = 7.0f;
#pragma endregion
		
#pragma region QTE
	UPROPERTY(EditAnywhere,Category="Minigame|QTE|Values")
		int inputCountQTE = 5;
	UPROPERTY(EditAnywhere,Category="Minigame|Mash|Values")
		float timeForQTE = 7.0f;
	UPROPERTY(VisibleAnywhere,Category="Minigame|QTE|Values")
		TArray<FKey> keyArrayQTE;
#pragma endregion

#pragma region Opti Properties
	UPROPERTY()
		int currentAskedInput = 0;
	UPROPERTY()
		bool usingKeyBoard = true;
	UPROPERTY()
		UInputComponent* inputComponentRef = nullptr;
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		AIU_Player* playerRef = nullptr;
	UPROPERTY()
		TArray<FKey> keyboardQTEKeys;
	UPROPERTY()
		TArray<FKey> gamepadQTEKeys;

	UPROPERTY()
	class UM_InputManager* inputManager = nullptr;
	
	UPROPERTY()
		AHUD_Game* hud = nullptr;
	UPROPERTY()
		UUI_ChestGameMashWidget* mashWidget;
	UPROPERTY()
		UUI_ChestGameQTEWidget* QTEWidget;
	
	UPROPERTY()
		bool countedInputDuringFrame = false;
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = "True"))
		FTimerHandle handle;
	
	UPROPERTY(EditAnywhere,BlueprintAssignable,BlueprintCallable,Category="Minigame|Event")
		FOnGameEnd onGameEnded;
#pragma endregion

	UPROPERTY()
	AGPE_Chest* chestOwner = nullptr;

#pragma endregion
public:	
	UGPE_ChestMiniGame();

	UFUNCTION() FORCEINLINE void SetUsingKeyBoardTrue()
	{
		usingKeyBoard = true;
		if (mashWidget)
			mashWidget->SetUsingKeyboard(true);
	}
	UFUNCTION() FORCEINLINE void SetUsingKeyBoardFalse()
	{
		usingKeyBoard = false;
		if (mashWidget)
			mashWidget->SetUsingKeyboard(false);
	}
	UFUNCTION() FORCEINLINE void ChangeMiniGameAskedInput(){usingKeyBoard = false;}
	FORCEINLINE FOnGameEnd& OnGameEnded() {return onGameEnded;}
	FORCEINLINE void SetChestOwner(AGPE_Chest* _chestOwner) {chestOwner =  _chestOwner;}
	
	UFUNCTION(BlueprintCallable) void StartMiniGame();
	
private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable) void Init();
	UFUNCTION(BlueprintCallable) void InitKeys();
	
	UFUNCTION(BlueprintCallable) void PressedGameKeyKeyBoard(FKey _key);
	UFUNCTION(BlueprintCallable) void PressedGameKeyGamePad(FKey _key);
	UFUNCTION(BlueprintCallable) void StartQTE();
	UFUNCTION(BlueprintCallable) void Mashing();
	UFUNCTION(BlueprintCallable) void SuccessQTE();
	UFUNCTION(BlueprintCallable) void FailQTE();
	UFUNCTION(BlueprintCallable) void ExitGPE();
	UFUNCTION(BlueprintCallable) void TimeExpired();

	UFUNCTION() void OnInputReset(UM_InputManager* _inputManager);
	UFUNCTION() void OnAxisRebind(const FInputAxisData& _input, FInputAxisKeyMapping _oldMapping, FInputAxisKeyMapping _newMapping);
};
