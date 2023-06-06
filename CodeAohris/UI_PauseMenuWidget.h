#pragma once
#include "CoreMinimal.h"
#include "UI_BackWidget.h"

#include "Components/Button.h"

#include "UI_PauseMenuWidget.generated.h"

UCLASS()
class INSIDEUS_API
UUI_PauseMenuWidget : public UUI_BackWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* checkpointButton = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* difficultyButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* extraButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* optionButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* menuButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* quitButton = nullptr;

	UPROPERTY()
	class AIU_Player* player = nullptr;

	UPROPERTY(EditAnywhere, Category = "Settings | Confim Widget")
	FString returnCheckpointTitle = "Return to Checkpoint";
	UPROPERTY(EditAnywhere, Category = "Settings | Confim Widget")
	FString returnCheckpointText = "This will restart the boss";	

public:
	UFUNCTION() void LockButtons();
	UFUNCTION() void UnlockButtons();
	
private:
	virtual void InitializeWidget() override;
	virtual void InitializeFocus() override;
	
	UFUNCTION(BlueprintCallable) void Resume();
	UFUNCTION(BlueprintCallable) void ReturnToCheckpoint();
	UFUNCTION(BlueprintCallable) void OpenDifficultyMenu();
	UFUNCTION(BlueprintCallable) void OpenOptionMenu();
	UFUNCTION(BlueprintCallable) void OpenExtraMenu();
	UFUNCTION(BlueprintCallable) void QuitMenu();
	UFUNCTION(BlueprintCallable) void QuitGame();

	UFUNCTION(BlueprintCallable) void ConfirmReturnToCheckpoint(); 

	UFUNCTION() void GetPlayer();
	
	virtual void Back() override;
};