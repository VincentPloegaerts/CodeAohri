#pragma once
#include "CoreMinimal.h"
#include "C_HittableComponent.h"

#include "IU_Camera.h"

#include "Components/ActorComponent.h"
#include "C_AimAssistComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INSIDEUS_API UC_AimAssistComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Aim Assist values")
		bool bShowDebugs = false;
	
	UPROPERTY(EditAnywhere, Category = "Aim Assist values", meta = (ClampMin = "0.1", ClampMax = "60.0"))
		float fStartCheck = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = "Aim Assist values", meta = (ClampMin = "0.001", ClampMax = "60.0"))
		float fCheckRate = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Aim Assist values", meta = (ClampMin = "0.0", ClampMax = "100000.0"))
		float fCheckRange = 10000.0f;
	
	UPROPERTY()
		FTimerHandle resetAimSpeedTimer = FTimerHandle();

	UPROPERTY(EditAnywhere, Category = "Aim Assist values")
		TArray<TEnumAsByte<EObjectTypeQuery>> hittableLayer = TArray<TEnumAsByte<EObjectTypeQuery>>();
	
	UPROPERTY(EditAnywhere, Category = "Aim Assist values")
		TSubclassOf<UC_HittableComponent> hittableObjectType = TSubclassOf<UC_HittableComponent>();

	UPROPERTY(EditAnywhere, Category = "Aim Assist values")
		TArray<AActor*> actorsToIgnore = TArray<AActor*>();
	
	UPROPERTY()
		AIU_Camera* camera = nullptr;
	
public:	
	UC_AimAssistComponent();

private:
	virtual void BeginPlay() override;
	void Init();

	UFUNCTION(BlueprintCallable) void CheckToTarget() const;  
};