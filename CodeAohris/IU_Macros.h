#pragma once

/*Clamp angle between 0 and 360 degrees. If angle > 360 -> angle - 360 else angle < 0 -> angle + 360.*/
#define Get_ClampedAngle(_currentAngle) _currentAngle < 0 ? _currentAngle + 360 : _currentAngle > 360 ? _currentAngle - 360 : _currentAngle

/*Default world speed = 1.*/
#define MIN_WORLD_SPEED 0.0f

/*Default world speed = 1.*/
#define MAX_WORLD_SPEED 1.0f

//Boss reset cooldown = 0.5f
#define BOSS_RESET_COOLDOWN 0.1f

//biggi boy = 694201.0f
#define BIG_FLOAT 694201.0f

/*Two.*/
#define LES_DEUX 2

/*Le jus (51).*/
#define PASTIS 51

/*Get angle in degree between two FVector.*/
#define Get_Angle(_from, _to) FMath::RadiansToDegrees(acosf(FVector::DotProduct(_from, _to)))

/*Divide a 360 angle by _divider. Avoid _divider == 0 (will crash).*/
#define Get_RadRatio(_divider) FMath::DegreesToRadians(360 / _divider)

/*Optimized FindLookAtRotation().*/
#define Get_LookRotation(_forward) UKismetMathLibrary::MakeRotFromX(_forward)

/*Return a random float between min and max.*/
#define Get_RandFloat(_min, _max) FMath::FRandRange(_min, _max)

/*Sphere Trace single more cool.*/
#define Get_SphereTrace(_world, _start, _end, _radius, _layer, _hitResult) UKismetSystemLibrary::SphereTraceSingleForObjects(_world, _start, _end, _radius, _layer, false, TArray<AActor*>(), EDrawDebugTrace::None, _hitResult, true)

//Line Trace Single
#define Get_LineTrace(_world, _start, _end, _layer, _hitResult) UKismetSystemLibrary::LineTraceSingleForObjects(_world, _start, _end, _layer, false, TArray<AActor*>(), EDrawDebugTrace::None, _hitResult, true)

#define LOG_GREY(_msg, ...) UE_LOG(LogTemp, Log, TEXT(_msg), ##__VA_ARGS__)

/*⢀⡴⠑⡄⠀⠀⠀⠀⠀⠀⠀⣀⣀⣤⣤⣤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ 
⠸⡇⠀⠿⡀⠀⠀⠀⣀⡴⢿⣿⣿⣿⣿⣿⣿⣿⣷⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠑⢄⣠⠾⠁⣀⣄⡈⠙⣿⣿⣿⣿⣿⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⢀⡀⠁⠀⠀⠈⠙⠛⠂⠈⣿⣿⣿⣿⣿⠿⡿⢿⣆⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⢀⡾⣁⣀⠀⠴⠂⠙⣗⡀⠀⢻⣿⣿⠭⢤⣴⣦⣤⣹⠀⠀⠀⢀⢴⣶⣆ 
⠀⠀⢀⣾⣿⣿⣿⣷⣮⣽⣾⣿⣥⣴⣿⣿⡿⢂⠔⢚⡿⢿⣿⣦⣴⣾⠁⠸⣼⡿ 
⠀⢀⡞⠁⠙⠻⠿⠟⠉⠀⠛⢹⣿⣿⣿⣿⣿⣌⢤⣼⣿⣾⣿⡟⠉⠀⠀⠀⠀⠀ 
⠀⣾⣷⣶⠇⠀⠀⣤⣄⣀⡀⠈⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ 
⠀⠉⠈⠉⠀⠀⢦⡈⢻⣿⣿⣿⣶⣶⣶⣶⣤⣽⡹⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠉⠲⣽⡻⢿⣿⣿⣿⣿⣿⣿⣷⣜⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣷⣶⣮⣭⣽⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⣀⣀⣈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠻⠿⠿⠿⠿⠛⠉*/
#define LOG(_msg, ...) UE_LOG(LogTemp, Warning, TEXT(_msg), ##__VA_ARGS__)

