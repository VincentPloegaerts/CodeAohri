#pragma once
#include "CoreMinimal.h"
#include "UI_UserWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "STRUCT_LeaderboardEntry.h"

#include "UI_ExtraDataWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_ExtraDataWidget : public UUI_UserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* userImage = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* userNameText = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    	UTextBlock* userRankText = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* timeText = nullptr;
public:
	void InitializeExtraDataWidget(const LeaderboardEntry_t& _entry);

private:
	UTexture2D* GetSteamImageAsTexture(int _imageHandle);
};
