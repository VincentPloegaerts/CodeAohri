#pragma once
#include "CoreMinimal.h"
#include "ENUM_CameraType.h"

#include "IU_Camera.h"

#include "GameFramework/Actor.h"
#include "IU_CameraManager.generated.h"

class US_PlayerSave;

UCLASS()
class INSIDEUS_API AIU_CameraManager : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category = "Camera",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TMap<TEnumAsByte<ECameraType>, AIU_Camera*> allCameras = TMap<TEnumAsByte<ECameraType>, AIU_Camera*>();
	
	UPROPERTY(EditAnywhere, Category = "Camera",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TArray<TSubclassOf<AIU_Camera>> cameraRefs = TArray<TSubclassOf<AIU_Camera>>();
	
	UPROPERTY(VisibleAnywhere, Category = "Camera",BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		AIU_Camera* currentCamera = nullptr;

public:
	FORCEINLINE AIU_Camera* GetCamera() const { return currentCamera; }
	
public:	
	AIU_CameraManager();

private:
	virtual void BeginPlay() override;

	UFUNCTION() void UpdateCameras(const US_PlayerSave* _playerSave);
	void UpdateCamera(AIU_Camera* _camera, const US_PlayerSave* _playerSave);
	UFUNCTION() void CinematicFinished();
	
public:
	UFUNCTION(BlueprintCallable) AIU_Camera* GetOrCreateCameraByType(ECameraType _cameraType);
};