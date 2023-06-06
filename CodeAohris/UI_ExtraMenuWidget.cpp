#include "UI_ExtraMenuWidget.h"

#include "M_GameInstance.h"
#include "UI_ExtraDataWidget.h"

#include "Components/ScrollBoxSlot.h"

#include "IU_Macros.h"

void UUI_ExtraMenuWidget::SetVisibility(ESlateVisibility InVisibility)
{
	if (steamIntegration)
	{
		if (InVisibility == ESlateVisibility::Visible)
			steamIntegration->OnLeaderboardDownloadCompleted().AddUniqueDynamic(this, &UUI_ExtraMenuWidget::SetLeaderboardData);
		else
			steamIntegration->OnLeaderboardDownloadCompleted().RemoveDynamic(this, &UUI_ExtraMenuWidget::SetLeaderboardData);
	}
	
	Super::SetVisibility(InVisibility);
}

void UUI_ExtraMenuWidget::InitializeWidget()
{
	Super::InitializeWidget();
	
	refreshButton->OnClicked.AddDynamic(this, &UUI_ExtraMenuWidget::RequestLeaderboardData);

	leaderboardTotalButton->OnClicked.AddUniqueDynamic(this, &UUI_ExtraMenuWidget::DisplayTotalLeaderboard);
	leaderboardBossOneButton->OnClicked.AddUniqueDynamic(this, &UUI_ExtraMenuWidget::DisplayBossOneLeaderboard);
	leaderboardBossTwoButton->OnClicked.AddUniqueDynamic(this, &UUI_ExtraMenuWidget::DisplayBossTwoLeaderboard);
	leaderboardBossThreeButton->OnClicked.AddUniqueDynamic(this, &UUI_ExtraMenuWidget::DisplayBossThreeLeaderboard);

	filterGlobalButton->OnClicked.AddUniqueDynamic(this, &UUI_ExtraMenuWidget::SetFilterGlobal);
	filterFriendsButton->OnClicked.AddUniqueDynamic(this, &UUI_ExtraMenuWidget::SetFilterFriends);
	
	if (const UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
		steamIntegration = _gi->GetSteamIntegration();

	ClearLeaderboard();
}

void UUI_ExtraMenuWidget::InitializeFocus()
{
	leaderboardTotalButton->SetKeyboardFocus();
}

void UUI_ExtraMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (leaderboardInit)
		InitializeLeaderboard();
}

void UUI_ExtraMenuWidget::DisplayTotalLeaderboard()
{
	leaderboardIndex = 0;
	
	RequestLeaderboardData();
}
void UUI_ExtraMenuWidget::DisplayBossOneLeaderboard()
{
	leaderboardIndex = 1;
	
	RequestLeaderboardData();
}
void UUI_ExtraMenuWidget::DisplayBossTwoLeaderboard()
{
	leaderboardIndex = 2;
	
	RequestLeaderboardData();
}
void UUI_ExtraMenuWidget::DisplayBossThreeLeaderboard()
{
	leaderboardIndex = 3;
	
	RequestLeaderboardData();
}

void UUI_ExtraMenuWidget::SetFilterGlobal()
{
	leaderboardRequestFilter = ELeaderboardDataRequest::k_ELeaderboardDataRequestGlobal;
	leaderboardMin = 1;
	leaderboardMax = 10;
	
	RequestLeaderboardData();
}
void UUI_ExtraMenuWidget::SetFilterFriends()
{
	leaderboardRequestFilter = ELeaderboardDataRequest::k_ELeaderboardDataRequestFriends;
	leaderboardMin = 1;
	leaderboardMax = 25;
	
	RequestLeaderboardData();
}

void UUI_ExtraMenuWidget::SetLeaderboardData(const TArray<FLeaderboardEntry>& _entries)
{
	ON_SCREEN_LOG_TIME(10, FColor::Green, *FString::Printf(TEXT("Leaderboard Scores Received : %d Entries"), _entries.Num()));
	
	if (userData.IsEmpty())
	{
		userData = _entries;
		GetLeaderboardData(leaderboardRequestFilter, leaderboardMin, leaderboardMax);	//Get other user data (depending on filter)
	}
	else
	{
		leaderboardData = _entries;
		leaderboardInit	= true;
	}
}
void UUI_ExtraMenuWidget::InitializeLeaderboard()
{
	leaderboardInit = false;

	loadingLeaderboardThrobber->SetVisibility(ESlateVisibility::Collapsed);
	
	if (extraDataWidgetType.Get())
	{
		if (userData.Num() > 0)
		{
			UUI_ExtraDataWidget* _userDataWidget = CreateWidget<UUI_ExtraDataWidget>(this, extraDataWidgetType);
			_userDataWidget->InitializeExtraDataWidget(userData[0].entry);
			userDataHB->AddChild(_userDataWidget);
		}
		
		const int _max = leaderboardData.Num();
		for (int i = 0; i < _max; i++)
		{
			const LeaderboardEntry_t& _entry = leaderboardData[i].entry;

			UUI_ExtraDataWidget* _widget = CreateWidget<UUI_ExtraDataWidget>(this, extraDataWidgetType);
			_widget->InitializeExtraDataWidget(_entry);

			if (UScrollBoxSlot* _slot = Cast<UScrollBoxSlot>(leaderboardSB->AddChild(_widget)))
			{
				_slot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
				_slot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
			}			
		}
	}

	userData.Empty();
	leaderboardData.Empty();
}
void UUI_ExtraMenuWidget::GetLeaderboardData(const ELeaderboardDataRequest& _request, const int _min, const int _max)
{
	if (!steamIntegration) return;
	
	if (leaderboardIndex == 0)
		steamIntegration->DownloadTotalLeaderboard(_request, _min, _max);
	if (leaderboardIndex == 1)
		steamIntegration->DownloadBossLeaderboard(EBossPhase::UN, _request, _min, _max);
	if (leaderboardIndex == 2)
		steamIntegration->DownloadBossLeaderboard(EBossPhase::DEUX, _request, _min, _max);
	if (leaderboardIndex == 3)
		steamIntegration->DownloadBossLeaderboard(EBossPhase::TROIS, _request, _min, _max);
}
void UUI_ExtraMenuWidget::ClearLeaderboard()
{
	leaderboardSB->ClearChildren();
	userDataHB->ClearChildren();
}

void UUI_ExtraMenuWidget::RequestLeaderboardData()
{
	ClearLeaderboard();

	loadingLeaderboardThrobber->SetVisibility(ESlateVisibility::Visible);
	
	//First get current user Data 
	GetLeaderboardData(ELeaderboardDataRequest::k_ELeaderboardDataRequestGlobalAroundUser, 0, 0);
}