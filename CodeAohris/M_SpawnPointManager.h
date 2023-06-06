#pragma once

#include "CoreMinimal.h"
#include "ENUM_SpawnSettings.h"
#include "GameFramework/Actor.h"
#include "M_SpawnPointManager.generated.h"

class AM_ArenaManager;
class AM_GameMode;
class AB_Boss;

UCLASS()
class INSIDEUS_API AM_SpawnPointManager : public AActor
{
	GENERATED_BODY()
	
	/*Player taken in the gameMode after BeginPlay.*/
	UPROPERTY(VisibleAnywhere, Category = "Spawn Point")
	AActor* player;
	
	/*Boss taken in the gameMode after BeginPlay.*/
	UPROPERTY(VisibleAnywhere, Category = "Spawn Point")
	class AB_Boss * boss;

	/*Arena Manager taken in the gameMode after BeginPlay.*/
	UPROPERTY(VisibleAnywhere, Category = "Spawn Point")
	AM_ArenaManager* arenaManager;

	/*Optimize GetWorld().*/
	UPROPERTY()
	UWorld* world;

	/*Used to avoid redundant GetAuthGameMode().*/
	UPROPERTY()
	AM_GameMode* gameMode;

	/*Timer used for the Initialization of variables.*/
	UPROPERTY()
	FTimerHandle timer;

public:
	FORCEINLINE const AActor* GetPlayer() const { return player; }
	FORCEINLINE const AB_Boss * GetBoss() const { return boss; }

public:
	AM_SpawnPointManager();

protected:
	virtual void BeginPlay() override;

public:
	/*Get an Arena Point by giving him the name of the point.*/
	const AActor* GetArenaPoint(const FString& _pointName) const;
	/*Attach the given actor the socket name if exist (return success state)*/
	bool AttachToSocket(const FName& _socketName, AActor* _toAttach) const;
	/*Return the Socket location (if exist)*/
	FVector GetSocketLocation(const FName& _socketName) const;
	/*Get an AActor* following SpawnSettings parameters and use the PointName for Arena Point.*/
	const AActor* GetPoint(const ESpawnSettings& _spawnSettings, const FString& _pointName) const;
	
	FVector GetPointLocation(const ESpawnSettings& _spawnSettings, const FString& _pointName) const;

private:
	/*Initialize his parameters after the BeginPlay.*/
	UFUNCTION() void InitializeSpawnPoints();
	UFUNCTION() void ChangeCurrentBoss(AB_Boss* _boss);
	
};