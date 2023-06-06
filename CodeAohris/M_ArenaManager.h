#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "M_ArenaManager.generated.h"

class AA_Arena;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArenaSetted, AA_Arena*, _arena);

UCLASS()
class INSIDEUS_API AM_ArenaManager : public AActor
{
	GENERATED_BODY()
	
	/*The current arena used to setup pattern spawn.
	 * -In future update, will be replaced by a TMap<ECurrentBossPhase, AA_Arena*>-*/
	UPROPERTY(EditAnywhere)
		AA_Arena* arena = nullptr;

	UPROPERTY()
		FOnArenaSetted onArenaSetted = FOnArenaSetted();
	
public:	
	AM_ArenaManager();

	/* Set the current Arena */
	FORCEINLINE void SetArena(AA_Arena* _arena)
	{
		arena = _arena;
		onArenaSetted.Broadcast(arena);
	}
	FORCEINLINE FOnArenaSetted* OnArenaSetted() { return &onArenaSetted; }
	
	/* Get the current Arena */
	FORCEINLINE AA_Arena* GetArena() const { return arena; } 
};