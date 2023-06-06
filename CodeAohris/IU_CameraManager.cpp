#include "IU_CameraManager.h"

#include "M_CinematicManager.h"
#include "M_GameInstance.h"
#include "M_GameMode.h"
#include "S_PlayerSave.h"

AIU_CameraManager::AIU_CameraManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AIU_CameraManager::BeginPlay()
{
	Super::BeginPlay();

	if (UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
		if (US_PlayerSave* _playerSave = _gi->GetPlayerSave())
			_playerSave->OnPlayerSaveEdited().AddUniqueDynamic(this, &AIU_CameraManager::UpdateCameras);
	GetWorld()->GetAuthGameMode<AM_GameMode>()->GetCinematicManager()->OnCinematicFinishedEvent().AddUniqueDynamic(this,&AIU_CameraManager::CinematicFinished);
}

void AIU_CameraManager::UpdateCameras(const US_PlayerSave* _playerSave)
{
	if (!_playerSave) return;

	for (const auto& _camera : allCameras)
		UpdateCamera(_camera.Value, _playerSave);
}

void AIU_CameraManager::UpdateCamera(AIU_Camera* _camera, const US_PlayerSave* _playerSave)
{
	if (!_camera) return;
	_camera->UpdateCameraFromSave(_playerSave);
}

void AIU_CameraManager::CinematicFinished()
{
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(currentCamera);
}

AIU_Camera* AIU_CameraManager::GetOrCreateCameraByType(ECameraType _cameraType)
{
	if (allCameras.Contains(_cameraType))
	{
		return currentCamera = allCameras[_cameraType];
	}

	if (cameraRefs.Num() < 0) return nullptr;
	AIU_Camera* _camera = GetWorld()->SpawnActor<AIU_Camera>(cameraRefs[_cameraType], GetActorLocation(), GetActorRotation());
	
	if (IsValid(_camera))
	{
		allCameras.Add(_cameraType, _camera);
		if (const UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
			UpdateCamera(_camera, _gi->GetPlayerSave());
	}
	
	return currentCamera = _camera;
}
