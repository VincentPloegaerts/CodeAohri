#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "STRUCT_ArenaCloserSettings.h"

#include "A_ArenaSlice.generated.h"

class AA_Arena;
class AIU_Player;
class UIU_HealthSystemComponent;

UCLASS()
class INSIDEUS_API AA_ArenaSlice : public AActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArenaSliceBeginClosure);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArenaSliceClose);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArenaSliceEndClosure, AA_ArenaSlice*, _arena);
	
	UPROPERTY(VisibleAnywhere, Category = "Arena Slice")
	AActor* arenaOwner = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Arena Slice | Components")
	UStaticMeshComponent* staticMesh = nullptr;
	//This collider will be used only to check if player is in arena slice (to take damaged)
	UPROPERTY(VisibleAnywhere, Category = "Arena Slice | Components")
	UStaticMeshComponent* staticMeshCollider = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Arena Slice | Settings")
	FSliceSettings sliceSettings;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Arena Slice | Debug")
	bool bDebug = true; 
#endif
	
	UPROPERTY()
	FOnArenaSliceBeginClosure onArenaSliceBeginClosure;
	UPROPERTY()
	FOnArenaSliceClose onArenaSliceClose;
	UPROPERTY()
	FOnArenaSliceEndClosure onArenaSliceEndClosure;
	
	FTimerManager* timerManager = nullptr;
	UPROPERTY()
	FTimerHandle timerClose;
	UPROPERTY()
	FTimerHandle timerOpen;
	
	UPROPERTY()
	AIU_Player* player = nullptr;
	UPROPERTY()
	UIU_HealthSystemComponent* healthComponent = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Arena Slice | Values")
	bool bPlayerPresence = false;
	UPROPERTY(VisibleAnywhere, Category = "Arena Slice | Values")
	bool bClose = false;
	
public:
	FORCEINLINE FOnArenaSliceBeginClosure& OnBeginClosure() { return onArenaSliceBeginClosure; }
	FORCEINLINE FOnArenaSliceClose& OnClose() { return onArenaSliceClose; }
	FORCEINLINE FOnArenaSliceEndClosure& OnEndClosure() { return onArenaSliceEndClosure; }

	FORCEINLINE const bool& GetPlayerPresence() const { return bPlayerPresence; }
	
public:	
	AA_ArenaSlice();

	UFUNCTION() void SetArenaOwner(AActor* _arena);
	
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	//Start Closure Process (Overlap collision enable)
	void InitializeArenaSlice(const FSliceSettings& _settings);
private:
	//Initialize Slice at Begin Play
	void InitializeSlice();
	//Close the Slice (Player will take damage from now)
	void CloseArenaSlice();
	//End Closure Process (Overlap collision disable)
	void OpenArenaSlice();

	UFUNCTION() void ArenaSliceOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION() void ArenaSliceOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

#if WITH_EDITOR
	void DebugArenaSlice(FColor _color, float _time);
	void DebugArenaSlicePlayerPresence();
#endif
};
