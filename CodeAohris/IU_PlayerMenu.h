#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "IU_PlayerMenu.generated.h"

class ACam_CameraMenu;
class AS_SaveActor;

class AHUD_Main;
class UUI_SaveMenuWidget;

UCLASS()
class INSIDEUS_API AIU_PlayerMenu : public APawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Player Menu | Settings")
	ACam_CameraMenu* camera = nullptr;
	UPROPERTY(EditAnywhere, Category = "Player Menu | Settings")
	TArray<AS_SaveActor*> saveActors = { };
	UPROPERTY(EditAnywhere, Category = "Player Menu | Settings")
	TEnumAsByte<EObjectTypeQuery> saveActorLayer;
	UPROPERTY(EditAnywhere, Category = "Player Menu | Settings", meta = (ClampMin = "0.01"))
	float checkRate = 0.05f;
	
	UPROPERTY(VisibleAnywhere, Category = "Player Menu | Values")
	bool saveSelection = false;
	UPROPERTY(VisibleAnywhere, Category = "Player Menu | Values")
	int saveIndex = -1;
	
	UPROPERTY()
	UUI_SaveMenuWidget* saveMenuWidget = nullptr;
	UPROPERTY()
	AHUD_Main* hud = nullptr;
	UPROPERTY()
	APlayerController* playerController = nullptr;

	FTimerManager* TimerManager = nullptr;
	FTimerHandle checkTimer;
	
public:
	FORCEINLINE bool IsPlayerMenuValid() const { return hud && camera && playerController; }
	
public:
	AIU_PlayerMenu();

private:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION() void InitializePlayerMenu();

	UFUNCTION() void EnableCursorTrace();
	UFUNCTION() void DisableCursorTrace();
	
	UFUNCTION() void BeginDelayedSaveSelection();
	UFUNCTION() void InitializeSaveSelection();
	UFUNCTION() void TerminateSaveSelection();
	
	UFUNCTION() void NextSave();
	UFUNCTION() void PreviousSave();
	UFUNCTION() void SelectSave();

	UFUNCTION() void CheckCursor();
	void HoverSaveActor(AS_SaveActor* _saveActor);

	void InverseLocation(FVector& _location);

	UFUNCTION() void DeleteHoveredSave();	
};