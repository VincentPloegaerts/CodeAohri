#pragma once

#pragma region Action Preset

#pragma region PRESET Shot																								//Shot
#define ACTION_KEYS_SHOT_GAMEPAD	FKey(EKeys::Gamepad_RightTrigger)													//Gamepad : Right Trigger 
#define ACTION_KEYS_SHOT_KEYBOARD	FKey(EKeys::LeftMouseButton)														//Keyboard : Mouse Left Button

#define ACTION_PRESET_SHOT { ACTION_KEYS_SHOT_KEYBOARD, ACTION_KEYS_SHOT_GAMEPAD }
#pragma endregion

#pragma region PRESET Slow Motion																						//Slow Motion
#define ACTION_KEYS_SLOW_MOTION_GAMEPAD		FKey(EKeys::Gamepad_LeftShoulder)											//Gamepad : Left Shoulder 
#define ACTION_KEYS_SLOW_MOTION_KEYBOARD	FKey(EKeys::RightMouseButton)												//Keyboard : Mouse Right Button

#define ACTION_PRESET_SLOW_MOTION { ACTION_KEYS_SLOW_MOTION_KEYBOARD, ACTION_KEYS_SLOW_MOTION_GAMEPAD }
#pragma endregion

#pragma region PRESET Dash																								//Dash
#define ACTION_KEYS_DASH_GAMEPAD	FKey(EKeys::Gamepad_LeftTrigger)													//Gamepad : Left Trigger
#define ACTION_KEYS_DASH_KEYBOARD	FKey(EKeys::LeftShift)																//Keyboard : Left Shift

#define ACTION_PRESET_DASH { ACTION_KEYS_DASH_KEYBOARD, ACTION_KEYS_DASH_GAMEPAD }
#pragma endregion

#pragma region PRESET Interaction																						//Interaction
#define ACTION_KEYS_INTERACTION_GAMEPAD		FKey(EKeys::Gamepad_FaceButton_Top)											//Gamepad : Y
#define ACTION_KEYS_INTERACTION_KEYBOARD	FKey(EKeys::F)																//Keyboard : F

#define ACTION_PRESET_INTERACTION { ACTION_KEYS_INTERACTION_KEYBOARD, ACTION_KEYS_INTERACTION_GAMEPAD }
#pragma endregion

#pragma region PRESET Jump																								//Jump
#define ACTION_KEYS_JUMP_GAMEPAD	FKey(EKeys::Gamepad_FaceButton_Bottom)												//Gamepad : A
#define ACTION_KEYS_JUMP_KEYBOARD	FKey(EKeys::SpaceBar)																//Keyboard : Space Bar

#define ACTION_PRESET_JUMP { ACTION_KEYS_JUMP_KEYBOARD, ACTION_KEYS_JUMP_GAMEPAD }
#pragma endregion

#pragma region PRESET Pause																								//Pause
#define ACTION_KEYS_PAUSE_GAMEPAD	FKey(EKeys::Gamepad_Special_Right)													//Gamepad : Special Right
#define ACTION_KEYS_PAUSE_KEYBOARD	FKey(EKeys::Escape), FKey(EKeys::P)													//Keyboard : Escape

#define ACTION_PRESET_PAUSE { ACTION_KEYS_PAUSE_KEYBOARD, ACTION_KEYS_PAUSE_GAMEPAD }
#pragma endregion

#pragma region PRESET Mash																								//Mash
#define ACTION_KEYS_MASH_GAMEPAD			FKey(EKeys::Gamepad_LeftTrigger), FKey(EKeys::Gamepad_RightTrigger)			//Gamepad : Left Trigger / Right Trigger
#define ACTION_KEYS_MASH_KEYBOARD			FKey(EKeys::LeftMouseButton), FKey(EKeys::RightMouseButton)					//Keyboard : Left Mouse Button / Right Mouse Button

#define ACTION_PRESET_MASH { ACTION_KEYS_MASH_KEYBOARD, ACTION_KEYS_MASH_GAMEPAD }
#pragma endregion

#pragma region PRESET QTE																								//QTE
#define ACTION_KEYS_QTE_GAMEPAD			FKey(EKeys::Gamepad_FaceButton_Bottom), \
										FKey(EKeys::Gamepad_FaceButton_Left), \
										FKey(EKeys::Gamepad_FaceButton_Top), \
										FKey(EKeys::Gamepad_FaceButton_Right)											//Gamepad : A X Y B

