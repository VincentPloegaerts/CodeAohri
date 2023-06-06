#pragma once

#include "CoreMinimal.h"
#include "C_InteractionWidgetComponent.h"
#include "GPE_ChestMiniGame.h"
#include "Components/BoxComponent.h"
#include "GPE_ChestAnimInstance.h"
#include "GameFramework/Actor.h"
#include "GPE_Chest.generated.h"

UCLASS()
class INSIDEUS_API AGPE_Chest : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	USkeletalMeshComponent* skeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	UGPE_ChestMiniGame* miniGame = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	UGPE_ChestAnimInstance* chestAnimInstance = nullptr;

	UPROPERTY(EditAnywhere, Category = "Settings", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float maxHealthToAdd = 10;
	UPROPERTY(EditAnywhere, Category = "Settings", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
	float maxSlowMotionToAdd = 10;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True),Category = "Anim TP location")
	USceneComponent* animTPLocation = nullptr;
	
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = True))
	class AIU_Player* player = nullptr;
	UPROPERTY()
	class UUI_PlayerWidget* playerWidget = nullptr;
	UPROPERTY()
	bool isWidgetHidden = true;
	UPROPERTY()
	bool miniGameStarted = false;
	UPROPERTY()
	bool active = true;
	UPROPERTY()
	bool isAlreadyOpen = false;
	UPROPERTY()
	bool canInteract = true;
	UPROPERTY()
	float playerInteractionRange = 0;

	UPROPERTY()
	FTimerHandle timerDistance;
	
public:
	FORCEINLINE USkeletalMeshComponent* GetSkeletalMesh() const {return skeletalMesh;}
	FORCEINLINE UGPE_ChestMiniGame* GetChestMinigame() const {return miniGame;}
	FORCEINLINE UGPE_ChestAnimInstance* GetChestAnimInstance() const {return chestAnimInstance;}
	FORCEINLINE AIU_Player* GetPlayer() const {return player;}
	FORCEINLINE void SetCanInteract(bool _status) {canInteract =  _status;}
	FORCEINLINE bool GetCanInteract() const {return canInteract;}
	FORCEINLINE USceneComponent* GetAnimTPLocation() const {return animTPLocation;}
	
public:	
	AGPE_Chest();

	void StartChestMiniGame();
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION() void InitializeChest();
	
	UFUNCTION() void MiniGameFinished(bool _success);

	void MiniGameSucceed();
	void MiniGameFailed();

	UFUNCTION() void CheckPlayerInteractionRange();
};
