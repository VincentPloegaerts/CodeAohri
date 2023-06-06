#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "IU_Player.h"

#include "C_RespawnComponent.generated.h"

class US_GameSave;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INSIDEUS_API UC_RespawnComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFadeInFinish);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRespawn);

private:
	// Respawn duration
	UPROPERTY(EditAnywhere, Category = "Respawn | Settings", meta = (UIMin = "0.0", UIMax = "60.0", ClampMin = "0.0", ClampMax = "60.0"))
		float fRespawnDuration = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Respawn | Settings", meta = (UIMin = "0.0", UIMax = "60.0", ClampMin = "0.0", ClampMax = "60.0"))
		float fHoldTime = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Respawn | Settings", meta = (UIMin = "0.0", UIMax = "60.0", ClampMin = "0.0", ClampMax = "60.0"))
		float fAdditionalHoldTimeDeath = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Respawn | Settings")
		float fRespawnTime = 1.f;

	UPROPERTY(EditAnywhere, Category = "Respawn | Settings")
		float fArenaRespawnTime = 2.f;
	
	// Fade widget
	UPROPERTY(EditAnywhere, Category = "Respawn | Settings")
		TSubclassOf<UUserWidget> fadingWidgetType = TSubclassOf<UUserWidget>();

	// Death reason
	UPROPERTY(VisibleAnywhere, Category = "Respawn | Values")
		bool isDead = false;

	#pragma region Fading
	
	// Fading status 
	UPROPERTY(VisibleAnywhere, Category = "Respawn | Values")
		bool bIsFading = false;

	UPROPERTY(VisibleAnywhere, Category = "Respawn | Values")
		bool bIsHolding = false;
	
	// State of Fading (In or Out)
	UPROPERTY(VisibleAnywhere, Category = "Respawn | Values")
		bool bIsFadingIn = false;

	// Current fade time
	UPROPERTY(VisibleAnywhere, Category = "Respawn | Values")
		float fFadingProgression = 0.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "Respawn | Values")
		float fElapsedHoldingTime = 0.0f;

	#pragma endregion 

	#pragma region Pointers
	
	UPROPERTY()
		UUserWidget* fadingWidget = nullptr;
		
	UPROPERTY()
		AIU_Player* owner = nullptr;
	UPROPERTY()
		US_GameSave* gameSave = nullptr;

	#pragma endregion

	#pragma region Events

protected:
	UPROPERTY(BlueprintCallable,BlueprintAssignable, BlueprintReadWrite)
		FOnRespawn onRespawn;
	
	UPROPERTY(BlueprintCallable,BlueprintAssignable, BlueprintReadWrite)
		FOnRespawn onRespawnStart;
	
	UPROPERTY(BlueprintCallable,BlueprintAssignable, BlueprintReadWrite)
		FOnFadeInFinish onFadeInFinish;

	#pragma endregion
	
private:
	FORCEINLINE float GetFadingDuration() const { return fRespawnDuration / 2.0f; } 
	
public:


	FORCEINLINE FOnRespawn& OnRespawn() { return onRespawn; }
	FORCEINLINE FOnRespawn& OnRespawnStart() { return onRespawnStart; }
	FORCEINLINE FOnFadeInFinish& OnFadeInFinish() { return onFadeInFinish; }

	FORCEINLINE void SetIsFading() { bIsFading = true; }
	
public:	
	UC_RespawnComponent();

private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	//Call on Player Death (Health at 0 or when respawn needed)
	UFUNCTION(BlueprintCallable) void Death(bool _isDead);
	UFUNCTION(BlueprintCallable) void ToDeath();
	
private:
	UFUNCTION(BlueprintCallable) void Respawn();

	#pragma region Fading
	void InitializeFadingWidget();
	
	void UpdateFading(const float _deltaTime);
	void UpdateFadingIn(const float _deltaTime);
	void UpdateFadingOut(const float _deltaTime);
	void UpdateHolding(const float _deltaTime);

	void FadingStart();
	void FadingCompleted();
	
	void SetFadingWidgetOpacity(float _opacity) const;
	#pragma endregion 
};
