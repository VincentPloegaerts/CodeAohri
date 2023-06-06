#pragma once
#include "CoreMinimal.h"

#include "IU_Player.h"
#include "IU_MovingOnGroundState.h"
#include "IU_DashingState.h"
#include "IU_JumpingState.h"
#include "IU_FallingState.h"

#include "Components/ActorComponent.h"
#include "IU_PlayerFSMBrain.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INSIDEUS_API UIU_PlayerFSMBrain : public UActorComponent
{
	GENERATED_BODY()

	#pragma region Properties

	#pragma region References
	
	// Ref on owner casted in AIU_Player
	UPROPERTY(VisibleAnywhere, Category = "Brain | References",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		AIU_Player* playerOwner = nullptr;

	#pragma endregion

	#pragma region States
	
	// Current state ticking its update 
	UPROPERTY(VisibleAnywhere, Category = "Brain | States", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UIU_State* currentState = nullptr;

	// Current state ticking its update 
	UPROPERTY(VisibleAnywhere, Category = "Brain | States", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UIU_State* nextState = nullptr;
	
	// Current state ticking its update 
	UPROPERTY(VisibleAnywhere, Category = "Brain | States", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool bufferDash = false;
	// Current state ticking its update 
	UPROPERTY(VisibleAnywhere, Category = "Brain | States", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool bufferJump = false;

	#pragma region MovingState

	// Class type of state when creating it
	UPROPERTY(EditAnywhere, Category = "Brain | States", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TSubclassOf<UIU_MovingOnGroundState> movingGroundStateRef = TSubclassOf<UIU_MovingOnGroundState>();
	
	// Ref on state so creating it is not needed each time
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UIU_MovingOnGroundState* movingOnGroundState = nullptr;

	#pragma endregion

	#pragma region JumpingState

	// Class type of state when creating it
	UPROPERTY(EditAnywhere, Category = "Brain | States", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TSubclassOf<UIU_JumpingState> jumpingStateRef = TSubclassOf<UIU_JumpingState>();

	// Ref on state so creating it is not needed each time
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UIU_JumpingState* jumpingState = nullptr;
	
	#pragma endregion

	#pragma region DashingState

	// Class type of state when creating it
	UPROPERTY(EditAnywhere, Category = "Brain | States", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TSubclassOf<UIU_DashingState> dashingStateRef = TSubclassOf<UIU_DashingState>();

	// Ref on state so creating it is not needed each time
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UIU_DashingState* dashingState = nullptr;

	#pragma endregion

	#pragma region FallingState
	
	// Class type of state when creating it
	UPROPERTY(EditAnywhere, Category = "Brain | States",BlueprintReadWrite , meta = (AllowPrivateAccess = True))
		TSubclassOf<UIU_FallingState> fallingStateRef = TSubclassOf<UIU_FallingState>();

	// Ref on state so creating it is not needed each time
	UPROPERTY(BlueprintReadWrite , meta = (AllowPrivateAccess = True))
		UIU_FallingState* fallingState = nullptr;

	#pragma endregion

	#pragma endregion

	#pragma endregion
	
public:
	FORCEINLINE void SetBufferDash(bool _status) { bufferDash = _status;}
	FORCEINLINE bool GetBufferDash() const {return bufferDash ;}
	
	FORCEINLINE void SetBufferJump(bool _status) { bufferJump = _status;}
	FORCEINLINE bool GetBufferJump() const {return bufferJump ;}
	
	FORCEINLINE bool IsSwitchingState() const {return currentState != nextState;}
	FORCEINLINE AIU_Player* PlayerOwner() const { return playerOwner; }
	FORCEINLINE UIU_State* CurrentState() const { return currentState; }
	FORCEINLINE UIU_State* NextState() const { return nextState; }
	FORCEINLINE EStateType CurrentStateType() const { return currentState ? currentState->StateType() : None; }
	FORCEINLINE UIU_JumpingState* JumpingState() const {return jumpingState; }
	FORCEINLINE UIU_FallingState* FallingState() const {return fallingState; }
	FORCEINLINE UIU_DashingState* DashingState() const {return dashingState; }
	FORCEINLINE UIU_MovingOnGroundState* MovingState() const {return movingOnGroundState; }
	
public:
	UIU_PlayerFSMBrain();
	
private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//Init properties
	UFUNCTION(BlueprintCallable) void Init();
	
public:
	//Switch current state and calling Exit and Enter accordingly
	 void SwitchCurrentState(UIU_State* _newState);
	
	//Switch current state and calling Exit and Enter accordingly using EStateType enum
	UFUNCTION(BlueprintCallable) void SwitchCurrentState(TEnumAsByte<EStateType> _newState);
#if  WITH_EDITOR
	UFUNCTION(BlueprintCallable) void PassALlDatas(const FMovementData& _datas) const;
#endif
};