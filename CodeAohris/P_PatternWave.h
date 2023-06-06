#pragma once

#include "CoreMinimal.h"
#include "P_PatternBase.h"
#include "P_PatternWave.generated.h"

class UPoseableMeshComponent;
class UBoxComponent;
class UP_PB_Wave;

UCLASS(Blueprintable)
class INSIDEUS_API AP_PatternWave : public AP_PatternBase
{
	GENERATED_BODY()

	/*Base torus mesh. If you want stack wave on each other, you have to copy past this mesh and attach to this.*/
	UPROPERTY(EditAnywhere, Category = "Pattern Wave",BlueprintReadWrite,meta = (AllowPrivateAccess = "True"))
	UPoseableMeshComponent* poseableMesh = nullptr;

	/*The collider used to damage the player. If you have multiple wave, it's better to scale Up this component.*/
	UPROPERTY(EditAnywhere, Category = "Pattern Wave",BlueprintReadWrite,meta = (AllowPrivateAccess = "True"))
	UBoxComponent* boxCollider = nullptr;

	/*The component that will do wave behavior.*/
	UPROPERTY(EditAnywhere, Category = "Pattern Wave")
	UP_PB_Wave* waveBehavior = nullptr;
	
public:
	FORCEINLINE UPoseableMeshComponent* GetPoseableMeshComponent() const { return poseableMesh; }
	FORCEINLINE UBoxComponent* GetBoxCollider() const { return boxCollider; }
	FORCEINLINE UP_PB_Wave* GetWaveBehavior() const { return waveBehavior; }
	
public:
	AP_PatternWave();

	//Called by TaskNode from boss BehaviorTree
	virtual void InitializePattern(UP_DT_BaseDataTable* _dataTable) override;
	virtual void InitializePatternData(FPatternData& _patternData) override;
	virtual void PatternCompletion() override;
};