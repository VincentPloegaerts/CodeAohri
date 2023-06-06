#pragma once
#include "CoreMinimal.h"
#include "API_SteamIntegration.h"
#include "ENUM_BossPhase.h"
#include "ENUM_DifficultyType.h"
#include "STRUCT_LeaderboardEntry.h"
#include "UI_BackWidget.h"

#include "Components/Button.h"

#include "UI_RushModeWidget.generated.h"

class AB_Boss;
class US_PlayerSave;
class UM_GameInstance;

UCLASS()
class INSIDEUS_API UUI_RushModeWidget : public UUI_BackWidget
{
	GENERATED_BODY() 

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* fullRushButton = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* bossButton1 = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* bossButton2 = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* bossButton3 = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* fullText = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* bossText1 = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* bossText2 = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* bossText3 = nullptr;
	
	UPROPERTY()
		UM_GameInstance* gameInstance = nullptr;
	UPROPERTY()
		US_PlayerSave* playerSave = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Rush mode values")
		TSoftObjectPtr<AB_Boss> boss1 = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Rush mode values")
		TSoftObjectPtr<AB_Boss> boss2 = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Rush mode values")
		TSoftObjectPtr<AB_Boss> boss3 = nullptr;

	
	int downloadPhase = 0;
	bool initScores = true;
	bool scoreReceived = false;
	
	UPROPERTY()
	TArray<FLeaderboardEntry> entries = { };
	UPROPERTY()
	UAPI_SteamIntegration* SteamIntegration = nullptr;
	
public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;
	
private:
	virtual void InitializeWidget() override;
	
	virtual void InitializeFocus() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION(BlueprintCallable) void PlayFullRushMode();
	UFUNCTION(BlueprintCallable) void PlayRushModeBoss1();
	UFUNCTION(BlueprintCallable) void PlayRushModeBoss2();
	UFUNCTION(BlueprintCallable) void PlayRushModeBoss3();

	void OnBossSelected(TSoftObjectPtr<AB_Boss> _boss);
	UFUNCTION() void OpenRushLevel(const EDifficultyType& _difficulty);

	virtual void Back() override;

	UFUNCTION() void ReceiveSteamData(const TArray<FLeaderboardEntry>& _entries);
	
	void InitBoss1(const LeaderboardEntry_t& _entry);
	void InitBoss2(const LeaderboardEntry_t& _entry);
	void InitBoss3(const LeaderboardEntry_t& _entry);
	void InitTotal(const LeaderboardEntry_t& _entry);
	
};