/*⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⣼⡟⡀⠀⠀⠀⠀⠁⡴⠋⠉⠉⠙⡧⡄⣆⠀⠀⠀⠀⠀⣠⣯⡇⠀⢀⣴⡶⣿⢍⣄⢄⡀⠀⠀⠀⣶⡆⠀⠀⢀⣤⡖⡀⠀⠀⠀⠀
⠀⠈⣿⡿⠿⣿⣷⣆⠀⣟⣏⣀⡀⣀⣠⣇⣷⢻⡆⠀⠀⠀⡼⣿⠋⣀⣦⡋⠯⠉⠀⠁⠃⠷⣽⠀⠀⠀⣿⡇⡠⡿⠉⠁⠀⠀⠀⠀⠀⠀
⠀⠐⣿⠅⠀⠈⠻⣿⡄⣯⡟⠛⠛⠛⠛⠛⠋⠉⣷⠀⠀⡟⡽⠁⢀⢕⡷⣷⣶⣶⣶⣶⡶⠛⠂⠀⠀⠀⣿⡏⠋⠀⠀⠀⠀⠀⠀⠀
⠀⠀⣿⡇⠀⠀⠀⣿⡇⠉⠳⣤⣀⣀⣀⣄⡀⠀⠽⡇⣿⠏⠀⠀⠘⣿⡇⠀⠀⠀⠉⠀⠀⠀⠀⠀⠀⠀⡿⡇⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠉⠀⠀⠀⠀⠱⠇⠀⠀⠈⠙⠛⠛⠉⠁⠀⠀⠿⠃⠀⠀⠀⠀⠙⠛⣿⣓⣶⣦⣤⣤⡄⠀⠀⠀⠀⣷⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⣤⣤⣄⠀⠀⠀⠀⣀⠤⡤⡄⡀⠀⠂⠀⣶⣆⣀⡀⠀⠀⠀⠉⠉⠛⠓⠛⠃⠀⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⣸⠏⡏⠛⢝⣧⠀⠀⣿⠏⠉⠑⠫⣝⡇⠀⠀⡿⡟⠻⠟⣟⣧⡀⠠⣾⣿⠷⢶⣦⣄⠀⠀⢠⡾⠯⣿⣆⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⣿⢆⡃⠀⡠⡟⡄⠀⡧⠃⠀⠀⠀⠠⡇⠀⠀⣿⡇⠀⠀⠩⡏⡇⠀⣿⣿⠀⠀⠉⢻⣷⡄⢿⡇⠀⠸⣿⡀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠣⣕⢟⣛⠟⣧⡇⠀⣻⣏⣆⣢⣔⠋⠁⠀⠀⠛⠿⠀⠀⠀⠃⠃⠀⢻⡿⠀⠀⠀⠈⣿⢿⠈⠛⠿⠟⢹⣷⡀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⣀⡀⠀⠉⣁⡮⡿⠁⠀⠈⠉⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠁⠀⠀⠀⠀⠉⠀⠀⠀⠀⠀⠀⠈⠃⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠑⠭⠭⠿⠛⠓⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠢⡄⠀⡆⢀⠀⠀⠀⠀⡄⠀⠀⢀⡠⠄⠄⠄⡀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⠟⠛⠛⢿⡆⠀⠀⠀⠯⠋⠀⠀⠩⣿⠀⠀⢀⢾⠇⠁⣀⣥⠶⠓⠻⢯⡲⡌⡱⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⡁⠀⠀⠀⡸⣿⠀⠀⠀⣄⠀⠀⠀⠈⣮⠢⠀⣮⠏⠀⢨⣯⣧⣆⣒⣨⣡⣯⠏⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠈⠙⠦⣤⡤⠟⢝⡄⠀⠀⠧⠅⠀⠀⠀⣟⡄⡟⠅⠀⠀⠀⡏⣇⠈⠙⠓⠓⠁⠀⢀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⢟⡅⠀⠀⠘⡇⠀⠀⠀⠈⠿⠃⠀⠀⠀⠀⠉⠻⣿⣦⣤⣤⣔⣯⡍⠁⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⡶⣄⢀⠀⠀⢀⡟⡑⠁⠀⠀⠀⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠋⠋⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠋⠭⠬⠋⠋⠉⠀⠀⠀⠀⠀⠀⠀⢀⡀⡀⡀⠀⠀⠀⠀⠀⢀⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠲⡦⡀⠀⠀⢀⡷⠋⣴⠃⠁⠁⡇⠻⣄⠀⠀⠀⣸⢸⠹⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⡦⣰⠏⠁⠀⢿⣀⢀⣴⠇⠀⠈⢯⣆⡦⣯⡋⠞⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣽⠋⠀⠀⠀⠑⠛⠋⠁⠀⠀⠀⠀⠀⠈⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣤⢯⠁⠀⠀⠀⢠⡆⠀⠀⠀⡄⠀⠀⠀⣾⡿⠿⠧⢿⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠀⠀⠀⠀⠀⣿⡏⠀⠀⠀⣿⡆⠀⠀⣿⣷⠁⡰⠿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠯⣿⣆⡀⣖⡟⠀⠀⠀⢹⣿⡿⠇⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠉⠀⠀⠀⠀⠸⢧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀*/
#define LOG_ERROR(_msg, ...) UE_LOG(LogTemp, Error, TEXT(_msg), ##__VA_ARGS__)

