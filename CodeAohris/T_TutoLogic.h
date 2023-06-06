#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ENUM_PhaseTuto.h"
#include "HUD_Game.h"
#include "NiagaraComponent.h"
#include "UI_TutoCountElementWidget.h"
#include "UI_TutoBossLifeWidget.h"
#include "UI_Widget.h"
#include "T_TutoLogic.generated.h"

class AIU_Camera;
class AM_GameMode;
class UT_ProjectileDetectionSphere;
class AT_BossTuto;
class AIU_Player;
class AT_ProjectileGuidedTuto;
class AM_CinematicManager;
class AT_BossTutoBlock;
class UUI_TutoElementWidget;

UCLASS()

class INSIDEUS_API AT_TutoLogic : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		TEnumAsByte<EPhaseTuto> currentPhase = NotStarted;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True), Category = "Tuto|Settings")
		float timeBeforeFirstBossActivation = 2;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True), Category = "Tuto|Settings")
		float orbForwardSpawnOffset = 50;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True, ClampMin = 0.1), Category = "Tuto|Settings")
		float showSlowMowDuration = 2.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True, ClampMin = 0.1), Category = "Tuto|Settings")
		int slowMowCount = 20;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True), Category = "Tuto|Settings")
		int harpoonNeededHits = 5;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True), Category = "Tuto|Settings")
		int harpoonStunNeededHits = 4;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True), Category = "Tuto|Settings")
		int guidedSpawnNeededHits = 5;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True), Category = "Tuto|Settings")
		int guidedTrialNeededHits = 5;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True), Category = "Tuto|Settings")
		int hitsCount = 0;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True), Category = "Tuto|Settings")
		float timeBeforeTutoEnd = 2.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True), Category = "Tuto|Settings")
		float newRadiusForDetectionSphere = 500.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True), Category = "Tuto|Settings")
	float tutoBossLife = 3000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True), Category = "Tuto|Settings")
		float timeBetweenWallDashAndSlowMo = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True), Category = "Tuto|Settings")
		bool boolBetWeenWallDashAndSlowMo = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True),Category = "Tuto|Reference")
		AT_BossTuto* refBoss = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True),Category = "Tuto|Reference")
		AIU_Player* refPlayer = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True),Category = "Tuto|Reference")
		AT_BossTutoBlock* refBlock = nullptr;
	UPROPERTY()
		float currentTimerForBossActivation = 0;
	UPROPERTY()
		bool canBeginBossActivationTimer = false;
	UPROPERTY()
		AM_GameMode* gameMode = nullptr;
	UPROPERTY()
		AM_CinematicManager* cinematicManager = nullptr;
	UPROPERTY()
		AHUD_Game* hud = nullptr;
	UPROPERTY()
		bool bStartedTuto = false;
	UPROPERTY()
		float fCurrentWorldSpeed = 1.0f;

	//Double guided power orb spawned in the tuto
	UPROPERTY(EditAnywhere, Category = "Tuto|Settings|Orb To Spawn", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TSubclassOf<AActor> doubleGuidedowerOrb = nullptr;

	//Harpoon power orb spawned in the tuto
	UPROPERTY(EditAnywhere, Category = "Tuto|Settings|Orb To Spawn", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TSubclassOf<AActor> harpoonPowerOrb = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True), Category = "Tuto|Settings|Timer")
	float fTimerForDashThroughWallCheck = 4;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True), Category = "Tuto|Settings|Timer")
	float fTimerForSlowMoDodgeCheck = 4;
	
	
	UPROPERTY(EditAnywhere, Category = "Tuto|Settings|Widgets")
		TSubclassOf<UUI_TutoBossLifeWidget> shotOnBossWidget;
	UPROPERTY(EditAnywhere, Category = "Tuto|Settings|Widgets")
		TSubclassOf<UUI_TutoElementWidget> doubleGuidedWidget;
	UPROPERTY(EditAnywhere, Category = "Tuto|Settings|Widgets")
		TSubclassOf<UUI_TutoElementWidget> harpoonWidget;
	UPROPERTY(EditAnywhere, Category = "Tuto|Settings|Widgets")
		TSubclassOf<UUI_TutoElementWidget> endTutoWidget;

	
	UPROPERTY(EditAnywhere, Category = "Tuto|Settings|Widgets")
		TSubclassOf<UUI_TutoCountElementWidget> iterationThreeShootCountWidget;
	UPROPERTY(EditAnywhere, Category = "Tuto|Settings|Widgets")
		TSubclassOf<UUI_TutoCountElementWidget> iterationStunCountWidget;
	UPROPERTY(EditAnywhere, Category = "Tuto|Settings|Widgets")
		TSubclassOf<UUI_TutoCountElementWidget> iterationDashCancelShootCountWidget;
	UPROPERTY(EditAnywhere, Category = "Tuto|Settings|Widgets")
		TSubclassOf<UUI_TutoCountElementWidget> iterationDoubleGuidedHitCountWidget;
	UPROPERTY(EditAnywhere, Category = "Tuto|Settings|Widgets")
		TSubclassOf<UUI_TutoCountElementWidget> iterationDashThroughWallCountWidget;
	UPROPERTY(EditAnywhere, Category = "Tuto|Settings|Widgets")
		TSubclassOf<UUI_TutoCountElementWidget> iterationSlowMoCountWidget;

	UPROPERTY()
		UUI_TutoCountElementWidget* currentCountElementWidget = nullptr;
	UPROPERTY()
		AIU_Camera* cameraRef = nullptr;
	
	UPROPERTY()
		class UUI_TutoWidget* tutoWidget = nullptr;

	UPROPERTY()
		int currentPhaseNumber = 0;


