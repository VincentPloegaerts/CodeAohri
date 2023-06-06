#pragma once
#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "T_ProjectileDetectionSphere.generated.h"



UCLASS()
class INSIDEUS_API UT_ProjectileDetectionSphere : public USphereComponent
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable) void Init();
public:
	UFUNCTION(BlueprintCallable) void ModifyRadius(float _newRadius);
};
