#pragma once
#include "CoreMinimal.h"
#include "GPE_BaseGPE.h"
#include "IU_Player.h"
#include "Components/BoxComponent.h"
#include "GPE_ArenaExit.generated.h"

UCLASS()
class INSIDEUS_API AGPE_ArenaExit : public AGPE_BaseGPE
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeactivate);
#pragma region Properties
#pragma region GPE Values
	UPROPERTY(EditAnywhere, Category = "GPE|Arena Exit", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float damageFirstEnter = 10.0f;
	UPROPERTY(EditAnywhere, Category = "GPE|Arena Exit", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float damagePerDuration = 10.0f;
	UPROPERTY(EditAnywhere, Category = "GPE|Arena Exit", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		float delayDamages = 1.0f;
	UPROPERTY(EditAnywhere, Category = "GPE|Arena Exit", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		AB_Boss* bossRef = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "GPE|Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UIU_HealthSystemComponent* targetHealth = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "GPE|Debug", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		FTimerHandle timer;
#pragma endregion

#pragma region Events
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite,meta = (AllowPrivateAccess = "True"))
		FOnDeactivate onDeactivate;
#pragma endregion

#pragma region Components
	// Collider used to damage the player
	UPROPERTY(EditAnywhere, Category = "GPE|Components", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UBoxComponent* damageBox = nullptr;
	// Collider used to block the player
	UPROPERTY(EditAnywhere, Category = "GPE|Components", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		UBoxComponent* blockingBox = nullptr;
#pragma endregion
#pragma endregion

public:
	AGPE_ArenaExit();
public:
	FORCEINLINE FOnDeactivate& OnDeactivate() {return onDeactivate;}
private:
#pragma region Methods
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	UFUNCTION(BlueprintCallable) void OnCollision(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
		bool _bFromSweep, const FHitResult& _sweepResult);
	UFUNCTION(BlueprintCallable) void OnExitCollision(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex);
	UFUNCTION(BlueprintCallable) void DamageTarget(UIU_HealthSystemComponent* _health,const float& _damage);
	UFUNCTION(BlueprintCallable) void Deactivate();
#pragma endregion 
};
