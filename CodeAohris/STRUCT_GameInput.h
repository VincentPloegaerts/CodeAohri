#pragma once

#include "GameInputKeyPreset.h"

#include "STRUCT_InputData.h"

#include "STRUCT_GameInput.generated.h"

USTRUCT(BlueprintType)
struct FGameInput
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inputs | Mode")
	bool IsQwerty = false;

	#pragma region Action
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Action | Character")
	FInputActionData PrimaryShotAction = FInputActionData("Primary Shot",					ACTION_PRESET_SHOT,					true);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Action | Character")
	FInputActionData SlowMotionAction = FInputActionData("Slow Motion",						ACTION_PRESET_SLOW_MOTION,				true);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Action | Character")
	FInputActionData DashAction = FInputActionData("Dash",									ACTION_PRESET_DASH,					true);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Action | Character")
	FInputActionData InteractionAction = FInputActionData("Interaction",					ACTION_PRESET_INTERACTION,				true);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Action | Character")
	FInputActionData JumpAction = FInputActionData("Jump",									ACTION_PRESET_JUMP,					true);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Action | Character")
	FInputActionData PauseGameAction = FInputActionData("Pause Game",						ACTION_PRESET_PAUSE,					false);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Action | Mini Game")
	FInputActionData MashChestAction = FInputActionData("Mash Chest",						ACTION_PRESET_MASH,				false);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Action | Mini Game")
	FInputActionData QTEKeyboardAction = FInputActionData("Keyboard QTE",					ACTION_PRESET_QTE_KEYBOARD(IsQwerty),	false);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Action | Mini Game")
	FInputActionData QTEGamepadAction = FInputActionData("Gamepad QTE",						ACTION_PRESET_QTE_GAMEPAD,			false);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Action | UI")
	FInputActionData UIBackAction = FInputActionData("UI Back",								ACTION_PRESET_UI_BACK,					false);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Action | UI")
	FInputActionData UILeftAction = FInputActionData("UI Left",								ACTION_PRESET_UI_LEFT(IsQwerty),			false);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Action | UI")
	FInputActionData UIRightAction = FInputActionData("UI Right",							ACTION_PRESET_UI_RIGHT(IsQwerty),			false);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Action | UI")
	FInputActionData UIResetInputAction = FInputActionData("UI Reset Input",				ACTION_PRESET_UI_RESET_INPUT,			false);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Action | Menu Save")
	FInputActionData MenuSaveNextAction = FInputActionData("Menu Save Next",				ACTION_PRESET_MENU_SAVE_NEXT(IsQwerty),		false);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Action | Menu Save")
	FInputActionData MenuSavePreviousAction = FInputActionData("Menu Save Previous",		ACTION_PRESET_MENU_SAVE_PREVIOUS(IsQwerty),	false);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Action | Menu Save")
	FInputActionData MenuSaveSelectAction = FInputActionData("Menu Save Select",			ACTION_PRESET_MENU_SAVE_SELECT,		false);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Action | Menu Save")
	FInputActionData MenuSaveDeleteAction = FInputActionData("Menu Save Delete",			ACTION_PRESET_MENU_SAVE_DELETE,		false);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Action | Misc")
	FInputActionData AnyKeyAction = FInputActionData("AnyKey",								ACTION_PRESET_ANY_KEY,					false);
	#pragma endregion

	#pragma region Axis
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Axis | Character")
	FInputAxisData VerticalMovementAxis = FInputAxisData("Move Forward/Backward",			AXIS_PRESET_VERTICAL_MOVEMENT(IsQwerty),	true);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Axis | Character")
	FInputAxisData HorizontalMovementAxis = FInputAxisData("Move Left/Right",				AXIS_PRESET_HORIZONTAL_MOVEMENT(IsQwerty),	true);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Axis | Camera")
	FInputAxisData CameraPitchKeyboardAxis = FInputAxisData("Camera Pitch Keyboard",		AXIS_PRESET_CAMERA_PITCH_KEYBOARD,		false);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Axis | Camera")
	FInputAxisData CameraYawKeyboardAxis = FInputAxisData("Camera Yaw Keyboard",			AXIS_PRESET_CAMERA_YAW_KEYBOARD,		false);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Axis | Camera")
	FInputAxisData CameraPitchGamepadAxis = FInputAxisData("Camera Pitch Gamepad",			AXIS_PRESET_CAMERA_PITCH_GAMEPAD,		false);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Axis | Camera")
	FInputAxisData CameraYawGamepadAxis = FInputAxisData("Camera Yaw Gamepad",				AXIS_PRESET_CAMERA_YAW_GAMEPAD,		false);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Axis | Mini Game")
	FInputAxisData ElevatorAxis = FInputAxisData("Elevator",								AXIS_PRESET_ELEVATOR(IsQwerty),			false);
	#pragma endregion

	#pragma region Cheat Code
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Cheat Code")
	FInputActionData CheatGodModeAction = FInputActionData("Cheat God Mode",			ACTION_PRESET_CHEAT_GOD_MODE,			false);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Cheat Code")
	FInputActionData CheatTPArena1Action = FInputActionData("Cheat TP Arena One",		ACTION_PRESET_CHEAT_TP_ARENA_ONE,		false);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Cheat Code")
	FInputActionData CheatTPArena2Action = FInputActionData("Cheat TP Arena Two",		ACTION_PRESET_CHEAT_TP_ARENA_TWO,		false);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Cheat Code")
	FInputActionData CheatTPArena3Action = FInputActionData("Cheat TP Arena Three",		ACTION_PRESET_CHEAT_TP_ARENA_THREE,	false);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Cheat Code")
	FInputActionData CheatBossStartAction = FInputActionData("Cheat Start Boss",		ACTION_PRESET_CHEAT_BOSS_START,		false);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Cheat Code")
	FInputActionData CheatBossKillAction = FInputActionData("Cheat Kill Boss",			ACTION_PRESET_CHEAT_BOSS_KILL,			false);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs | Cheat Code")
	FInputActionData CheatSkipTutorialAction = FInputActionData("Cheat Skip Tutorial",	ACTION_PRESET_CHEAT_GOD_MODE,			false);
	#pragma endregion	
	
	TMap<FName, FInputActionData*> InputActionData = { };
	TMap<FName, FInputAxisData*> InputAxisData = { };
	
	FGameInput() { }
	
	FGameInput(const bool _isQwerty) : IsQwerty(_isQwerty) { }	
	
	//Generate each Action & Axis Mappings
	void GenerateMappings()
	{
		GenerateActionMappings();
		GenerateAxisMappings();
	}

	//Generate each Action & Axis Mappings
	void GenerateMappings(TArray<FInputActionData>& _actionInputs, TArray<FInputAxisData>& _axisInputs)
	{
		GenerateActionMappings();
		_actionInputs = GetInputActionData();
		
		GenerateAxisMappings();
		_axisInputs = GetInputAxisData();
	}
	
	//Generate each Action Mappings and store them in Input Action Data
	void GenerateActionMappings()
	{
		InitInputActionData(PrimaryShotAction);
		InitInputActionData(SlowMotionAction);
		InitInputActionData(DashAction);
		InitInputActionData(InteractionAction);
		InitInputActionData(JumpAction);
		InitInputActionData(PauseGameAction);
		
		InitInputActionData(MashChestAction);
		InitInputActionData(QTEKeyboardAction);
		InitInputActionData(QTEGamepadAction);
		
		InitInputActionData(UIBackAction);
		InitInputActionData(UILeftAction);
		InitInputActionData(UIRightAction);
		InitInputActionData(UIResetInputAction);
		
		InitInputActionData(MenuSaveNextAction);
		InitInputActionData(MenuSavePreviousAction);
		InitInputActionData(MenuSaveSelectAction);
		InitInputActionData(MenuSaveDeleteAction);
		
		InitInputActionData(AnyKeyAction);

		#if !UE_BUILD_SHIPPING
		#pragma region Cheat Code
		InitInputActionData(CheatGodModeAction);

		InitInputActionData(CheatTPArena1Action);
		InitInputActionData(CheatTPArena2Action);
		InitInputActionData(CheatTPArena3Action);

		InitInputActionData(CheatBossStartAction);
		InitInputActionData(CheatBossKillAction);

		InitInputActionData(CheatSkipTutorialAction);
		#pragma endregion
		#endif
	}

	//Generate each Axis Mappings and store them in Input Axis Data
	void GenerateAxisMappings()
	{
		InitInputAxisData(VerticalMovementAxis);
		InitInputAxisData(HorizontalMovementAxis);
		
		InitInputAxisData(CameraPitchKeyboardAxis);
		InitInputAxisData(CameraYawKeyboardAxis);
		InitInputAxisData(CameraPitchGamepadAxis);
		InitInputAxisData(CameraYawGamepadAxis);

		InitInputAxisData(ElevatorAxis);
	}

	void InitInputActionData(FInputActionData& _action)
	{
		_action.GenerateActionMappings();
		InputActionData.Add(_action.ActionName, &_action);
	}
	void InitInputAxisData(FInputAxisData& _axis)
	{
		_axis.GenerateAxisMappings();
		InputAxisData.Add(_axis.AxisName, &_axis);
	}
	
	TArray<FInputActionData> GetInputActionData() const
	{
		TArray<FInputActionData> _actionData = { };
		for (const TTuple<FName, FInputActionData*>& _action : InputActionData)
		{
			_actionData.Add(*_action.Value);
		}

		return _actionData;
	}
	TArray<FInputAxisData> GetInputAxisData() const
	{
		TArray<FInputAxisData> _axisData = { };
		for (const TTuple<FName, FInputAxisData*>& _axis : InputAxisData)
		{
			_axisData.Add(*_axis.Value);
		}

		return _axisData;
	}
	
	void UpdateActionMappings(const FInputActionData& _action)
	{
		if (!InputActionData.Contains(_action.ActionName)) return;
		InputActionData[_action.ActionName]->Mappings = _action.Mappings;
	}
	void UpdateAxisMappings(const FInputAxisData& _axis)
	{
		if (!InputAxisData.Contains(_axis.AxisName)) return;
		InputAxisData[_axis.AxisName]->Mappings = _axis.Mappings;
	}
};