#define ACTION_KEYS_QTE_KEYBOARD_AZERTY FKey(EKeys::Z), FKey(EKeys::Q), FKey(EKeys::S), FKey(EKeys::D)					//Keyboard AZERTY : Z Q S D
#define ACTION_KEYS_QTE_KEYBOARD_QWERTY FKey(EKeys::W), FKey(EKeys::A), FKey(EKeys::S), FKey(EKeys::D)					//Keyboard QWERTY : W A S D

#define ACTION_PRESET_QTE_GAMEPAD			{ ACTION_KEYS_QTE_GAMEPAD }
#define ACTION_PRESET_QTE_KEYBOARD_AZERTY	{ ACTION_KEYS_QTE_KEYBOARD_AZERTY } 
#define ACTION_PRESET_QTE_KEYBOARD_QWERTY	{ ACTION_KEYS_QTE_KEYBOARD_QWERTY }

#define ACTION_PRESET_QTE_KEYBOARD(IsQwerty) IsQwerty ? TArray<FInputChord>() = ACTION_PRESET_QTE_KEYBOARD_QWERTY : TArray<FInputChord>() = ACTION_PRESET_QTE_KEYBOARD_AZERTY
#pragma endregion

#pragma region UI Back																									//UI Back
#define ACTION_KEYS_UI_BACK_GAMEPAD		FKey(EKeys::Gamepad_FaceButton_Right)											//Gamepad : B
#define ACTION_KEYS_UI_BACK_KEYBOARD	FKey(EKeys::Escape)																//Keyboard : Escape

#define ACTION_PRESET_UI_BACK { ACTION_KEYS_UI_BACK_KEYBOARD, ACTION_KEYS_UI_BACK_GAMEPAD }
#pragma endregion

#pragma region PRESET UI Left																							//UI Left
#define ACTION_KEYS_UI_LEFT_GAMEPAD			FKey(EKeys::Gamepad_LeftShoulder)											//Gamepad : Left Shoulder
#define ACTION_KEYS_UI_LEFT_KEYBOARD_AZERTY FKey(EKeys::A)																//Keyboard AZERTY : A
#define ACTION_KEYS_UI_LEFT_KEYBOARD_QWERTY FKey(EKeys::Q)																//Keyboard QWERTY : Q

#define ACTION_PRESET_UI_LEFT_AZERTY { ACTION_KEYS_UI_LEFT_KEYBOARD_AZERTY, ACTION_KEYS_UI_LEFT_GAMEPAD }
#define ACTION_PRESET_UI_LEFT_QWERTY { ACTION_KEYS_UI_LEFT_KEYBOARD_QWERTY, ACTION_KEYS_UI_LEFT_GAMEPAD }

#define ACTION_PRESET_UI_LEFT(IsQwerty) IsQwerty ? TArray<FInputChord>() = ACTION_PRESET_UI_LEFT_QWERTY : TArray<FInputChord>() = ACTION_PRESET_UI_LEFT_AZERTY
#pragma endregion

#pragma region PRESET UI Right																							//UI Right
#define ACTION_KEYS_UI_RIGHT_GAMEPAD		 FKey(EKeys::Gamepad_RightShoulder)											//Gamepad : Right Shoulder
#define ACTION_KEYS_UI_RIGHT_KEYBOARD_AZERTY FKey(EKeys::E)																//Keyboard AZERTY : E
#define ACTION_KEYS_UI_RIGHT_KEYBOARD_QWERTY FKey(EKeys::E)																//Keyboard QWERTY : E

#define ACTION_PRESET_UI_RIGHT_AZERTY { ACTION_KEYS_UI_RIGHT_KEYBOARD_AZERTY, ACTION_KEYS_UI_RIGHT_GAMEPAD }
#define ACTION_PRESET_UI_RIGHT_QWERTY { ACTION_KEYS_UI_RIGHT_KEYBOARD_QWERTY, ACTION_KEYS_UI_RIGHT_GAMEPAD }

#define ACTION_PRESET_UI_RIGHT(IsQwerty) IsQwerty ? TArray<FInputChord>() = ACTION_PRESET_UI_RIGHT_QWERTY : TArray<FInputChord>() = ACTION_PRESET_UI_RIGHT_AZERTY
#pragma endregion

