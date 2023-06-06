#include "UI_PlayerWidget.h"

#include "M_GameInstance.h"
#include "S_PlayerSave.h"

#include "B_Boss.h"

#include "IU_HealthSystemComponent.h"
#include "UC_PlayerAttackComponent.h"
#include "C_CollectibleComponent.h"

#include "UI_HitPlayer.h"

#include "Components/CanvasPanelSlot.h"

#include "Kismet/KismetMathLibrary.h"

#include "IU_Macros.h"

void UUI_PlayerWidget::InitializeWidget()
{
	Super::InitializeWidget();
	if (const UCanvasPanelSlot* _slot = Cast<UCanvasPanelSlot>(lifePB->Slot))
	{
		fInitSizeX = _slot->GetSize().X;
		fInitSizeY = _slot->GetSize().Y;
	}
	currentX = fInitSizeX;
	targetX = fInitSizeX;
	
	if (const UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
	{
		playerSave = _gi->GetPlayerSave();
		if (playerSave)
		{
			playerSave->OnPlayerSaveEdited().AddUniqueDynamic(this, &UUI_PlayerWidget::UpdateDisplay);
			UpdateDisplay(playerSave);
		}
	}

	FTimerHandle _timeTimer;
	GetWorld()->GetTimerManager().SetTimer(_timeTimer, this, &UUI_PlayerWidget::UpdateTime, 10.0f, true);

	HideInteractionWidget();
}

void UUI_PlayerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateFPS(InDeltaTime);
	
	//UpdateLifeVisibility(InDeltaTime);

	if (bUpdateSize)
	{
		currentX = FMath::Lerp(currentX, targetX, InDeltaTime * fLifeUpdateSpeed);
		if (currentX >= targetX)
		{
			currentX = targetX;
			bUpdateSize = false;
		}
		UpdateLifeSize(FVector2D(currentX, fInitSizeY), true, false, false);
	}
}

#pragma region Attack
void UUI_PlayerWidget::InitializePlayerAttackUI(UC_PlayerAttackComponent* _attackComponent)
{	
	_attackComponent->OnPrimaryShootChange().AddUniqueDynamic(this, &UUI_PlayerWidget::UpdatePlayerAttack);					//Shoot Type (Texture, color, visibility, etc.)
	_attackComponent->OnPrimaryShootTimer().AddUniqueDynamic(this, &UUI_PlayerWidget::UpdateShotCharge);					//Shot Charge
	
	_attackComponent->OnOrbCountChange().AddUniqueDynamic(this, &UUI_PlayerWidget::UpdateCapacityCharge);					//Capacity Charge (Slow motion)
	
	_attackComponent->OnAlternativeShotCharging().AddUniqueDynamic(this, &UUI_PlayerWidget::UpdateAlternativeShotCharge);	//Alternative Shot charging
	_attackComponent->OnPrimaryShootChange().AddUniqueDynamic(this, &UUI_PlayerWidget::UpdateAlternativeShotSelected);		//Set Alternative Shot widget elements
	_attackComponent->OnAlternativeShotCharged().AddUniqueDynamic(this, &UUI_PlayerWidget::UpdateAlternativeShotCharged);	//Alternative Shot charge complete
	_attackComponent->OnPowerOrbNotUsed().AddUniqueDynamic(this, &UUI_PlayerWidget::UpdateAlternativeShotReset);		//Orb not used
	
	_attackComponent->OnPrimaryShootAltTimer().AddUniqueDynamic(this, &UUI_PlayerWidget::UpdateAlternativeShotTime);		//Alternative Shot remaining Time
	
	_attackComponent->OnEnemyDetection().AddUniqueDynamic(this, &UUI_PlayerWidget::UpdateEnemyDetection);					//Enemy Detection
	
	UpdatePlayerAttack(EPrimaryShootType::NORMAL);
}

void UUI_PlayerWidget::ShowInteractionWidget()
{
	interactWidget->SetVisibility(ESlateVisibility::Visible);
}
void UUI_PlayerWidget::HideInteractionWidget()
{
	interactWidget->SetVisibility(ESlateVisibility::Collapsed);	
}

void UUI_PlayerWidget::ShowMaxLifeBar()
{
	maxLifePB->SetVisibility(ESlateVisibility::Visible);
}
void UUI_PlayerWidget::HideMaxLifeBar()
{
	maxLifePB->SetVisibility(ESlateVisibility::Hidden);	
}

