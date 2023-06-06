#pragma once
#include "ENUM_PatternDataTableType.h"
#include "STRUCT_AngleSettings.h"
#include "STRUCT_ArenaCloserSettings.h"
#include "STRUCT_TripleDeluxeFeedBack.h"
#include "STRUCT_GPE_FloorDangerSettings.h"
#include "STRUCT_GPE_HuntRiftSettings.h"
#include "STRUCT_GPE_ProjectileSettings.h"
#include "STRUCT_InstantSettings.h"
#include "STRUCT_LaserSettings.h"
#include "STRUCT_MovementOnAxeSettings.h"
#include "STRUCT_MovementTeleportSettings.h"
#include "STRUCT_MovementToTargetSettings.h"
#include "STRUCT_RotationSettings.h"
#include "STRUCT_RotatorSettings.h"
#include "STRUCT_ScalingSettings.h"
#include "STRUCT_ShootArcSettings.h"
#include "STRUCT_ShootCircleSettings.h"
#include "STRUCT_ShootCrossSettings.h"
#include "STRUCT_ShootLineSettings.h"
#include "STRUCT_ShootPointSettings.h"
#include "STRUCT_ShootRandom2DSettings.h"
#include "STRUCT_ShootRandom3DSettings.h"
#include "STRUCT_ShootSettings.h"
#include "STRUCT_SpawnerConcentricSettings.h"
#include "STRUCT_SpawnerDefaultSettings.h"
#include "STRUCT_SpawnerMultiSettings.h"
#include "STRUCT_SpawnerSettings.h"
#include "STRUCT_TargetSettings.h"
#include "STRUCT_WaveSettings.h"
#include "STRUCT_PatternData.generated.h"

class UA_DT_ArenaDataTable;
class AP_PatternBase;

//Store every pattern data in struct
USTRUCT()
struct FPatternData
{
	GENERATED_BODY()

#pragma region Base
private:
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Base")
	TSubclassOf<AP_PatternBase> patternType;
	UPROPERTY(EditAnywhere,Category="Pattern Data | Base")
	FString patternName = "Pattern";
	UPROPERTY(EditAnywhere,Category="Pattern Data | Base")
	TEnumAsByte<EPatternDataTableType> patternDataTableType = EPatternDataTableType::PATTERN_NONE;
	
	//FDD
	UPROPERTY(EditAnywhere,Category="Pattern Data | Base")
		float fDelay = 0.0f;
	//Range between 0 and this added to 'fDelay'
	UPROPERTY(EditAnywhere,Category="Pattern Data | Base")
		float fRangeRandom = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Pattern Data | Base")
	bool bDuplicateFX = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Base")
	int duplication = 0;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Base")
	bool centerDuplication = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Base")
	FVector duplicationOffset = FVector::ZeroVector;

public:
	FORCEINLINE const TSubclassOf<AP_PatternBase>& GetPatternType() const { return patternType; }
	FORCEINLINE const FString& GetPatternName() const { return patternName; }
	FORCEINLINE const TEnumAsByte<EPatternDataTableType>& GetPatternDataTableType() const { return patternDataTableType; }
	FORCEINLINE float GetDelay() const { return fDelay; }
	FORCEINLINE float GetRangeRandom() const { return fRangeRandom; }
	FORCEINLINE int	GetDuplication() const { return duplication; }
	FORCEINLINE bool GetCenterDuplication() const { return centerDuplication; }
	FORCEINLINE const FVector& GetDuplicationOffset() const { return duplicationOffset; }
	
	FORCEINLINE void SetPatternType(const TSubclassOf<AP_PatternBase>& _patternType) { patternType = _patternType; }
	FORCEINLINE void SetPatternName(const FString& _name) { patternName = _name; }
	FORCEINLINE void SetPatternDataTableType(const EPatternDataTableType& _type) { patternDataTableType = _type; }
	FORCEINLINE void SetDelay(const float& _fDelay) { fDelay = _fDelay; }
	FORCEINLINE void SetRangeRandom(const float& _fRangeRandom) { fRangeRandom = _fRangeRandom; }
	FORCEINLINE void SetDuplicationFX(const bool& _duplicateFX) { duplication = _duplicateFX; }
	FORCEINLINE void SetDuplication(const int _duplication) { duplication = _duplication; }
	FORCEINLINE void SetCenterDuplication(const bool _centerDuplication) { centerDuplication = _centerDuplication; }
	FORCEINLINE void SetDuplicationOffset(const FVector _duplicationOffset) { duplicationOffset = _duplicationOffset; }
#pragma endregion Base
	
#pragma region Rotation
private:
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Rotation")
	FRotationSettings rotationSettings;

