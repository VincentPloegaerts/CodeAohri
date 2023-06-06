#pragma once
#include "CoreMinimal.h"

#include "P_Projectile.h"
#include "P_ProjectileBallistic.h"
#include "P_ProjectileGuided.h"
#include "P_ProjectileSimulated.h"
#include "P_ProjectileSinusoidal.h"

#include "GameFramework/Actor.h"
#include "M_DataCenter.generated.h"

UENUM()
enum EProjectileDataType
{
	BALISTIC UMETA(DisplayName = "Balistic"),
	GUIDED UMETA(DisplayName = "Guided"),
	SIMULATED UMETA(DisplayName = "Simulated"),
	SINUSOIDAL UMETA(DisplayName = "Sinusoidal")
};

USTRUCT(BlueprintType)
struct FProjectilesDatas : public FTableRowBase
{
	GENERATED_BODY()

#pragma region Global

	UPROPERTY(EditAnywhere, Category = "Projectiles")
		TArray<TSubclassOf<AP_Projectile>> globalTypes = TArray<TSubclassOf<AP_Projectile>>();
	
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		FProjectileGlobalSettings datas = FProjectileGlobalSettings();

#pragma endregion

	UPROPERTY(EditAnywhere, Category = "Projectiles")
		TEnumAsByte<EProjectileDataType> projectileDataType = TEnumAsByte<EProjectileDataType>();

#pragma region Ballistic
	
	UPROPERTY(EditAnywhere, Category = "Projectiles", meta = (EditCondition = "projectileDataType == EProjectileDataType::BALISTIC", EditConditionHides))
		TArray<TSubclassOf<AP_ProjectileBallistic>> ballisticTypes = TArray<TSubclassOf<AP_ProjectileBallistic>>();
		
	UPROPERTY(EditAnywhere, Category = "Projectiles", meta = (DisplayName = "Ballistic", EditCondition = "projectileDataType == EProjectileDataType::BALISTIC", EditConditionHides))
		FProjectileBallisticSettings ballisticDatas = FProjectileBallisticSettings();

#pragma endregion
	
#pragma region Guided
	
	UPROPERTY(EditAnywhere, Category = "Projectiles", meta = (EditCondition = "projectileDataType == EProjectileDataType::GUIDED", EditConditionHides))
		TArray<TSubclassOf<AP_ProjectileGuided>> guidedTypes = TArray<TSubclassOf<AP_ProjectileGuided>>();
		
	UPROPERTY(EditAnywhere, Category = "Projectiles", meta = (DisplayName = "Guided", EditCondition = "projectileDataType == EProjectileDataType::GUIDED", EditConditionHides))
		FProjectileGuidedSettings selfGuidedDatas = FProjectileGuidedSettings();

#pragma endregion

#pragma region Simulated
	
	UPROPERTY(EditAnywhere, Category = "Projectiles", meta = (EditCondition = "projectileDataType == EProjectileDataType::GUIDED", EditConditionHides))
		TArray<TSubclassOf<AP_ProjectileSimulated>> simulatedTypes = TArray<TSubclassOf<AP_ProjectileSimulated>>();
	
	UPROPERTY(EditAnywhere, Category = "Projectiles", meta = (DisplayName = "Simulated", EditCondition = "projectileDataType == EProjectileDataType::SIMULATED", EditConditionHides))
		FProjectileSimulatedSettings simulatedDatas = FProjectileSimulatedSettings();

#pragma endregion

#pragma region Sinusoidal
	
	UPROPERTY(EditAnywhere, Category = "Projectiles", meta = (EditCondition = "projectileDataType == EProjectileDataType::SINUSOIDAL", EditConditionHides))
		TArray<TSubclassOf<AP_ProjectileSinusoidal>> sinusoidalTypes = TArray<TSubclassOf<AP_ProjectileSinusoidal>>();
	
	UPROPERTY(EditAnywhere, Category = "Projectiles", meta = (DisplayName = "Sinusoidal", EditCondition = "projectileDataType == EProjectileDataType::SINUSOIDAL", EditConditionHides))
		FProjectileSinusoidalSettings sinusoidalDatas = FProjectileSinusoidalSettings();

#pragma endregion 

	FProjectilesDatas() {}
};

UCLASS()
class INSIDEUS_API AM_DataCenter : public AActor
{
	GENERATED_BODY()

#pragma region Projectiles
	
private:
	UPROPERTY(EditAnywhere, Category = "Datas")
		UDataTable* projectileDataTable = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Datas")
		FProjectilesDatas projectileDatas = FProjectilesDatas();

public:
	FORCEINLINE FProjectilesDatas* GetProjectilesDatas(const FName _rowName) const
	{
		return projectileDataTable->FindRow<FProjectilesDatas>(_rowName, _rowName.ToString() + " doesn't exist !");
	}

#pragma endregion 
	
public:
	AM_DataCenter();

private:
	virtual void OnConstruction(const FTransform& Transform) override;
	void Init();
	UFUNCTION() void UpdateProjectilesDatas();

	// Refresh all blueprints saved
	// UFUNCTION(CallInEditor, DisplayName = "Refesh", Category = "Octree values") void Update();
};