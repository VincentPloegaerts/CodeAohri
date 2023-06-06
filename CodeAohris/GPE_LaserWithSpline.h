#pragma once

#include "CoreMinimal.h"
#include "GPE_Laser.h"
#include "Components/SplineComponent.h"
#include "GPE_LaserWithSpline.generated.h"


UCLASS()
class INSIDEUS_API AGPE_LaserWithSpline : public AGPE_Laser
{
	GENERATED_BODY()	
#pragma region Properties
	//Time between lasers spawns, used only in Movement mode
	UPROPERTY(EditAnywhere,Category = "GPE|Laser|Spline",BlueprintReadWrite,meta = (AllowPrivateAccess = True),meta=(EditCondition="laserMode == ELaserMode::SplineBackAndForth || laserMode == ELaserMode::SplineMovement",EditConditionHides))
		float respawnTime = 10.0f;
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		float splineLenght = 0.0f;
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FRotator firstRotation = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FVector baseBoxOffset = FVector(0);
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FVector niagaraSize = FVector(0);
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		FCollisionResponseContainer initialCollisions;
	
	//spline component the staticMesh will follow
	UPROPERTY(EditAnywhere,Category = "GPE|Components",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		USplineComponent* splineComponent = nullptr;

	//Will show the spline on play if true
	UPROPERTY(EditAnywhere,Category = "GPE|Laser|Spline|Debug",BlueprintReadWrite,meta = (AllowPrivateAccess = True)
				,meta=(EditCondition="laserMode == ELaserMode::SplineBackAndForth || laserMode == ELaserMode::SplineMovement",EditConditionHides)) //Maybe change to a struct
		bool showDebug = false;
	//how precise the debug needs to be, impact performances
	UPROPERTY(EditAnywhere,Category = "GPE|Laser|Spline|Debug",BlueprintReadWrite,meta = (AllowPrivateAccess = True),
				meta=(EditCondition="laserMode == ELaserMode::SplineBackAndForth || laserMode == ELaserMode::SplineMovement",EditConditionHides),
				meta =(ClampMin = "0.0",ClampMax = "100.0")) //Maybe change to a struct
		float debugPrecisionPercent = 100.0f;
	//Array of lasers created and stocked when respawnTime < duration
	UPROPERTY(VisibleAnywhere,Category = "GPE|Laser|Spline|Debug",BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		TMap<UNiagaraComponent*,UStaticMeshComponent*> laserReserve = TMap<UNiagaraComponent*,UStaticMeshComponent*>();
	//Array of timers created and stocked when respawnTime < duration associated to laserReserve
	UPROPERTY(VisibleAnywhere,Category = "GPE|Laser|Spline|Debug",BlueprintReadWrite,meta = (AllowPrivateAccess = True)) //Maybe change to a struct
		TArray<float> timers = TArray<float>();
#pragma endregion 
public:
	AGPE_LaserWithSpline();
private:
	virtual void BeginPlay() override;	
	virtual void Tick(float DeltaSeconds) override;
	UFUNCTION(BlueprintCallable) void MoveAlongSpline(const float& _actualDistanceOnSpline, const float& _nextDistanceOnSpline, UNiagaraComponent* _toMove) const;
	UFUNCTION(BlueprintCallable) void UpdateSplineBackForth(float _deltaSeconds);
	UFUNCTION(BlueprintCallable) void UpdateSplineMovement(float _deltaSeconds);
	UFUNCTION(BlueprintCallable) void SpawnLasers();
	UFUNCTION(BlueprintCallable) void InitSpline();
	UFUNCTION(BlueprintCallable) void InitNewLaser(const FVector& _startLocation);

#if WITH_EDITOR
	UFUNCTION(BlueprintCallable) void Debug();
#endif
};
