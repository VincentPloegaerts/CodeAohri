#include "UI_RushModeWidget.h"

#include "HUD_Main.h"
#include "IU_Macros.h"
#include "M_GameInstance.h"
#include "S_PlayerSave.h"
#include "UI_DifficultyMenuWidget.h"

void UUI_RushModeWidget::SetVisibility(ESlateVisibility InVisibility)
{
	if (initScores && SteamIntegration)
	{
		if (InVisibility == ESlateVisibility::Visible)
		{
			SteamIntegration->OnLeaderboardDownloadCompleted().AddUniqueDynamic(this, &UUI_RushModeWidget::ReceiveSteamData);
			SteamIntegration->DownloadBossLeaderboard(UN, ELeaderboardDataRequest::k_ELeaderboardDataRequestGlobalAroundUser, 0, 0);
		}
	}
	
	Super::SetVisibility(InVisibility);
}

void UUI_RushModeWidget::InitializeWidget()
{
	Super::InitializeWidget();
	fullRushButton->OnClicked.AddUniqueDynamic(this, &UUI_RushModeWidget::PlayFullRushMode);
	bossButton1->OnClicked.AddUniqueDynamic(this, &UUI_RushModeWidget::PlayRushModeBoss1);
	bossButton2->OnClicked.AddUniqueDynamic(this, &UUI_RushModeWidget::PlayRushModeBoss2);
	bossButton3->OnClicked.AddUniqueDynamic(this, &UUI_RushModeWidget::PlayRushModeBoss3);
	gameInstance = GetGameInstance<UM_GameInstance>();
	if (!gameInstance) return;
	playerSave = gameInstance->GetPlayerSave();

	SteamIntegration = gameInstance->GetSteamIntegration();
	if (SteamIntegration)
	{
		bossButton1->SetIsEnabled(false);
		bossButton2->SetIsEnabled(false);
		bossButton3->SetIsEnabled(false);
	}
	else
		initScores = false;
}

void UUI_RushModeWidget::InitializeFocus()
{
	fullRushButton->SetKeyboardFocus();
}

void UUI_RushModeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (initScores)
	{
		if (scoreReceived)
		{
			if (entries.Num() > 0)
			{
				const LeaderboardEntry_t& _entry = entries[0].entry;
				if (downloadPhase == 0)
				{
					ON_SCREEN_LOG_TIME(10, FColor::Red, FString::Printf(TEXT("Boss One : %d"), _entry.m_nScore));
					downloadPhase = 1;
					InitBoss1(_entry);
				}
				else if (downloadPhase == 1)
				{
					downloadPhase = 2;					
					InitBoss2(_entry);
				}
				else if (downloadPhase == 2)
				{
					downloadPhase = -1;				
					InitBoss3(_entry);
				}
				else
				{
					InitTotal(_entry);
					initScores = false;
					SteamIntegration->OnLeaderboardDownloadCompleted().RemoveDynamic(this, &UUI_RushModeWidget::ReceiveSteamData);
					return;
				}
			}

			scoreReceived = false;
			if (downloadPhase >= 0)
			{
				const EBossPhase& _phase = downloadPhase == 0 ? UN : downloadPhase == 1 ? DEUX : TROIS;
				SteamIntegration->DownloadBossLeaderboard(_phase, ELeaderboardDataRequest::k_ELeaderboardDataRequestGlobalAroundUser, 0, 0);
			}
			else
				SteamIntegration->DownloadTotalLeaderboard(ELeaderboardDataRequest::k_ELeaderboardDataRequestGlobalAroundUser, 0, 0);
		}
	}
}

void UUI_RushModeWidget::PlayFullRushMode()
{
	if (gameInstance)
	{
		PlayRushModeBoss1();
	}
}

void UUI_RushModeWidget::PlayRushModeBoss1()
{
	OnBossSelected(boss1);
}
void UUI_RushModeWidget::PlayRushModeBoss2()
{
	OnBossSelected(boss2);
}
void UUI_RushModeWidget::PlayRushModeBoss3()
{
	OnBossSelected(boss3);
}

void UUI_RushModeWidget::OnBossSelected(TSoftObjectPtr<AB_Boss> _boss)
{
	if (!gameInstance || !hud) return;
	
	gameInstance->SetCurrentStepBossRush(_boss);		
	if (UUI_DifficultyMenuWidget* _widget = hud->GetDifficultyWidget())
		_widget->OnDifficultySelected().AddUniqueDynamic(this, &UUI_RushModeWidget::OpenRushLevel);
}
void UUI_RushModeWidget::OpenRushLevel(const EDifficultyType& _difficulty)
{
	gameInstance->ChangeLevel(RUSH_LEVEL);
}

void UUI_RushModeWidget::Back()
{
	if (hud)
		if (UUI_DifficultyMenuWidget* _widget = hud->GetWidget<UUI_DifficultyMenuWidget>(WIDGET_DIFFICULTY))
			_widget->OnDifficultySelected().RemoveDynamic(this, &UUI_RushModeWidget::OpenRushLevel);
	
	Super::Back();
}

void UUI_RushModeWidget::ReceiveSteamData(const TArray<FLeaderboardEntry>& _entries)
{
	ON_SCREEN_LOG_TIME(10, FColor::Red, FString::Printf(TEXT("Receive Boss score (%d entry)"), _entries.Num()));
	entries = _entries;
	scoreReceived = true;
}

void UUI_RushModeWidget::InitBoss1(const LeaderboardEntry_t& _entry)
{
	bossButton1->SetIsEnabled(true);
	bossText1->SetText(FText::FromString(FTimespan::FromSeconds(_entry.m_nScore).ToString()));
}
void UUI_RushModeWidget::InitBoss2(const LeaderboardEntry_t& _entry)
{
	bossButton2->SetIsEnabled(true);
	bossText2->SetText(FText::FromString(FTimespan::FromSeconds(_entry.m_nScore).ToString()));
}
void UUI_RushModeWidget::InitBoss3(const LeaderboardEntry_t& _entry)
{
	bossButton3->SetIsEnabled(true);
	bossText3->SetText(FText::FromString(FTimespan::FromSeconds(_entry.m_nScore).ToString()));
}

void UUI_RushModeWidget::InitTotal(const LeaderboardEntry_t& _entry)
{
	fullText->SetText(FText::FromString(FTimespan::FromSeconds(_entry.m_nScore).ToString()));
}
