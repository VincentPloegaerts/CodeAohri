#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GPE_BalanceMiniGame.generated.h"


class UUI_ElevatorWidget;
class AIU_Player;
class AHUD_Game;
class UDT_InputInfoDataTable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INSIDEUS_API UGPE_BalanceMiniGame : public UActorComponent
{
	GENERATED_BODY()
#pragma region Properties
#pragma region Values
	//How much percent of the bar does the fail zone takes,x left, y right
	UPROPERTY(EditAnywhere,Category="Minigame|Areas")
		FVector2f vAreaBaseSize;
	//How low can the percent of the bar goes to,x left, y right
	UPROPERTY(EditAnywhere,Category="Minigame|Areas")
		FVector2f vAreaMinimumSize;
	UPROPERTY(EditAnywhere,Category="Minigame|Camera")
		float cameraPitch = 20.0f;
	UPROPERTY(EditAnywhere,Category="Minigame|Camera")
		float cameraYaw = 0.0f;

#pragma region Cursor
	//Where on the bar is the cursor, percent position
	UPROPERTY(VisibleAnywhere,Category="Minigame|Cursor")
		float fCursorPosition = 50.0f;
	//How fast can the player cursor go, x is the start speed and y is the speed after some time on the same input
	UPROPERTY(EditAnywhere,Category="Minigame|Cursor")
		FVector2f vPlayerSpeedRange;
	//How fast can the cursor move by itself (from the miniGame)
	UPROPERTY(EditAnywhere,Category="Minigame|Cursor")
		FVector2f vGameSpeedRange;
	//Random range for the delay between 2 speed changes
	UPROPERTY(EditAnywhere,Category="Minigame|Cursor")
		FVector2f vGameChangeSpeedRange;
	//How fast does the input goes from vPlayerSpeedRange.x to vPlayerSpeedRange.y
	UPROPERTY(EditAnywhere,Category="Minigame|Cursor",meta = (ClampMin = "1.0"))
		float fInputAcceleration= 1.1f;
#pragma endregion

#pragma region Fail
	//How much does the fail areas regress after each fail
	UPROPERTY(EditAnywhere,Category="Minigame|Fail",meta = (ClampMin = "0.0"))
		float fAreasRegression= 1.0f;
	//How fast does the platform fall when there's a fail
	UPROPERTY(EditAnywhere,Category="Minigame|Fail",meta = (ClampMin = "0.0"))
		float fSpeedFail= 100.0f;
	//How long does the platform fall for when there's a fail
	UPROPERTY(EditAnywhere,Category="Minigame|Fail",meta = (ClampMin = "0.01"))
		float fDurationFail= 1.0f;
	//Debug to see if player failed
	UPROPERTY(VisibleAnywhere,Category="Minigame|Fail",BlueprintReadWrite,meta = (AllowPrivateAccess = "True"))
		bool bFailed= false;
#pragma endregion

#pragma region Inputs
	//Input name in the input manager to use for the axis for the game
	UPROPERTY(EditAnywhere,Category="Minigame|Inputs")
		FName gameInputName = "ElevatorInput";
#pragma endregion

#pragma region Debug
	UPROPERTY(VisibleAnywhere,Category="Minigame|Debug")
		bool bPlayerGoingRight = false;
	UPROPERTY(VisibleAnywhere,Category="Minigame|Debug")
		bool bGameGoingRight = false;
	UPROPERTY(VisibleAnywhere,Category="Minigame|Debug")
		float fActualInputSpeed = 5.0f;
	UPROPERTY(VisibleAnywhere,Category="Minigame|Debug")
		float fActualGameSpeed = 5.0f;
	UPROPERTY(VisibleAnywhere,Category="Minigame|Debug",BlueprintReadWrite,meta = (AllowPrivateAccess = "True"))
		FVector2f vAreaCurrentSizeRange;
	UPROPERTY()
		FTimerHandle handle;
	UPROPERTY()
		AIU_Player* playerRef = nullptr;
	UPROPERTY()
		UWorld* world;
	UPROPERTY()
		AHUD_Game* hud;
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		UUI_ElevatorWidget* elevatorWidget;
	UPROPERTY()
		TArray<FInputAxisKeyMapping> mappings;
	UPROPERTY()
		const UDT_InputInfoDataTable* inputInfoData;
	UPROPERTY()
		bool usingKeyBoard = true;
	UPROPERTY(VisibleAnywhere,Category="Minigame|Debug")
		float checkInputQuantity = 0.0f;
	UPROPERTY()
		bool bIsStarted = false;
#pragma endregion

#pragma endregion 
#pragma region Event
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFail,const float&,_failSpeed,const float&,_duration);
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FOnFail onFail;
#pragma endregion 
#pragma endregion 
public:	
	UGPE_BalanceMiniGame();
	FORCEINLINE FOnFail& OnFail() {return onFail;}
	FORCEINLINE float GetCursorPosition() const {return fCursorPosition;}
	UFUNCTION(BlueprintCallable) FORCEINLINE void ChangeFailedStatus() {bFailed = !bFailed;}
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetUsingKeyBoardTrue() { usingKeyBoard = true;}
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetUsingKeyBoardFalse(){ usingKeyBoard = false;}
private:	
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable) void GameInputAxis(float _value);
	UFUNCTION(BlueprintCallable) void Fail();
	UFUNCTION(BlueprintCallable) void ChangeSpeed();
	UFUNCTION(BlueprintCallable) void Init();
public:
	UFUNCTION(BlueprintCallable)void StartGame();
	UFUNCTION(BlueprintCallable)void EndGame();
};
