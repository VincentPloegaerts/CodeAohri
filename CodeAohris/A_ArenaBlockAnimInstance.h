#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "A_ArenaBlockAnimInstance.generated.h"

UCLASS()
class INSIDEUS_API UA_ArenaBlockAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool isOpening = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool isInstant = false;


public:
	UFUNCTION(BlueprintCallable) void UpdateIsOpening(const bool& _isOpening);
	UFUNCTION(BlueprintCallable) void UpdateIsInstantOpening(const bool& _isOpening);
};