	UPROPERTY(EditAnywhere, Category = "Pattern Data | Rotation")
	bool bRotator = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Rotation")
	FRotatorSettings rotatorSettings;

	UPROPERTY(EditAnywhere, Category = "Pattern Data | Rotation")
	bool bInstant = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Rotation")
	FInstantSettings instantSettings;

	UPROPERTY(EditAnywhere, Category = "Pattern Data | Rotation")
	bool bAngle = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Rotation")
	FAngleSettings angleSettings;

	UPROPERTY(EditAnywhere, Category = "Pattern Data | Rotation")
	bool bTarget = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Rotation")
	FTargetSettings targetSettings;
	
public:
	FORCEINLINE const bool& GetRotatorState() const { return bRotator; }
	FORCEINLINE const bool& GetInstantState() const { return bInstant; }
	FORCEINLINE const bool& GetAngleState() const { return bAngle; }
	FORCEINLINE const bool& GetTargetState() const { return bTarget; }
	
	FORCEINLINE const FRotationSettings& GetRotationSettings() const { return rotationSettings; }
	FORCEINLINE const FRotatorSettings& GetRotatorSettings() const { return rotatorSettings; }
	FORCEINLINE const FInstantSettings& GetInstantSettings() const { return instantSettings; }
	FORCEINLINE const FAngleSettings& GetAngleSettings() const { return angleSettings; }
	FORCEINLINE const FTargetSettings& GetTargetSettings() const { return targetSettings; }

	FORCEINLINE void SetRotatorState(const bool& _state) { bRotator = _state; }
	FORCEINLINE void SetInstantState(const bool& _state) { bInstant = _state; }
	FORCEINLINE void SetAngleState(const bool& _state) { bAngle = _state; }
	FORCEINLINE void SetTargetState(const bool& _state) { bTarget = _state; }
	
	FORCEINLINE void SetRotationSettings(const FRotationSettings& _rotationSettings) { rotationSettings = _rotationSettings; }
	FORCEINLINE void SetRotatorSettings(const FRotatorSettings& _rotatorSettings) { rotatorSettings = _rotatorSettings; }
	FORCEINLINE void SetInstantSettings(const FInstantSettings& _instantSettings) { instantSettings = _instantSettings; }
	FORCEINLINE void SetAngleSettings(const FAngleSettings& _angleSettings) { angleSettings = _angleSettings; }
	FORCEINLINE void SetTargetSettings(const FTargetSettings& _targetSettings) { targetSettings = _targetSettings; }
#pragma endregion Rotation
	
#pragma region Movement
private:
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Movement")
	bool bEditMovementOnAxe = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Movement")
	FMovementOnAxeSettings movementOnAxeSetting;

	UPROPERTY(EditAnywhere, Category = "Pattern Data | Movement")
	bool bEditMovementToTarget = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Movement")
	FMovementToTargetSettings movementToTargetSettings;

	UPROPERTY(EditAnywhere, Category = "Pattern Data | Movement")
	bool bEditMovementTeleport = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Movement")
	FMovementTeleportSettings movementTeleportSettings;
	
public:
	FORCEINLINE const bool& GetMovementOnAxeState() const { return bEditMovementOnAxe; }
	FORCEINLINE const bool& GetMovementToTargetState() const { return bEditMovementToTarget; }
	FORCEINLINE const bool& GetMovementTeleportState() const { return bEditMovementTeleport; }
	
	FORCEINLINE const FMovementOnAxeSettings& GetMovementOnAxeSettings() const { return movementOnAxeSetting; }
	FORCEINLINE const FMovementToTargetSettings& GetMovementToTargetSettings() const { return movementToTargetSettings; }
	FORCEINLINE const FMovementTeleportSettings& GetMovementTeleportSettings() const { return movementTeleportSettings; }

	FORCEINLINE void SetMovementOnAxeState(const bool& _state) { bEditMovementOnAxe = _state; }
	FORCEINLINE void SetMovementToTargetState(const bool& _state) { bEditMovementToTarget = _state; }
	FORCEINLINE void SetMovementTeleportState(const bool& _state) { bEditMovementTeleport = _state; }
	
