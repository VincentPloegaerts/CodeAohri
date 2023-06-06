#pragma once
#include "CoreMinimal.h"

#include "M_Octree.h"
#include "Camera/CameraComponent.h"

#include "Components/ActorComponent.h"
#include "C_FrustumChecker.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INSIDEUS_API UC_FrustumChecker : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Frutum checker values", meta = (UIMin = "0.1", UIMax = "60.0", ClampMin = "0.1", ClampMax = "60.0"))
		float fCheckRate = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Frutum checker values", meta = (UIMin = "0.0", UIMax = "100000.0", ClampMin = "0.0", ClampMax = "100000.0"))
		float fRange = 500.0f;

	UPROPERTY()
		UCameraComponent* cameraComponent = nullptr;
	
	UPROPERTY()
		UWorld* world = nullptr;

	UPROPERTY()
		AM_Octree* octree = nullptr;

	UPROPERTY()
		ULocalPlayer* localPlayer = nullptr;

	UPROPERTY()
		TObjectPtr<UGameViewportClient> viewportClient = nullptr;

	FViewport* viewport = nullptr;

public:	
	UC_FrustumChecker();

private:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable) void InitTimer(AM_Octree* _octree);
	UFUNCTION(BlueprintCallable) void CheckOctree();
	bool IsInFrustum(const AM_Octree* _target) const;
};