#pragma region UI Reset Input																							//Reset Input
#define ACTION_KEYS_UI_RESET_INPUT_GAMEPAD		FKey(EKeys::Gamepad_FaceButton_Top)										//Gamepad : Y

#define ACTION_PRESET_UI_RESET_INPUT { ACTION_KEYS_UI_RESET_INPUT_GAMEPAD }
#pragma endregion

#pragma region Menu Save Next																							//Menu Save Next
#define ACTION_KEYS_MENU_SAVE_NEXT_GAMEPAD			FKey(EKeys::Gamepad_LeftStick_Right), \
													FKey(EKeys::Gamepad_DPad_Right)										//Gamepad : Left Thumbstick Right
#define ACTION_KEYS_MENU_SAVE_NEXT_KEYBOARD_AZERTY	FKey(EKeys::E)														//Keyboard AZERTY : E
#define ACTION_KEYS_MENU_SAVE_NEXT_KEYBOARD_QWERTY	FKey(EKeys::E)														//Keyboard QWERTY : E

#define ACTION_PRESET_MENU_SAVE_NEXT_AZERTY { ACTION_KEYS_MENU_SAVE_NEXT_KEYBOARD_AZERTY, ACTION_KEYS_MENU_SAVE_NEXT_GAMEPAD }
#define ACTION_PRESET_MENU_SAVE_NEXT_QWERTY { ACTION_KEYS_MENU_SAVE_NEXT_KEYBOARD_QWERTY, ACTION_KEYS_MENU_SAVE_NEXT_GAMEPAD }

#define ACTION_PRESET_MENU_SAVE_NEXT(IsQwerty) IsQwerty ? TArray<FInputChord>() = ACTION_PRESET_MENU_SAVE_NEXT_QWERTY : TArray<FInputChord>() = ACTION_PRESET_MENU_SAVE_NEXT_AZERTY
#pragma endregion

#pragma region Menu Save Previous																						//Menu Save Previous
#define ACTION_KEYS_MENU_SAVE_PREVIOUS_GAMEPAD			FKey(EKeys::Gamepad_LeftStick_Left), \
														FKey(EKeys::Gamepad_DPad_Left)									//Gamepad :	Left Thumbstick Left 
#define ACTION_KEYS_MENU_SAVE_PREVIOUS_KEYBOARD_AZERTY	FKey(EKeys::A)													//Keyboard AZERTY : A
#define ACTION_KEYS_MENU_SAVE_PREVIOUS_KEYBOARD_QWERTY	FKey(EKeys::Q)													//Keyboard QWERTY : Q

#define ACTION_PRESET_MENU_SAVE_PREVIOUS_AZERTY { ACTION_KEYS_MENU_SAVE_PREVIOUS_KEYBOARD_AZERTY, ACTION_KEYS_MENU_SAVE_PREVIOUS_GAMEPAD }
#define ACTION_PRESET_MENU_SAVE_PREVIOUS_QWERTY { ACTION_KEYS_MENU_SAVE_PREVIOUS_KEYBOARD_QWERTY, ACTION_KEYS_MENU_SAVE_PREVIOUS_GAMEPAD }

#define ACTION_PRESET_MENU_SAVE_PREVIOUS(IsQwerty) IsQwerty ? TArray<FInputChord>() = ACTION_PRESET_MENU_SAVE_PREVIOUS_QWERTY : TArray<FInputChord>() = ACTION_PRESET_MENU_SAVE_PREVIOUS_AZERTY
#pragma endregion

#pragma region Menu Save Select																							//Menu Save Select
#define ACTION_KEYS_MENU_SAVE_SELECT_GAMEPAD	FKey(EKeys::Gamepad_FaceButton_Bottom)									//Gamepad : A
#define ACTION_KEYS_MENU_SAVE_SELECT_KEYBOARD	FKey(EKeys::Enter), FKey(EKeys::LeftMouseButton)						//Keyboard : Enter / Left Mouse Button

#define ACTION_PRESET_MENU_SAVE_SELECT { ACTION_KEYS_MENU_SAVE_SELECT_KEYBOARD, ACTION_KEYS_MENU_SAVE_SELECT_GAMEPAD }
#pragma endregion

#pragma region Menu Save Delete																							//Menu Save Delete
#define ACTION_KEYS_MENU_SAVE_DELETE_GAMEPAD	FKey(EKeys::Gamepad_FaceButton_Left)									//Gamepad : X
#define ACTION_KEYS_MENU_SAVE_DELETE_KEYBOARD	FKey(EKeys::Delete)														//Keyboard : Delete