public:
	AT_TutoLogic();
public:
	UFUNCTION(BlueprintCallable) FORCEINLINE AT_BossTuto* GetTutoBoss() {return refBoss;}
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetTutoBoss(AT_BossTuto* _tutoBoss) {refBoss = _tutoBoss;}
	UFUNCTION(BlueprintCallable) FORCEINLINE EPhaseTuto GetCurrentPhase() {return currentPhase;}
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetCurrentWorldSpeed(const float& _speed) {fCurrentWorldSpeed = _speed;}
	UFUNCTION(BlueprintCallable) FORCEINLINE float GetTimerForDashThroughWallCheck() {return fTimerForDashThroughWallCheck;}
	UFUNCTION(BlueprintCallable) FORCEINLINE float GetTimerForSlowMoDodgeCheck() {return fTimerForSlowMoDodgeCheck;}
	UFUNCTION(BlueprintCallable) FORCEINLINE void ResetTutoCondition() { hitsCount = 0; currentCountElementWidget->ResetCurrentCount();}
	UFUNCTION(BlueprintCallable) FORCEINLINE bool GetBoolBetWeenWallDashAndSlowMo() { return boolBetWeenWallDashAndSlowMo;}
	

public :
	FORCEINLINE AIU_Player* SendRefPlayerToTutoBlock() const {return refPlayer;}
	FORCEINLINE TSubclassOf<UUI_TutoCountElementWidget> GetThreeShootTutoWidget() const {return iterationThreeShootCountWidget;}
	FORCEINLINE TSubclassOf<UUI_TutoCountElementWidget> GetStunTutoWidget() const {return iterationStunCountWidget;}
	FORCEINLINE TSubclassOf<UUI_TutoCountElementWidget> GetDashCancelShootTutoWidget() const {return iterationDashCancelShootCountWidget;}
	FORCEINLINE TSubclassOf<UUI_TutoCountElementWidget> GetDoubleGuidedTutoWidget() const {return iterationDoubleGuidedHitCountWidget;}
	FORCEINLINE TSubclassOf<UUI_TutoCountElementWidget> GetTDashThroughWallTutoWidget() const {return iterationDashThroughWallCountWidget;}
	FORCEINLINE TSubclassOf<UUI_TutoCountElementWidget> GetSlowMoTutoWidget() const {return iterationSlowMoCountWidget;}
	
	FORCEINLINE UUI_TutoCountElementWidget* GetCurrentCountElementWidget() const {return currentCountElementWidget;}
	
	
	
	
	
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION() void Init();
	UFUNCTION(BlueprintCallable)void CheckForBossActivationTime();
	UFUNCTION(BlueprintCallable)void Behaviour(EPhaseTuto _current,const float& _deltaTime);
	UFUNCTION(BlueprintCallable) void EndStartCinematic();
	UFUNCTION(BlueprintCallable) void DashedToDodge();

	UFUNCTION(BlueprintCallable) void CollectedHarpoonOrb();
	UFUNCTION(BlueprintCallable) void CollectedGuidedOrb();
	
	UFUNCTION(BlueprintCallable) void HitFirstShot(float _dmg);
	UFUNCTION(BlueprintCallable) void HitHarpoonSpawn(float _dmg);
	UFUNCTION(BlueprintCallable) void HitShotBeforeHarpoon(float _dmg);
	UFUNCTION(BlueprintCallable) void HitHarpoonStun(float _dmg);
	
	UFUNCTION(BlueprintCallable) void HitGuidedSpawn(float _dmg);
	UFUNCTION(BlueprintCallable) void HitGuidedTrial(float _dmg);
	
	UFUNCTION(BlueprintCallable) void FuckLesSwitch(TEnumAsByte<EWidgetEnum> _widget,float _delay,TEnumAsByte<EPhaseTuto> _phase);
	UFUNCTION(BlueprintCallable) void EndTuto();
	UFUNCTION(BlueprintCallable) void SwitchPhaseToEndTuto();


	UFUNCTION(BlueprintCallable) void ResetPhaseIfTutoDeath();

	UFUNCTION(BlueprintCallable) void StopFX(UNiagaraComponent* niagaraComp);
	UFUNCTION(BlueprintCallable) void SetCam();
public:
	UFUNCTION(BlueprintCallable) void StartTuto();
	UFUNCTION(BlueprintCallable) void SwitchPhase(EPhaseTuto _newPhase);
	UFUNCTION(BlueprintCallable) void RegisterSphere(UT_ProjectileDetectionSphere* _sphere);
	UFUNCTION(BlueprintCallable) void SphereOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult);
	UFUNCTION(BlueprintCallable) void DestroyTutoBossProjectiles();
};
