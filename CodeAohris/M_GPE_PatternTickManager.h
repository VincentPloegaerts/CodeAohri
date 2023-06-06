#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "M_GPE_PatternTickManager.generated.h"

class AHUD_Game;
class UUI_ZoneWarningWidget;
class AIU_CameraManager;
class AIU_Camera;
class AB_Boss;
class AM_BossManager;
class AIU_Player;
class AGPE_Pattern;

/**
 * Class used for optimizing Tick on GPE_Zone and GPE_Boss by
 * registering and Broadcast() there tick function when they are enabled.
 */
UCLASS()
class INSIDEUS_API AM_GPE_PatternTickManager : public AActor
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTick, const float&, _deltaTime);
	
private:
	UPROPERTY(VisibleAnywhere)
	float fWorldSpeed = 1;
	
	UPROPERTY(VisibleAnywhere)
	TArray<AGPE_Pattern*> allGPEPattern;
		
	
	UPROPERTY()
	FOnTick onTick;

	UPROPERTY()
		AIU_Player* player = nullptr;
	UPROPERTY()
		bool bossStarted = false;
	UPROPERTY()
		AM_BossManager* BossManager = nullptr;
	UPROPERTY()
		AB_Boss* currentBoss = nullptr;
	UPROPERTY()
		AIU_Camera* currentCamera = nullptr;
	UPROPERTY()
		AIU_CameraManager* camManager;
	UPROPERTY()
		AHUD_Game* hud = nullptr;
	UPROPERTY()
		UUI_ZoneWarningWidget* zoneWarning = nullptr;
public:
	FORCEINLINE FOnTick& GetOnTick() { return onTick; }
	FORCEINLINE void SetWorldSpeed(const float& _speed) { fWorldSpeed = _speed; }
	
public:	
	AM_GPE_PatternTickManager();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	void AddGPEPattern(AGPE_Pattern* _gpePattern);
	void DestroyGPEPattern(AGPE_Pattern* _gpePattern);
	UFUNCTION() void DestroyEveryGPEPattern();
	
private:
	UFUNCTION(BlueprintCallable) void CheckZonePosition();
	UFUNCTION(BlueprintCallable) void Init();
	UFUNCTION(BlueprintCallable) void SwitchCurrentBoss(AB_Boss* _boss);
	UFUNCTION(BlueprintCallable) void CurrentBossActivated();
	UFUNCTION(BlueprintCallable) void CurrentBossDeath();
};