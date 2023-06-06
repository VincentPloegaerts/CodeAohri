#pragma once

#include "CoreMinimal.h"
#include "P_PatternBehavior.h"
#include "P_PB_Wave.generated.h"

class UPoseableMeshComponent;
class UBoxComponent;
class AIU_Player;
class UIU_HealthSystemComponent;

UCLASS()
class INSIDEUS_API UP_PB_Wave : public UP_PatternBehavior
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBehaviorTick);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBehaviorDeltaTick, const float&, _deltaTime);

	/*Every meshes children + the basic cpp Mesh.*/
	UPROPERTY(VisibleAnywhere)
	TArray<UPoseableMeshComponent*> poseableMeshs;

	/*Total of poseable mesh in the array. (Optimization)*/
	UPROPERTY(VisibleAnywhere)
	unsigned int iTotalPoseableMesh = 0;

	/*Capsule taken on the wave AActor.*/
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* boxCollider = nullptr;

	/*Player used for turning the Capsule Collider in his way.*/
	UPROPERTY(VisibleAnywhere)
	AIU_Player* player = nullptr;

	/*The player health component used to damage him.*/
	UPROPERTY(VisibleAnywhere)
	UIU_HealthSystemComponent* playerHealthComponent = nullptr;

/*Settings egal to FWaveSettings but more optimized.*/
#pragma region WaveSettings
	UPROPERTY(VisibleAnywhere)
	float fWaveSpeed = 100.0f;
	
	UPROPERTY(VisibleAnywhere)
	float fWaveDamage = 10.0f;

	UPROPERTY(VisibleAnywhere)
		float fTubeSize = 0.0f;
	UPROPERTY(VisibleAnywhere)
		float fTubeHeight = 0.0f;
	UPROPERTY(VisibleAnywhere)
		float fMaxSize = 0.0f;
	UPROPERTY(VisibleAnywhere)
		bool bUseMaxSize = 0.0f;
#pragma endregion WaveSettings

	/*The current scale of all waves.*/
	UPROPERTY(VisibleAnywhere)
	float fScale = 0.0f;

	/*The local height of the capsule collider at the begin play.*/
	UPROPERTY(VisibleAnywhere)
	float fColliderLocalHeight = 0.0f;

	/*The current scale stored in a vector.*/
	UPROPERTY(VisibleAnywhere)
	FVector vCurrentScale = FVector::ZeroVector;

	/*Optimized OneVector.*/
	UPROPERTY(VisibleAnywhere)
	FVector vOneVector = FVector::OneVector;

	/*Optimized EBoneSpace.*/
	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<EBoneSpaces::Type> eBoneSpace = EBoneSpaces::ComponentSpace;

	/*Event which tick the wave behavior.*/
	UPROPERTY()
	FOnBehaviorTick onBehaviorTick;
	
	/*Event which tick player damage.*/
	UPROPERTY()
	FOnBehaviorDeltaTick onBehaviorDeltaTick;

	/*Duration timer.*/
	UPROPERTY()
	FTimerHandle timer;

#pragma region BonesName
	/*The total number of bones present in the array.(Optimization)*/
	UPROPERTY(VisibleAnywhere)
	unsigned int iTotalBones = 18;

	/*The central bone which is the higher in the hierarchy.*/
	UPROPERTY(VisibleAnywhere)
	FName nParentBone = "externe";

	/*The first bone used to get the central location of the wave border.*/
	UPROPERTY(VisibleAnywhere)
	FName nColliderCenterBoneOne = "root_jntz";
	
	/*The second bone used to get the central location of the wave border.*/
	UPROPERTY(VisibleAnywhere)
	FName nColliderCenterBoneTwo = "CollisionSocket";

	/*Every bones name that have to be scale reset.*/
	UPROPERTY(VisibleAnywhere)
	TArray<FName> bonesNameToReset;
#pragma endregion BonesName	

public:
	UP_PB_Wave();
	
public:
	virtual void ActivateBehavior(UP_DT_BaseDataTable* _dataTable) override;
	virtual void ActivateBehavior(FPatternData& _patternData) override;
	
private:
	void InitPoseableMeshs(const UPoseableMeshComponent* _baseMesh);
	
public:
	virtual void TickBehavior(const float& _deltaTime) override;

private:
	UFUNCTION() void UpdateScale(const float& _deltaTime);
	UFUNCTION() void ScaleWave();
	UFUNCTION() void MoveCollider();

#pragma region DamagePlayer
	UFUNCTION() void OnBeginOverlap(AActor* _mine, AActor* _other);
	UFUNCTION() void DamagePlayer();
	UFUNCTION() void OnEndOverlap(AActor* _mine, AActor* _other);
#pragma endregion DamagePlayer
	
	UFUNCTION() void ReachEndDuration();
};