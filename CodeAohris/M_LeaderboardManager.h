#pragma once

#include "CoreMinimal.h"
#include "API_SteamIntegration.h"
#include "GameFramework/Actor.h"
#include "M_LeaderboardManager.generated.h"

UCLASS()
class INSIDEUS_API AM_LeaderboardManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	UAPI_SteamIntegration* steamIntegration = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Leaderboard Manager | Values")
	class AB_Boss* boss = nullptr;
	
	UPROPERTY()
	float elapsedTime = 0;
	
public:	
	AM_LeaderboardManager();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION() void InitializeLeaderboardManager();

	UFUNCTION() void InitializeManagerFromBoss(AB_Boss* _boss);
	
	UFUNCTION() void StartTimer();
	UFUNCTION() void StopTimer();
	UFUNCTION() void RegisterTimer();
};
