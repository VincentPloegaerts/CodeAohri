#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "A_DT_ArenaDataTable.generated.h"

UCLASS()
class INSIDEUS_API UA_DT_ArenaDataTable : public UDataAsset
{
	GENERATED_BODY()
		
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		TArray<FString> arenaPointNames;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = True))
		TArray<FString> arenaSliceNames;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TArray<FString> arenaMovementPointNames;
	
public:
	FORCEINLINE TArray<FString> GetPointNames() const { return arenaPointNames; }
	FORCEINLINE TArray<FString> GetSliceNames() const { return arenaSliceNames; }
	FORCEINLINE TArray<FString> GetMovementPointNames() const { return arenaMovementPointNames; }

	TArray<FString> GetArenaSpawnPoint() const;

	UFUNCTION(BlueprintCallable) void SetPointNames(const TArray<FString>& _names);
	UFUNCTION(BlueprintCallable) void SetSliceNames(const TArray<FString>& _names);
	UFUNCTION(BlueprintCallable) void SetMovementPointNames(const TArray<FString>& _names);
};
