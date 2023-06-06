#pragma once

#include "CoreMinimal.h"
#include "IU_Macros.h"
#include "GameFramework/Actor.h"
#include "M_FPSChecker.generated.h"

UCLASS()
class INSIDEUS_API AM_FPSChecker : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "FPS")
	bool bActivateGEngineLOG = true;

	UPROPERTY(EditAnywhere, Category = "FPS", meta = (ClampMin = "0.01"))
	float fStartDelay = 5.0f;

	UPROPERTY()
	bool bActivated = false;

	
	UPROPERTY(VisibleAnywhere, Category = "FPS")
	unsigned iFrameCount = 0;

	UPROPERTY(VisibleAnywhere, Category = "FPS")
	float fTotalTime = 0.0f;
	

	UPROPERTY(VisibleAnywhere, Category = "FPS")
	unsigned iFrameCountInSecond = 0;
	
	UPROPERTY(VisibleAnywhere, Category = "FPS")
	float fCurrentTimeInSecond = 0.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "FPS")
	float fMinimalFrameInSecond = 694201.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "FPS")
	float fAverageFrameInSecond = 0.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "FPS")
	float fMaximalFrameInSecond = 0.0f;


	UPROPERTY(VisibleAnywhere, Category = "FPS")
	unsigned iSecond = 0;
	
	UPROPERTY(VisibleAnywhere, Category = "FPS")
	float fTotalAverageFrame = 0.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "FPS")
	float fTotalFPS = 0.0f;
	
public:	
	AM_FPSChecker();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION() void ActivateTimer();
	
};