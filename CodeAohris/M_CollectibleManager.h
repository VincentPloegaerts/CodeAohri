#pragma once

#include "CoreMinimal.h"
#include "C_Collectible.h"
#include "GameFramework/Actor.h"
#include "M_CollectibleManager.generated.h"

UCLASS()
class INSIDEUS_API AM_CollectibleManager : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category="Collectible Manager")
		TArray<AC_Collectible*> collectibles = TArray<AC_Collectible*>();

public:
	FORCEINLINE TArray<AC_Collectible*> GetCollectibles() {return collectibles;}
	FORCEINLINE int GetCollectibleCount() {return collectibles.Num();}
	
public:	
	AM_CollectibleManager();

	UFUNCTION(BlueprintCallable) void RegisterToManager(AC_Collectible* _collectible);

};
