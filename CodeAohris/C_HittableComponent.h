#pragma once
#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "C_HittableComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INSIDEUS_API UC_HittableComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Hittable values | External", meta = (ClampMin = "0.0", ClampMax = "100.0"))
		float fExternalFactor = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Hittable values | External")
		UBoxComponent* externalBox = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Hittable values | Internal", meta = (ClampMin = "0.0", ClampMax = "100.0"))
		float fInternalFactor = 25.0f;

	UPROPERTY(EditAnywhere, Category = "Hittable values | Internal")
		UBoxComponent* internalBox = nullptr;
	
public:	
	UC_HittableComponent();
	
private:
	virtual void BeginPlay() override;

public:
	float GetSlowFactor(const UBoxComponent* _collidedBox) const;
};