	FORCEINLINE void SetMovementOnAxeSettings(const FMovementOnAxeSettings& _movementOnAxeSettings) { movementOnAxeSetting = _movementOnAxeSettings; }
	FORCEINLINE void SetMovementToTargetSettings(const FMovementToTargetSettings& _movementToTargetSettings) { movementToTargetSettings = _movementToTargetSettings; }
	FORCEINLINE void SetMovementTeleportSettings(const FMovementTeleportSettings& _movementTeleportSettings) { movementTeleportSettings = _movementTeleportSettings; }
#pragma endregion Movement
	
#pragma region ArenaCloser
private:
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Arena Closer")
	FArenaCloserSettings arenaCloserSettings;
	
public:
	FORCEINLINE const FArenaCloserSettings& GetArenaCloserSettings() const { return arenaCloserSettings; }

	FORCEINLINE void SetArenaCloserSettings(const FArenaCloserSettings& _arenaCloserSettings) { arenaCloserSettings = _arenaCloserSettings; }
#pragma endregion ArenaCloser
	
#pragma region Laser
private:
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Laser")
	bool bEditLaser = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Laser")
	FLaserSettings laserSettings;
	
public:
	FORCEINLINE const bool& GetLaserState() const { return bEditLaser; }

	FORCEINLINE const FLaserSettings& GetLaserSettings() const { return laserSettings; }

	FORCEINLINE void SetLaserState(const bool& _state) { bEditLaser = _state; }
	
	FORCEINLINE void SetLaserSettings(const FLaserSettings& _laserSettings) { laserSettings = _laserSettings; }
#pragma endregion Laser
	
#pragma region Scaling
private:
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Scaling")
	bool bScaling = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Scaling")
	FScalingSettings scalingSettings;
	
public:
	FORCEINLINE const bool& GetScalingState() const { return bScaling; }
	
	FORCEINLINE const FScalingSettings& GetScalingSettings() const { return scalingSettings; }

	FORCEINLINE void SetScalingState(const bool& _state) { bScaling = _state; }
	
	FORCEINLINE void SetScalingSettings(const FScalingSettings& _scalingSettings) { scalingSettings = _scalingSettings; }
#pragma endregion Scaling
	
#pragma region Spawner
private:
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Spawner")
	FSpawnerSettings spawnerSettings;

	UPROPERTY(EditAnywhere, Category = "Pattern Data | Spawner")
	bool bSpawnerDefault = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Spawner")
	FSpawnerDefaultSettings spawnerDefaultSettings;

	UPROPERTY(EditAnywhere, Category = "Pattern Data | Spawner")
	bool bSpawnerConcentric = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Spawner")
	FSpawnerConcentricSettings spawnerConcentricSettings;
	
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Spawner")
	bool bSpawnerMulti = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Spawner")
	FSpawnerMultiSettings spawnerMultiSettings;
	
public:
	FORCEINLINE const bool& GetSpawnerDefaultState() const { return bSpawnerDefault; }
	FORCEINLINE const bool& GetSpawnerConcentricState() const { return bSpawnerConcentric; }
	FORCEINLINE const bool& GetSpawnerMultiState() const { return bSpawnerMulti; }
	
	FORCEINLINE const FSpawnerSettings& GetSpawnerSettings() const { return spawnerSettings; }
	FORCEINLINE const FSpawnerDefaultSettings& GetSpawnerDefaultSettings() const { return spawnerDefaultSettings; }
	FORCEINLINE const FSpawnerConcentricSettings& GetSpawnerConcentricSettings() const { return spawnerConcentricSettings; }
	FORCEINLINE const FSpawnerMultiSettings& GetSpawnerMultiSettings() const { return spawnerMultiSettings; }

	FORCEINLINE void SetSpawnerDefaultState(const bool& _state) { bSpawnerDefault = _state; }
	FORCEINLINE void SetSpawnerConcentricState(const bool& _state) { bSpawnerConcentric = _state; }
	FORCEINLINE void SetSpawnerMultiState(const bool& _state) { bSpawnerMulti = _state; }
	
	FORCEINLINE void SetSpawnerSettings(const FSpawnerSettings& _spawnerSettings) { spawnerSettings = _spawnerSettings; }
	FORCEINLINE void SetSpawnerDefaultSettings(const FSpawnerDefaultSettings& _spawnerDefaultSettings) { spawnerDefaultSettings = _spawnerDefaultSettings; }
	FORCEINLINE void SetSpawnerConcentricSettings(const FSpawnerConcentricSettings& _spawnerConcentricSettings) { spawnerConcentricSettings = _spawnerConcentricSettings; }
	FORCEINLINE void SetSpawnerMultiSettings(const FSpawnerMultiSettings& _spawnerMultiSettings) { spawnerMultiSettings = _spawnerMultiSettings; }
#pragma endregion Spawner
	
#pragma region GPE
private:
	UPROPERTY(EditAnywhere, Category = "Pattern Data | GPE")
	bool bEditProjectileGPE = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | GPE")
	FGPE_ProjectileSettings gpeProjectileSettings;

