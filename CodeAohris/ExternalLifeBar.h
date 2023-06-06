#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "ExternalLifeBar.generated.h"

UCLASS()
class INSIDEUS_API UExternalLifeBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UProgressBar* lifeBar = nullptr;


public:
	UFUNCTION(BlueprintCallable) void SetLife(float _current, float _max);
};
