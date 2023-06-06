#pragma once
#include "CoreMinimal.h"
#include "UI_BackWidget.h"

#include "Components/Button.h"
#include "Components/ScrollBox.h"

#include "STRUCT_LeaderboardEntry.h"
#include "Components/CircularThrobber.h"
#include "Components/HorizontalBox.h"

#include "UI_ExtraMenuWidget.generated.h"

class UAPI_SteamIntegration;
class UUI_ExtraDataWidget;

UCLASS()
class INSIDEUS_API UUI_ExtraMenuWidget : public UUI_BackWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UUI_ExtraDataWidget> extraDataWidgetType = TSubclassOf<UUI_ExtraDataWidget>();
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* refreshButton = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* leaderboardTotalButton = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* leaderboardBossOneButton = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* leaderboardBossTwoButton = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* leaderboardBossThreeButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* filterGlobalButton = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* filterFriendsButton = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* leaderboardSB = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* userDataHB = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCircularThrobber* loadingLeaderboardThrobber = nullptr;
	
private:		
	
	UPROPERTY()
	UAPI_SteamIntegration* steamIntegration = nullptr;
	
	UPROPERTY()
	TArray<FLeaderboardEntry> leaderboardData = { };
	UPROPERTY()
	TArray<FLeaderboardEntry> userData = { };
	UPROPERTY()
	bool leaderboardInit = false;
	
	ELeaderboardDataRequest leaderboardRequestFilter = ELeaderboardDataRequest::k_ELeaderboardDataRequestGlobal;
	//0 : Total / 1 : Boss One / 2 : Boss One / 3 : Boss Three
	int leaderboardIndex = 0;
	int leaderboardMin = 1;
	int leaderboardMax = 10;

public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;
	
	
private:
	virtual void InitializeWidget() override;
	virtual void InitializeFocus() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION() void DisplayTotalLeaderboard();
	UFUNCTION() void DisplayBossOneLeaderboard();
	UFUNCTION() void DisplayBossTwoLeaderboard();
	UFUNCTION() void DisplayBossThreeLeaderboard();

	UFUNCTION() void SetFilterGlobal();
	UFUNCTION() void SetFilterFriends();
	
	UFUNCTION() void SetLeaderboardData(const TArray<FLeaderboardEntry>& _entries);
	void InitializeLeaderboard();
	void GetLeaderboardData(const ELeaderboardDataRequest& _request, const int _min, const int _max);
	void ClearLeaderboard();

	UFUNCTION() void RequestLeaderboardData();
};