void UUI_PlayerWidget::UpdatePlayerAttack(EPrimaryShootType _shootType)
{
	if (!attacksData.Contains(_shootType)) return;
	shootType = _shootType;
	const FAttackWidgetData& _data = attacksData[_shootType];
	
	SetCrosshair(_data.crosshairTexture, _data.position);
	
	doubleGuidedPB->SetPercent(0);
	harpoonPB->SetPercent(0);
}
void UUI_PlayerWidget::UpdateShotCharge(float _currentCharge, float _maxCharge)
{
	shotChargePB->SetPercent(_currentCharge / _maxCharge);
	shotChargePB->SetVisibility(_currentCharge > 0.0f ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UUI_PlayerWidget::UpdateCapacityCharge(float _currentCharge, float _maxCharge, float _minRequiredCharge)
{
	capacityChargePB->SetPercent(_currentCharge / _maxCharge);
	capacityChargePB->SetFillColorAndOpacity(_currentCharge >= _minRequiredCharge ? capacityAvailableColor : capacityUnavailableColor);
}

void UUI_PlayerWidget::UpdateAlternativeShotCharge(const float _currentCharge, const float _maxCharge)
{
	const float _percent = _currentCharge / _maxCharge;
	
	harpoonPB->SetPercent(_percent);
	doubleGuidedPB->SetPercent(_percent);
}

void UUI_PlayerWidget::UpdateAlternativeShotTime(float _remainingTime, float _totalTime)
{
	const float _percent = 1.0f - _remainingTime / _totalTime;

	if (shootType == EPrimaryShootType::HARPOON)
		harpoonPB->SetPercent(_percent);
	if (shootType == EPrimaryShootType::THORN)
		doubleGuidedPB->SetPercent(_percent);
}

void UUI_PlayerWidget::UpdateEnemyDetection(const bool _hit)
{
	FLinearColor _color = _hit ? crosshairEnemyColor : crosshairNormalColor;
	crosshairIn->SetColorAndOpacity(_color);
	_color.A = _hit || shootType != EPrimaryShootType::NORMAL;
	crosshair->SetColorAndOpacity(_color);
}

void UUI_PlayerWidget::SetCrosshair(UTexture2D* _crosshairTexture, const FVector2D& _position)
{
	crosshair->SetBrushFromTexture(_crosshairTexture);
	
	if (UCanvasPanelSlot* _slot = Cast<UCanvasPanelSlot>(shotChargePB->Slot))
		_slot->SetPosition(_position);		
}
#pragma endregion

#pragma region Boss
void UUI_PlayerWidget::InitializePlayerBossFight(AB_Boss* _boss)
{
	_boss->OnBossActivated().AddDynamic(this, &UUI_PlayerWidget::BossFightStart);
	_boss->OnBossReset().AddDynamic(this, &UUI_PlayerWidget::BossFightEnd);
	_boss->GetHealthComponent()->OnDeath()->AddDynamic(this, &UUI_PlayerWidget::BossFightEnd);
}

void UUI_PlayerWidget::BossFightStart()
{
	bBossFight = true;
	//ShowLifeContent();
	//ShowEnergyContent();
}
void UUI_PlayerWidget::BossFightEnd()
{
	bBossFight = false;
	//HideEnergyContent();
}
#pragma endregion

#pragma region Collectible
void UUI_PlayerWidget::InitializePlayerCollectibleUI(UC_CollectibleComponent* _collectibleComponent)
{
	_collectibleComponent->OnCollectibleUpdated()->AddDynamic(this, &UUI_PlayerWidget::UpdateParchments);
}

void UUI_PlayerWidget::SetParchmentVisibility(bool _status)
{
	collectibleHB->SetVisibility(_status ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
void UUI_PlayerWidget::UpdateParchments(int _currentParchment, int _maxParchment)
{
	const float _percent = (static_cast<float>(_currentParchment) / _maxParchment) * 100.0f;
	collectibleText->SetText(FText::FromString(FString::FromInt(_percent) + " %"));
}
#pragma endregion

#pragma region Health
void UUI_PlayerWidget::InitializePlayerLifeUI(UIU_HealthSystemComponent* _healthComponent)
{
	fInitMaxLife = _healthComponent->GetMaxHealth();
	_healthComponent->OnLifeUpdated()->AddDynamic(this, &UUI_PlayerWidget::UpdateLife);
	_healthComponent->OnActualMaxLifeUpdated()->AddDynamic(this, &UUI_PlayerWidget::UpdateMaxLife);
	//_healthComponent->OnTriesUpdated()->AddDynamic(this, &UUI_PlayerWidget::UpdateTries);

	if (hitPlayer)
	{
		_healthComponent->OnTakeDamage()->AddDynamic(hitPlayer, &UUI_HitPlayer::Hit);
		_healthComponent->OnDeath()->AddDynamic(hitPlayer, &UUI_HitPlayer::Reset);
	}
}

void UUI_PlayerWidget::UpdateLife(float _life, float _maxLife)
{
	lifePB->SetPercent(_life / _maxLife);
	fElapsedTimeDamageTaken = 0;
	bDamageTaken = true;
}
void UUI_PlayerWidget::UpdateMaxLife(float _actualLife, float _maxLife)
{
	maxLifePB->SetPercent(_actualLife / _maxLife);
	const float& _newSizeScale = _maxLife / fInitMaxLife;
	targetX = fInitSizeX * _newSizeScale;
	if (targetX > fInitSizeX)
	{
		bUpdateSize = true;
		UpdateLifeSize(FVector2D(targetX, fInitSizeY), false, true, true);
		ShowExtraLifeContent();
	}
}

void UUI_PlayerWidget::UpdateTries(int _triesLeft)
{
	//tryText->SetText(FText::FromString(_triesLeft < 0 ? "∞" : FString::FromInt(_triesLeft)));
}
#pragma endregion

#pragma region Content Visibility
void UUI_PlayerWidget::UpdateLifeVisibility(const float _deltaTime)
{
	if (!bDamageTaken || bBossFight) return;
	fElapsedTimeDamageTaken += _deltaTime;
	if (fElapsedTimeDamageTaken > fLifeVisibilityTime)
	{
		fElapsedTimeDamageTaken = 0;
		bDamageTaken = false;
		//HideLifeContent();
	}
}

void UUI_PlayerWidget::ShowLifeContent() const
{
	lifePB->SetVisibility(ESlateVisibility::Visible);
	maxLifePB->SetVisibility(ESlateVisibility::Visible);
}
void UUI_PlayerWidget::HideLifeContent() const
{
	lifePB->SetVisibility(ESlateVisibility::Hidden);
	maxLifePB->SetVisibility(ESlateVisibility::Hidden);
}

void UUI_PlayerWidget::ShowEnergyContent() const
{
	capacityChargePB->SetVisibility(ESlateVisibility::Visible);
}
void UUI_PlayerWidget::HideEnergyContent() const
{
	capacityChargePB->SetVisibility(ESlateVisibility::Hidden);
}

void UUI_PlayerWidget::ShowExtraLifeContent()
{
	if (!extraLife->IsVisible())
	{
		extraLife->SetVisibility(ESlateVisibility::Visible);
		extraLifeChest->SetVisibility(ESlateVisibility::Visible);
		PlayExtraLifeAnimation();
	}
}
#pragma endregion 

#pragma region Content Display
void UUI_PlayerWidget::UpdateLifeSize(const FVector2D& _size, bool _updateLife, bool _updateMaxLife, bool _updateDeco)
{
	//Update LifeBar Size
	if (_updateLife)
		if (UCanvasPanelSlot* _slot = Cast<UCanvasPanelSlot>(lifePB->Slot))
			_slot->SetSize(_size);
	if (_updateMaxLife)
		if (UCanvasPanelSlot* _slot = Cast<UCanvasPanelSlot>(maxLifePB->Slot))
			_slot->SetSize(_size);
	if (_updateDeco)
		if (UCanvasPanelSlot* _slot = Cast<UCanvasPanelSlot>(lifeDeco->Slot))
			_slot->SetSize(_size);
}

void UUI_PlayerWidget::UpdateDisplay(const US_PlayerSave* _playerSave)
{
	const FPlayerConfig& _config = _playerSave->GetPlayerConfig();
	SetSizeCrosshair(_config.GetSizeCrosshair());
	SetSizeHUD(_config.GetSizeHUD());

	const FAccessibilityConfig& _playerAccessibility = _playerSave->GetAccessibilityConfig();
	miscBorder->SetVisibility(_playerAccessibility.GetDisplayFPS() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UUI_PlayerWidget::SetSizeCrosshair(float _size)
{
	crosshairPanel->SetRenderScale(FVector2D::One() * _size);
}
void UUI_PlayerWidget::SetSizeHUD(float _size)
{
	const FVector2D& _scale = FVector2D::One() * _size;
	collectibleHB->SetRenderScale(_scale);
	miscBorder->SetRenderScale(_scale);
	playerInfoPanel->SetRenderScale(_scale);
}
#pragma endregion 

void UUI_PlayerWidget::UpdateFPS(const float _deltaTime)
{
	fElapsedTimeFPS += _deltaTime;
	const int& _fps = FMath::TruncToInt(1.0f / _deltaTime);
	fpsCurrentText->SetText(FText::FromString(FString::FromInt(_fps) + " FPS"));
	
	if (fElapsedTimeFPS > fRefreshTimeFPS)
	{
		fElapsedTimeFPS = 0;
		fpsText->SetText(FText::FromString(FString::FromInt(_fps) + " FPS"));
	}
}
void UUI_PlayerWidget::UpdateTime()
{
	const FDateTime& _date = UKismetMathLibrary::Now();
	const FString& _day  = FString::FromInt(_date.GetDay());
	const FString& _month  = FString::FromInt(_date.GetMonth());
	const FString& _hours = FString::FromInt(_date.GetHour());
	const FString& _minute = FString::FromInt(_date.GetMinute());
	const FString& _dateStr = _day + "/" + _month + " - " + _hours + ":" + _minute; 
	timeText->SetText(FText::FromString(_dateStr));
}

void UUI_PlayerWidget::InitializeKeyboardVisual()
{
	interactWidget->InitializeKeyboardVisual();
}
void UUI_PlayerWidget::InitializeGamepadVisual()
{
	interactWidget->InitializeGamepadVisual();
}
