#pragma once

#include "CoreMinimal.h"
#include "UI_UserWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "UI_GameSaveWidget.generated.h"

UCLASS()
class INSIDEUS_API UUI_GameSaveWidget : public UUI_UserWidget
{
	GENERATED_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameSelected, const int&, _index);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameDeleted);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* gameButton = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* deleteButton = nullptr;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* gameText = nullptr;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* gameInfoText= nullptr;
	
private:
	UPROPERTY(EditAnywhere, Category = "Settings | Confirm Box")
	FString confirmTitle = "DELETE SAVE ?";
	UPROPERTY(EditAnywhere, Category = "Settings | Confirm Box")
	FString confirmText = "This action can not be undone";
	
	UPROPERTY()
	int iGameIndex = 0;
	UPROPERTY()
	bool bDisableEmpty = false;
	
	UPROPERTY()
	FOnGameSelected onGameSelected;
	UPROPERTY()
	FOnGameDeleted onGameDeleted;

public:
	FORCEINLINE FOnGameSelected& OnGameSelected() { return onGameSelected; }
	FORCEINLINE FOnGameDeleted& OnGameDeleted() { return onGameDeleted; }

public:
	void InitializeGameSaveWidget(const int& _index, const class US_GameSave* _gameSave, const bool& _bDisableEmpty = false);

	virtual void InitializeFocus() override;
	
	UFUNCTION() void OnSelected();
	UFUNCTION() void DeleteSave();
	
private:
	virtual void InitializeWidget() override;

	UFUNCTION() void ConfirmDeleteSave();
};
