#pragma once

#include "CoreMinimal.h"
#include "STRUCT_BossCritPoint.h"
#include "GameFramework/Actor.h"
#include "B_BossCritPoint.generated.h"

class AB_Boss;
class AIU_PlayerArrow;

UCLASS(Abstract)
class INSIDEUS_API AB_BossCritPoint : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Crit Point | Settings", meta = (UIMin = "-100", UIMax = "100", ClampMin = "-100", ClampMax = "100"))
	float effect = 0;
	UPROPERTY(EditAnywhere, Category = "Crit Point | Debug")
	bool debug = false;
	
	UPROPERTY(VisibleAnywhere, Category = "Crit Point | Values")
	AB_Boss* boss = nullptr;

public:
	FORCEINLINE void SetBoss(AB_Boss* _boss) { boss = _boss; }
	FORCEINLINE float GetEffectValue() {return effect;}
	
public:	
	AB_BossCritPoint();

private:
	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void DrawCritPointDebug();
	
public:
	void DamageCritPoint(const AIU_PlayerArrow* _arrow) const;

	virtual void GetCritPointSettings(FCritPointSettings& _critPointSettings) const;
	virtual void SetCritPointSettings(const FCritPointSettings& _critPointSettings);
};
