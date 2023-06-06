#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"

#include "C_InteractionWidgetComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INSIDEUS_API UC_InteractionWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	float checkRate = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Settings")
	float additionalRange = 0.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "Values")
	AActor* owner = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Values")
	class AIU_Player* player = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Values")
	float playerInteractionRange = 0;
	UPROPERTY(VisibleAnywhere, Category = "Values")
	FTimerHandle checkTimer;
	
	UPROPERTY(VisibleAnywhere, Category = "Values")
	bool enabled = true;
	
public:	
	UC_InteractionWidgetComponent();

private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION() void InitializeInteractionWidgetComponent();
	
	UFUNCTION() void CheckPlayerInteractionRange();

public:
	UFUNCTION() void EnableWidgetInteraction();
	UFUNCTION() void DisableWidgetInteraction();
};