	UPROPERTY(EditAnywhere, Category = "Pattern Data | GPE")
	bool bEditFloorGPE = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | GPE")
	FGPE_FloorDangerSettings floorDangerSettings;

	UPROPERTY(EditAnywhere, Category = "Pattern Data | GPE")
	bool bEditHuntRiftGPE = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | GPE")
	FGPE_HuntRiftSettings huntRiftSettings;
	
public:
	FORCEINLINE const bool& GetEditProjectileState() const { return bEditProjectileGPE; }
	FORCEINLINE const bool& GetEditFloorState() const { return bEditFloorGPE; }
	FORCEINLINE const bool& GetEditHuntRiftState() const { return bEditHuntRiftGPE; }
	
	FORCEINLINE const FGPE_ProjectileSettings& GetGPEProjectileSettings() const { return gpeProjectileSettings; }
	FORCEINLINE const FGPE_FloorDangerSettings& GetGPEFloorDangerSettings() const { return floorDangerSettings; }
	FORCEINLINE const FGPE_HuntRiftSettings& GetGPEHuntRiftSettings() const { return huntRiftSettings; }

	FORCEINLINE void SetEditProjectileState(const bool& _state) { bEditProjectileGPE = _state; }
	FORCEINLINE void SetEditFloorState(const bool& _state) { bEditFloorGPE = _state; }
	FORCEINLINE void SetEditHuntRiftState(const bool& _state) { bEditHuntRiftGPE = _state; }
	
	FORCEINLINE void SetGPEProjectileSettings(const FGPE_ProjectileSettings& _gpeProjectileSettings) { gpeProjectileSettings = _gpeProjectileSettings; }
	FORCEINLINE void SetGPEFloorDangerSettings(const FGPE_FloorDangerSettings& _floorDangerSettings) { floorDangerSettings = _floorDangerSettings; }
	FORCEINLINE void SetGPEHuntRiftSettings(const FGPE_HuntRiftSettings& _huntRiftSettings) { huntRiftSettings = _huntRiftSettings; }
#pragma endregion GPE
	
#pragma region Wave
private:
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Wave")
	bool bOverrideWave = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Wave")
	FWaveSettings waveSettings;
	
public:
	FORCEINLINE const bool& GetOverrideWave() const { return bOverrideWave; }
	
	FORCEINLINE const FWaveSettings& GetWaveSettings() const { return waveSettings; }

	FORCEINLINE void SetOverrideWave(const bool& _state) { bOverrideWave = _state; }
	
	FORCEINLINE void SetWaveSettings(const FWaveSettings& _waveSettings) { waveSettings = _waveSettings; }
#pragma endregion Wave
	
#pragma region Shoot
private:
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Shoot")
	FShootSettings shootSettings;

	UPROPERTY(EditAnywhere, Category = "Pattern Data | Shoot")
	bool bShootPoint = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Shoot")
	FShootPointSettings shootPointSettings;

	UPROPERTY(EditAnywhere, Category = "Pattern Data | Shoot")
	bool bShootLine = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Shoot")
	FShootLineSettings shootLineSettings;

	UPROPERTY(EditAnywhere, Category = "Pattern Data | Shoot")
	bool bShootCircle = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Shoot")
	FShootCircleSettings shootCircleSettings;

	UPROPERTY(EditAnywhere, Category = "Pattern Data | Shoot")
	bool bShootArc = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Shoot")
	FShootArcSettings shootArcSettings;

	UPROPERTY(EditAnywhere, Category = "Pattern Data | Shoot")
	bool bShootCross = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Shoot")
	FShootCrossSettings shootCrossSettings;

	UPROPERTY(EditAnywhere, Category = "Pattern Data | Shoot")
	bool bShootRandom2D = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Shoot")
	FShootRandom2DSettings shootRandom2DSettings;

	UPROPERTY(EditAnywhere, Category = "Pattern Data | Shoot")
	bool bShootRandom3D = false;
	UPROPERTY(EditAnywhere, Category = "Pattern Data | Shoot")
	FShootRandom3DSettings shootRandom3DSettings;

