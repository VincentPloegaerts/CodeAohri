#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ENUM_BossPhase.h"

#include "MovieSceneSequencePlaybackSettings.h"
#include "LevelSequence.h"
#include "MediaAssets/Public/MediaPlayer.h"
#include "MediaAssets/Public/MediaSource.h"

#include "M_CinematicManager.generated.h"

class AIU_Player;
class AHUD_Game;
class UUI_CinematicWidget;
class ALevelSequenceActor;
class ULevelSequencePlayer;

USTRUCT()
struct FCinematicData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (EditCondition = "!cinematicMediaPlayer", EditConditionHides))
	ULevelSequence* cinematicSequence = nullptr;
	
	UPROPERTY(EditAnywhere)
	UMediaPlayer* cinematicMediaPlayer = nullptr;
	UPROPERTY(EditAnywhere)
	UMaterialInstance* cinematicMaterial = nullptr;
	UPROPERTY(EditAnywhere)
	UMediaSource* cinematicSource = nullptr;

	FORCEINLINE bool IsSequenceValid() const { return cinematicSequence != nullptr; }
	FORCEINLINE bool IsMediaValid() const { return cinematicMediaPlayer && cinematicMaterial && cinematicSource; }
};

UCLASS()
class INSIDEUS_API AM_CinematicManager : public AActor
{
	GENERATED_BODY()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCinematicFinishedEvent);
	
	UPROPERTY(EditAnywhere, Category = "Manager | Settings", meta = (ClampMin = 0.01))
	float holdTime = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Manager | Settings", meta = (ClampMin = 0.01))
	float playerResetTime = 0.2f;
	
	UPROPERTY(EditAnywhere, Category = "Manager | Settings", meta = (ClampMin = 0.01))
	float initTime = 0.1f;
	UPROPERTY(EditAnywhere, Category = "Manager | Cinematic")
	FCinematicData levelIntroCinematic;
	UPROPERTY(EditAnywhere, Category = "Manager | Cinematic")
    FCinematicData levelTwoIntroCinematic;
	UPROPERTY(EditAnywhere, Category = "Manager | Cinematic")
    FCinematicData levelThreeIntroCinematic;
    
	UPROPERTY(EditAnywhere, Category = "Manager | Cinematic")
	FCinematicData tutoIntroCinematic;
	UPROPERTY(EditAnywhere, Category = "Manager | Cinematic")
    FCinematicData tutoOutroCinematic;	
	
	UPROPERTY(EditAnywhere, Category = "Manager | Cinematic")
	FCinematicData templeDoorCinematic;
	
	UPROPERTY(EditAnywhere, Category = "Manager | Cinematic")
	TMap<TEnumAsByte<EBossPhase>, FCinematicData> bossIntroCinematic =
	{
		{ EBossPhase::UN, FCinematicData() },
		{ EBossPhase::DEUX, FCinematicData() },
		{ EBossPhase::TROIS, FCinematicData() },
	};
	UPROPERTY(EditAnywhere, Category = "Manager | Cinematic")
	TMap<TEnumAsByte<EBossPhase>, FCinematicData> bossEndCinematic =
	{
		{ EBossPhase::UN, FCinematicData() },
		{ EBossPhase::DEUX, FCinematicData() },
		{ EBossPhase::TROIS, FCinematicData() },
	};

	UPROPERTY(EditAnywhere, Category = "Manager | Values")
	bool passCinematic = false;
	UPROPERTY()
	AHUD_Game* hud = nullptr;
	UPROPERTY()
	UUI_CinematicWidget* widget = nullptr;
	UPROPERTY()
	AIU_Player* player = nullptr;
	UPROPERTY()
	APlayerController* playerController = nullptr;
	UPROPERTY()
	ALevelSequenceActor* currentLevelSequenceActor = nullptr;
	UPROPERTY()
	UMediaPlayer* currentMediaPlayer = nullptr;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, EditAnywhere, meta = (AllowPrivateAccess = True))
	FOnCinematicFinishedEvent onCinematicFinishedEvent;

	float holdingTime = 0;
	bool isHolding = false;
	bool canPass = true;
	
public:
	FORCEINLINE bool CanPlayCinematic() const { return !passCinematic && IsCinematicManagerValid(); }
	FORCEINLINE bool IsCinematicManagerValid() const { return player && playerController; }
	FORCEINLINE FOnCinematicFinishedEvent& OnCinematicFinishedEvent() { return onCinematicFinishedEvent; }
	
public:	
	AM_CinematicManager();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION() void InitializeCinematicManager();

public:
	UFUNCTION(BlueprintCallable) void PlayLevelIntroCinematic();
	UFUNCTION(BlueprintCallable) void PlayLevelTwoIntroCinematic();
	UFUNCTION(BlueprintCallable) void PlayLevelThreeIntroCinematic();
	
    UFUNCTION(BlueprintCallable) void PlayTutoCinematic();
    UFUNCTION(BlueprintCallable) void PlayTutoOutroCinematic();
	
    UFUNCTION(BlueprintCallable) void PlayTempleDoorCinematic();
	
    UFUNCTION(BlueprintCallable) void PlayBossIntroCinematic(const EBossPhase& _bossPhase);
	
	UFUNCTION(BlueprintImplementableEvent) void OnBoss();
    UFUNCTION(BlueprintCallable) void PlayBossEndCinematic(const EBossPhase& _bossPhase);

protected:
	UFUNCTION(BlueprintImplementableEvent) void OnIntroCinematicPlayed();
	UFUNCTION(BlueprintImplementableEvent) void OnIntroTwoCinematicPlayed();
	UFUNCTION(BlueprintImplementableEvent) void OnIntroThreeCinematicPlayed();
	
	UFUNCTION(BlueprintImplementableEvent) void OnTempleDoorCinematicPlayed();
	
	UFUNCTION(BlueprintImplementableEvent) void OnTutoIntroCinematicPlayed();
	UFUNCTION(BlueprintImplementableEvent) void OnTutoOutroCinematicPlayed();
	
	UFUNCTION(BlueprintImplementableEvent) void OnBossIntroCinematicPlayed(EBossPhase _bossPhase);
	UFUNCTION(BlueprintImplementableEvent) void OnBossOutroCinematicPlayed(EBossPhase _bossPhase);
	
	UFUNCTION(BlueprintImplementableEvent) void OnCinematicFinished();
	
private:
	bool PlayCinematic(const FCinematicData& _cinematicData, const bool hidePlayer = true);
	
	bool PlaySequence(ULevelSequence* _sequence, const bool hidePlayer = true);
	bool PlayMedia(UMediaPlayer* _mediaPlayer, UMediaSource* _mediaSource, UMaterialInstance* _mediaMaterial);

	void LockPlayer(const bool& _hide);
	void UnlockPlayer();

	void ShowCinematicWidget();
	void HideCinematicWidget();
	
	
	UFUNCTION() void BeginHold();
public:
	UFUNCTION() void EndHold();
private:
	UFUNCTION() void OnSequenceFinished();
	UFUNCTION() void InitializeHUD(AActor* _levelSequenceActor);
};