//Auto return if condition is true.
#define ROG(_value, _msg, ...) if (_value) \
{ \
	LOG(_msg, ##__VA_ARGS__); \
	return; \
}

//Auto return if condition is true.
#define ROG_ERROR(_value, _msg, ...) if (_value) \
{ \
	LOG_ERROR(_msg, ##__VA_ARGS__); \
	return; \
}

//Auto return if condition is true.
#define ROG_NULLPTR(_value, _msg, ...) if (_value) \
{ \
	LOG(_msg, ##__VA_ARGS__); \
	return nullptr; \
}

//Auto return nullptr if condition is true.
#define ROG_ERROR_NULLPTR(_value, _msg, ...) if (_value) \
{ \
	LOG_ERROR(_msg, ##__VA_ARGS__); \
	return nullptr; \
}

/*Shortcut for unsigned int.*/
#define uint unsigned int

/*Convert degree to radiant.*/
#define To_Rad(_degree) FMath::DegreesToRadians(_degree)

//Convert radiant to degree
#define To_Deg(_rad) FMath::RadiansToDegrees(_rad)

/*Lerp FVector constantly.*/
#define Get_VLerpConst(_current, _target, _deltaTime, _speed) FMath::VInterpConstantTo(_current, _target, _deltaTime, _speed)

/*Lerp float constantly.*/
#define Get_FLerpConst(_current, _target, _deltaTime, _speed) FMath::FInterpConstantTo(_current, _target, _deltaTime, _speed)

/*Lerp float constantly.*/
#define Get_RLerpConst(_current, _target, _deltaTime, _speed) FMath::RInterpConstantTo(_current, _target, _deltaTime, _speed)

/*The _direction is the forward used and a random vector with _halfAngleInRad will be add to this.*/
#define Get_RandomVectorInCone(_direction, _halfAngleInRad) FMath::VRandCone(_direction, _halfAngleInRad)

/*Give the middle FVector between _first and _second.*/
#define Get_MidVector(_first, _second) (_first + _second) * 0.5f

// Spawn a niagara system with default values
#define SpawnNiagaraSystem(_niagaraSystem) UNiagaraFunctionLibrary::SpawnSystemAttached(_niagaraSystem, RootComponent, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true) 
#define SpawnNiagaraSystemOnSceneComponent(_niagaraSystem, _sceneComponent) UNiagaraFunctionLibrary::SpawnSystemAttached(_niagaraSystem, _sceneComponent, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true)
#define SpawnNiagaraSystemOnSceneComponentNoDestroy(_niagaraSystem, _sceneComponent) UNiagaraFunctionLibrary::SpawnSystemAttached(_niagaraSystem, _sceneComponent, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, false)

// Add event only on this
#define Event_Add(_event, _func) _event.AddDynamic(this, &_func)

// Add unique event only on this
#define Event_AddUnique(_event, _func) _event.AddUniqueDynamic(this, &_func)

// Remove event only on this
#define Event_Remove(_event, _func) _event.RemoveDynamic(this, &_func)


#define ON_SCREEN_LOG(_msg) GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.0f, FColor::Red, _msg);

#define ON_SCREEN_LOG_TIME(_time, _color,  _msg) GEngine->AddOnScreenDebugMessage(INDEX_NONE, _time, _color, _msg);

#define ON_SCREEN_LOG_TIME_(time, text, ...) \
GEngine->AddOnScreenDebugMessage(-1, time, FColor::Yellow, FString::Printf(TEXT(text), __VA_ARGS__));

#define TIMED_DELEGATE(_timerManager, _functionName, _delay, ...) \
	{ \
		FTimerHandle _handle; \
		FTimerDelegate _delegate; \
		_delegate.BindUFunction(this, _functionName, ##__VA_ARGS__); \
		_timerManager.SetTimer(_handle, _delegate, _delay, false); \
	} \

//_mine, _otherActor, _otherComponent
#define COMPONENT_START_COLLISION_PARAMS UPrimitiveComponent* _mine, AActor* _otherActor, UPrimitiveComponent* _otherComponent, int32 _otherBodyIndex, bool _fromSweep, const FHitResult& _sweepResult
//_mine, _otherActor, _otherComponent
#define COMPONENT_STOP_COLLISION_PARAMS UPrimitiveComponent* _mine, AActor* _otherActor, UPrimitiveComponent* _otherComponent, int32 _otherBodyIndex

// Macro used to enable or not LOG()s
#define WITH_LOG_ANIM_BOSS 0
#define WITH_LOG_MOVEMENT_BOSS 0
#define WITH_LOG_PHASE_BOSS 0
#define WITH_LOG_STUN_BOSS 0
#define WITH_LOG_ANIM_FEEDBACK 0
#define WITH_LOG_PB_SPAWNER 0
#define WITH_LOG_OVERRIDE_FEEDBACK 0
#define WITH_LOG_DEBUG 0

#define DEBUG_BOSS 0

#define DEBUG_BB 0