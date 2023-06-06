#pragma once
#include "CoreMinimal.h"

#include "Components/Image.h"

#include "Blueprint/UserWidget.h"

#include "UI_HitPlayer.generated.h"

UCLASS()
class INSIDEUS_API UUI_HitPlayer : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Hit Player")
		UImage* left = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Hit Player")
		UImage* right = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Hit Player")
		UImage* top = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Hit Player")
		UImage* bottom = nullptr;

private:
	UPROPERTY(EditAnywhere, Category = "Hit Player | Time Values")
		float fDecayTime = 5.0f;
	
	UPROPERTY()
		float fInternalDecayTime = 0.0f;
	
	UPROPERTY(EditAnywhere, Category = "Hit Player | Time Values")
		float fDecayingTime = 1.0f;
	
	UPROPERTY()
		float fInternalDecayingTime = 0.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "Hit Player | Time Values")
		bool bCanDecay = false;
	
	UPROPERTY(VisibleAnywhere, Category = "Hit Player | Time Values")
		bool bIsDecaying = false;
	
	UPROPERTY(EditAnywhere, Category = "Hit Player | Width Values")
		float fTotalMaxWidth = 50.0f;
	
	UPROPERTY(EditAnywhere, Category = "Hit Player | Width Values")
		int iTotalMaxNumberOfSection = 5;
	
	UPROPERTY(VisibleAnywhere, Category = "Hit Player | Width Values")
		int iCurrentNumberOfSection = 0;
	
	UPROPERTY()
		int iLastNumberOfSection = 0;

private:
	FORCEINLINE float GetCurrentWidth() {return fTotalMaxWidth * (static_cast<float>(iCurrentNumberOfSection) / static_cast<float>(iTotalMaxNumberOfSection));}
	FORCEINLINE float GetLastWidth() {return fTotalMaxWidth * (static_cast<float>(iLastNumberOfSection) / static_cast<float>(iTotalMaxNumberOfSection));}
	
private:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	void UpdateAllSize(const float _size);
	void UpdateImageSize(const UImage* _image, FVector2D _size);
	void Decay();
	void Decaying();

public:
	UFUNCTION(BlueprintCallable) void Hit(float _null = 0.0f);
	UFUNCTION(BlueprintCallable) void Reset();
	
};
