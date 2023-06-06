#pragma once
#include "CoreMinimal.h"
#include "UI_Widget.h"

#include "ENUM_PrimaryShootType.h"
#include "STRUCT_UI_AttackWidgetData.h"
#include "UI_TutoElementWidget.h"

#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"

#include "UI_PlayerWidget.generated.h"

class AB_Boss;
class US_PlayerSave;

class UUI_HitPlayer;

class UIU_HealthSystemComponent;
class UC_PlayerAttackComponent;
class UC_CollectibleComponent;

UCLASS()
class INSIDEUS_API UUI_PlayerWidget : public UUI_Widget
{
	GENERATED_BODY()

protected:

#pragma region Player Info
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* playerInfoPanel = nullptr;	

#pragma region Life
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* lifePB = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* maxLifePB = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* lifeDeco = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* extraLife = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWidget* extraLifeChest = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Life", meta = (UIMin = "0", UIMax = "25", ClampMin = "0", ClampMax = "25"))
	float fLifeVisibilityTime = 8.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Life", meta = (ClampMin = "0.1"))
	float fLifeUpdateSpeed = 1; 
#pragma endregion

#pragma region Shot
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* shotChargePB = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* capacityChargePB = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* doubleGuidedPB = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* harpoonPB = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Widget | Capacity")
	FLinearColor capacityAvailableColor = FLinearColor::Blue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Widget | Capacity")
	FLinearColor capacityUnavailableColor = FLinearColor::Gray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Widget | Shot")
	TMap<TEnumAsByte<EPrimaryShootType>, FAttackWidgetData> attacksData =
	{
		{ EPrimaryShootType::NORMAL, FAttackWidgetData() },
		{ EPrimaryShootType::THORN, FAttackWidgetData() },
		{ EPrimaryShootType::HARPOON, FAttackWidgetData() },
	};
	#pragma endregion
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional), Category = "Player Widget | Hit")
	UUI_HitPlayer* hitPlayer = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUI_TutoElementWidget* interactWidget = nullptr;

#pragma endregion 

#pragma region Crosshair
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* crosshairPanel = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* crosshair = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* crosshairIn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Widget | Crosshair")
	FLinearColor crosshairNormalColor = FLinearColor::White;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Widget | Crosshair")
	FLinearColor crosshairEnemyColor = FLinearColor::Red;
#pragma endregion 

#pragma region Collectible
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UHorizontalBox* collectibleHB = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* collectibleText = nullptr;
#pragma endregion

#pragma region Misc
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* miscBorder = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* fpsCurrentText = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* fpsText = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* timeText = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Misc", meta = (UIMin = "0", UIMax = "2", ClampMin = "0", ClampMax = "2"))
	float fRefreshTimeFPS = 0.5f;
	UPROPERTY(BlueprintReadWrite)
	float fElapsedTimeFPS = 0;
#pragma endregion
	
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EPrimaryShootType> shootType = EPrimaryShootType::NORMAL;
	UPROPERTY(BlueprintReadWrite)
	bool bBossFight = false;
	UPROPERTY(BlueprintReadWrite)
	bool bDamageTaken = true;
	UPROPERTY(BlueprintReadWrite)
	float fElapsedTimeDamageTaken = 0;

	UPROPERTY()
	float fInitMaxLife = 0;
	UPROPERTY()
	float fInitSizeX = 512.0f;
	UPROPERTY()
	float fInitSizeY = 49.0f;
	
	UPROPERTY()
	bool bUpdateSize = false;
	UPROPERTY()
	float currentX = 0;
	UPROPERTY()
	float targetX = 0;
	
	UPROPERTY()
	US_PlayerSave* playerSave = nullptr;
	
private:
	virtual void InitializeWidget() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
protected:
	UFUNCTION(BlueprintImplementableEvent) void PlayExtraLifeAnimation();
	
public:
	//Health
	void InitializePlayerLifeUI(UIU_HealthSystemComponent* _healthComponent);

	//Collectible
	void InitializePlayerCollectibleUI(UC_CollectibleComponent* _collectibleComponent);
	UFUNCTION(BlueprintCallable) void SetParchmentVisibility(bool _status);

	//Boss
	void InitializePlayerBossFight(AB_Boss* _boss);
	
	//Attack
	void InitializePlayerAttackUI(UC_PlayerAttackComponent* _attackComponent);
	
private:
	//Health
	UFUNCTION(BlueprintCallable) void UpdateLife(float _life, float _maxLife);
	UFUNCTION(BlueprintCallable) void UpdateMaxLife(float _actualMaxLife, float _maxLife);
	UFUNCTION(BlueprintCallable) void UpdateTries(int _triesLeft);
	
	//Collectible
	UFUNCTION(BlueprintCallable) void UpdateParchments(int _currentParchment, int _maxParchment);
	
	//Attack
	UFUNCTION(BlueprintCallable) void UpdatePlayerAttack(EPrimaryShootType _shootType);
	UFUNCTION(BlueprintCallable) void UpdateShotCharge(float _currentCharge, float _maxCharge);
	UFUNCTION(BlueprintCallable) void UpdateCapacityCharge(float _currentCharge, float _maxCharge, float _minRequiredCharge);
	UFUNCTION(BlueprintCallable) void UpdateAlternativeShotCharge(const float _currentCharge, const float _maxCharge);
protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent) void UpdateAlternativeShotCharged();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent) void UpdateAlternativeShotSelected(EPrimaryShootType _shootType);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent) void UpdateAlternativeShotReset();
private:
	UFUNCTION(BlueprintCallable) void UpdateAlternativeShotTime(float _remainingTime, float _totalTime);
	UFUNCTION(BlueprintCallable) void UpdateEnemyDetection(const bool _hit);
	
	UFUNCTION(BlueprintCallable) void SetCrosshair(UTexture2D* _crosshairTexture, const FVector2D& _position);
	
	//Boss
	UFUNCTION(BlueprintCallable) void BossFightStart();
	UFUNCTION(BlueprintCallable) void BossFightEnd();	

#pragma region Content Visibility
public:
	UFUNCTION() void ShowInteractionWidget();
	UFUNCTION() void HideInteractionWidget();

	UFUNCTION() void ShowMaxLifeBar();
	UFUNCTION() void HideMaxLifeBar();	
	
private:
	void UpdateLifeVisibility(const float _deltaTime);
	
	void ShowLifeContent() const;
	void HideLifeContent() const;

	void ShowEnergyContent() const;
	void HideEnergyContent() const;
	
	void ShowExtraLifeContent();
#pragma endregion 

#pragma region Content Display
	void UpdateLifeSize(const FVector2D& _size, bool _updateLife, bool _updateMaxLife, bool _updateDeco);
	
	UFUNCTION() void UpdateDisplay(const US_PlayerSave* _playerSave);

	UFUNCTION(BlueprintCallable) void SetSizeCrosshair(float _size);
	UFUNCTION(BlueprintCallable) void SetSizeHUD(float _size);
#pragma endregion 
	
	void UpdateFPS(const float _deltaTime);
	void UpdateTime();

	virtual void InitializeKeyboardVisual() override;
	virtual void InitializeGamepadVisual() override;
};