#define ACTION_PRESET_MENU_SAVE_DELETE { ACTION_KEYS_MENU_SAVE_DELETE_KEYBOARD, ACTION_KEYS_MENU_SAVE_DELETE_GAMEPAD }
#pragma endregion 

#pragma region AnyKey
#define ACTION_KEYS_ANY_KEY FKey(EKeys::AnyKey)
#define ACTION_PRESET_ANY_KEY { ACTION_KEYS_ANY_KEY } 
#pragma endregion

#pragma endregion

#pragma region Axis Preset

#pragma region Vertical Movement																						//Vertical Movement
#define AXIS_KEYS_VERTICAL_MOVEMENT_GAMEPAD { FKey(EKeys::Gamepad_LeftY), 1 }											//Gamepad : Left Thumbstick Y-Axis
#define AXIS_KEYS_VERTICAL_MOVEMENT_KEYBOARD_AZERTY { FKey(EKeys::Z), 1 }, { FKey(EKeys::S), -1 }						//Keyboard AZERTY : Z (1) S (-1)
#define AXIS_KEYS_VERTICAL_MOVEMENT_KEYBOARD_QWERTY { FKey(EKeys::W), 1 }, { FKey(EKeys::S), -1 }						//Keyboard QWERTY : W (1) S (-1)

#define AXIS_PRESET_VERTICAL_MOVEMENT_AZERTY { AXIS_KEYS_VERTICAL_MOVEMENT_KEYBOARD_AZERTY, AXIS_KEYS_VERTICAL_MOVEMENT_GAMEPAD }
#define AXIS_PRESET_VERTICAL_MOVEMENT_QWERTY { AXIS_KEYS_VERTICAL_MOVEMENT_KEYBOARD_QWERTY, AXIS_KEYS_VERTICAL_MOVEMENT_GAMEPAD }

#define AXIS_PRESET_VERTICAL_MOVEMENT(IsQwerty) IsQwerty ? TArray<TTuple<FKey, float>>() = AXIS_PRESET_VERTICAL_MOVEMENT_QWERTY : TArray<TTuple<FKey, float>>() = AXIS_PRESET_VERTICAL_MOVEMENT_AZERTY
#pragma endregion

#pragma region Horizontal Movement																						//Horizontal Movement
#define AXIS_KEYS_HORIZONTAL_MOVEMENT_GAMEPAD { FKey(EKeys::Gamepad_LeftX), 1 }											//Gamepad : Left Thumbstick X-Axis
#define AXIS_KEYS_HORIZONTAL_MOVEMENT_KEYBOARD_AZERTY { FKey(EKeys::Q), -1 }, { FKey(EKeys::D), 1 }						//Keyboard AZERTY : Q (-1) D (1)
#define AXIS_KEYS_HORIZONTAL_MOVEMENT_KEYBOARD_QWERTY { FKey(EKeys::A), -1 }, { FKey(EKeys::D), 1 }						//Keyboard QWERTY : W (-1) D (1)

#define AXIS_PRESET_HORIZONTAL_MOVEMENT_AZERTY { AXIS_KEYS_HORIZONTAL_MOVEMENT_KEYBOARD_AZERTY, AXIS_KEYS_HORIZONTAL_MOVEMENT_GAMEPAD }
#define AXIS_PRESET_HORIZONTAL_MOVEMENT_QWERTY { AXIS_KEYS_HORIZONTAL_MOVEMENT_KEYBOARD_QWERTY, AXIS_KEYS_HORIZONTAL_MOVEMENT_GAMEPAD }

#define AXIS_PRESET_HORIZONTAL_MOVEMENT(IsQwerty) IsQwerty ? TArray<TTuple<FKey, float>>() = AXIS_PRESET_HORIZONTAL_MOVEMENT_QWERTY : TArray<TTuple<FKey, float>>() = AXIS_PRESET_HORIZONTAL_MOVEMENT_AZERTY
#pragma endregion

#pragma region Camera Pitch Keyboard																					//Camera Pitch Keyboard
#define AXIS_KEYS_CAMERA_PITCH_KEYBOARD { FKey(EKeys::MouseY), 1 }														//Keyboard : Mouse Y

