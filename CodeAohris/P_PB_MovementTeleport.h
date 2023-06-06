#pragma once

#include "CoreMinimal.h"
#include "P_PB_Movement.h"

#include "STRUCT_MovementTeleportSettings.h"

#include "P_PB_MovementTeleport.generated.h"

UCLASS()
class INSIDEUS_API UP_PB_MovementTeleport : public UP_PB_Movement
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTeleport);
	
	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
	FMovementTeleportSettings movementTeleportSettings;

	UPROPERTY(VisibleAnywhere, Category = "Behavior | Teleport | Values")
	AActor* target = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Teleport | Values")
	float fElapsedTime = 0;
	UPROPERTY(VisibleAnywhere, Category = "Behavior | Teleport | Values")
	unsigned int iTeleportTime = 0;

	UPROPERTY()
	FOnTeleport onTeleport;

public:
	FORCEINLINE FOnTeleport& OnTeleport() { return onTeleport; }
	
private:
	virtual void InitializeMovement(UP_DT_PatternDataTable* _dataTable) override;
	virtual void InitializeMovement(FPatternData& _patternData) override;

	virtual void MovementBehavior(const float& _deltaTime) override;

	void Teleport();
};