	UPROPERTY(EditAnywhere, Category = "Pattern Data | Shoot")
	FProjectileSettings projectileShootSettings;
	
public:
	FORCEINLINE const bool& GetShootPointState() const { return bShootPoint; }
	FORCEINLINE const bool& GetShootLineState() const { return bShootLine; }
	FORCEINLINE const bool& GetShootCircleState() const { return bShootCircle; }
	FORCEINLINE const bool& GetShootArcState() const { return bShootArc; }
	FORCEINLINE const bool& GetShootCrossState() const { return bShootCross; }
	FORCEINLINE const bool& GetShootRandom2DState() const { return bShootRandom2D; }
	FORCEINLINE const bool& GetShootRandom3DState() const { return bShootRandom3D; }
	
	FORCEINLINE const FShootSettings& GetShootSettings() const { return shootSettings; }
	FORCEINLINE const FShootPointSettings& GetShootPointSettings() const { return shootPointSettings; }
	FORCEINLINE const FShootLineSettings& GetShootLineSettings() const { return shootLineSettings; }
	FORCEINLINE const FShootCircleSettings& GetShootCircleSettings() const { return shootCircleSettings; }
	FORCEINLINE const FShootArcSettings& GetShootArcSettings() const { return shootArcSettings; }
	FORCEINLINE const FShootCrossSettings& GetShootCrossSettings() const { return shootCrossSettings; }
	FORCEINLINE const FShootRandom2DSettings& GetShootRandom2DSettings() const { return shootRandom2DSettings; }
	FORCEINLINE const FShootRandom3DSettings& GetShootRandom3DSettings() const { return shootRandom3DSettings; }
	FORCEINLINE const FProjectileSettings& GetProjectileShootSettings() const { return projectileShootSettings; }

	FORCEINLINE void SetShootPointState(const bool& _state) { bShootPoint = _state; }
	FORCEINLINE void SetShootLineState(const bool& _state) { bShootLine = _state; }
	FORCEINLINE void SetShootCircleState(const bool& _state) { bShootCircle = _state; }
	FORCEINLINE void SetShootArcState(const bool& _state) { bShootArc = _state; }
	FORCEINLINE void SetShootCrossState(const bool& _state) { bShootCross = _state; }
	FORCEINLINE void SetShootRandom2DState(const bool& _state) { bShootRandom2D = _state; }
	FORCEINLINE void SetShootRandom3DState(const bool& _state) { bShootRandom3D = _state; }
	
	FORCEINLINE void SetShootSettings(const FShootSettings& _shootSettings) { shootSettings = _shootSettings; }
	FORCEINLINE void SetShootPointSettings(const FShootPointSettings& _shootPointSettings) { shootPointSettings = _shootPointSettings; }
	FORCEINLINE void SetShootLineSettings(const FShootLineSettings& _shootLineSettings) { shootLineSettings = _shootLineSettings; }
	FORCEINLINE void SetShootCircleSettings(const FShootCircleSettings& _shootCircleSettings) { shootCircleSettings = _shootCircleSettings; }
	FORCEINLINE void SetShootArcSettings(const FShootArcSettings& _shootArcSettings) { shootArcSettings = _shootArcSettings; }
	FORCEINLINE void SetShootCrossSettings(const FShootCrossSettings& _shootCrossSettings) { shootCrossSettings = _shootCrossSettings; }
	FORCEINLINE void SetShootRandom2DSettings(const FShootRandom2DSettings& _shootRandom2DSettings) { shootRandom2DSettings = _shootRandom2DSettings; }
	FORCEINLINE void SetShootRandom3DSettings(const FShootRandom3DSettings& _shootRandom3DSettings) { shootRandom3DSettings = _shootRandom3DSettings; }
	FORCEINLINE void SetProjectileShootSettings(const FProjectileSettings& _projectileSettings) { projectileShootSettings = _projectileSettings; }
#pragma endregion Shoot

#pragma region Feedback
	UPROPERTY(EditAnywhere, Category = "Pattern Data | FeedBack")
		FTripleDeluxeFeedBack tripleFeedBackData;
	FORCEINLINE const FTripleDeluxeFeedBack& GetTripleFeedBackData() const { return tripleFeedBackData; }
	FORCEINLINE void SetTripleFeedBackData(const FTripleDeluxeFeedBack& _tripleFeedBackData) { tripleFeedBackData = _tripleFeedBackData; }
#pragma  endregion 
};