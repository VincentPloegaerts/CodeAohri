#pragma once

#include "CoreMinimal.h"
#include "GPE_BaseGPE.h"
#include "I_SavedObject.h"
#include "GPE_Platform.generated.h"

class AIU_Player;

UCLASS(Abstract)
class INSIDEUS_API AGPE_Platform : public AGPE_BaseGPE, public II_SavedObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "GPE")
		float playerDetectionDistance = 8000.0f;
protected:
	UPROPERTY()
		AIU_Player* player = nullptr;
	
protected:
	//Delay before the behaviour of this GPE starts
	UPROPERTY(EditAnywhere, Category = "GPE", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float delayBeforeActivation = 0;
	
	//bool to see if delayBeforeActivation is finished
	UPROPERTY(VisibleAnywhere, Category = "GPE", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		bool started = false;
	
	//staticMesh of this GPE
	UPROPERTY(EditAnywhere, Category = "GPE|Components", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
        UStaticMeshComponent* staticMesh = nullptr;


public:
	AGPE_Platform();
	
public:
	FORCEINLINE UStaticMeshComponent* StaticMesh() const { return staticMesh; }
	FORCEINLINE void SetPlayerDetectionRange(float _value) { playerDetectionDistance = _value; }

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION() void GetPlayerGamemode();
	UFUNCTION() virtual void CheckPlayerDistance();
	
};