#define AXIS_PRESET_CAMERA_PITCH_KEYBOARD { AXIS_KEYS_CAMERA_PITCH_KEYBOARD }
#pragma endregion

#pragma region Camera Yaw Keyboard																						//Camera Yaw Keyboard
#define AXIS_KEYS_CAMERA_YAW_KEYBOARD { FKey(EKeys::MouseX), 1 }														//Keyboard : Mouse X

#define AXIS_PRESET_CAMERA_YAW_KEYBOARD { AXIS_KEYS_CAMERA_YAW_KEYBOARD }
#pragma endregion

#pragma region Camera Pitch Gamepad																						//Camera Pitch Gamepad
#define AXIS_KEYS_CAMERA_PITCH_GAMEPAD { FKey(EKeys::Gamepad_RightY), -1 }												//Gamepad : Right Thumbstick Y-Axis

#define AXIS_PRESET_CAMERA_PITCH_GAMEPAD { AXIS_KEYS_CAMERA_PITCH_GAMEPAD }
#pragma endregion

#pragma region Camera Yaw Gamepad																						//Camera Yaw Gamepad
#define AXIS_KEYS_CAMERA_YAW_GAMEPAD { FKey(EKeys::Gamepad_RightX), 1 }													//Gamepad : Right Thumbstick X-Axis

#define AXIS_PRESET_CAMERA_YAW_GAMEPAD { AXIS_KEYS_CAMERA_YAW_GAMEPAD }
#pragma endregion

#pragma region Elevator																									//Elevator
#define AXIS_KEYS_ELEVATOR_GAMEPAD { FKey(EKeys::Gamepad_RightX), 1 }													//Gamepad : Right Thumbstick X-Axis
#define AXIS_KEYS_ELEVATOR_KEYBOARD_AZERTY { FKey(EKeys::Q), -1 }, { FKey(EKeys::D), 1 }								//Keyboard AZERTY : Q (-1) D (1)
#define AXIS_KEYS_ELEVATOR_KEYBOARD_QWERTY { FKey(EKeys::A), -1 }, { FKey(EKeys::D), 1 }								//Keyboard QWERTY : A (-1) D (1) 

#define AXIS_PRESET_ELEVATOR_AZERTY { AXIS_KEYS_ELEVATOR_KEYBOARD_AZERTY, AXIS_KEYS_CAMERA_YAW_GAMEPAD }
#define AXIS_PRESET_ELEVATOR_QWERTY { AXIS_KEYS_ELEVATOR_KEYBOARD_QWERTY, AXIS_KEYS_CAMERA_YAW_GAMEPAD }

#define AXIS_PRESET_ELEVATOR(IsQwerty) IsQwerty ? TArray<TTuple<FKey, float>>() = AXIS_PRESET_ELEVATOR_QWERTY : TArray<TTuple<FKey, float>>() = AXIS_PRESET_ELEVATOR_AZERTY
#pragma endregion

#pragma endregion



#pragma region Cheat Code

#define ACTION_PRESET_CHEAT_GOD_MODE		{ FInputChord(FKey(EKeys::NumPadZero),	false, true, false, false) }		//God Mode : Ctrl + 0

#define ACTION_PRESET_CHEAT_TP_ARENA_ONE	{ FInputChord(FKey(EKeys::NumPadOne),	false, true, false, false) }		//TP Arena One   : Ctrl + 1
#define ACTION_PRESET_CHEAT_TP_ARENA_TWO	{ FInputChord(FKey(EKeys::NumPadTwo),	false, true, false, false) }		//TP Arena Two   : Ctrl + 2
#define ACTION_PRESET_CHEAT_TP_ARENA_THREE	{ FInputChord(FKey(EKeys::NumPadThree), false, true, false, false) }		//TP Arena Three : Ctrl + 3

#define ACTION_PRESET_CHEAT_BOSS_START		{ FInputChord(FKey(EKeys::B), false, true, false, false) }					//Start Boss : Ctrl + B
#define ACTION_PRESET_CHEAT_BOSS_KILL		{ FInputChord(FKey(EKeys::K), false, true, false, false) }					//Kill Boss  : Ctrl + K

#define ACTION_PRESET_CHEAT_SKIP_TUTORIAL	{ FInputChord(FKey(EKeys::Tab), false, false, false, false) }				//Skip Tutorial : Ctrl + T

#pragma endregion