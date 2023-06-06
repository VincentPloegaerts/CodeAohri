#pragma once
#include "CoreMinimal.h"

#include "A_ArenaPoint.h"
#include "A_ArenaSlice.h"
#include "ENUM_BossPhase.h"

#include "GameFramework/Actor.h"
#include "A_Arena.generated.h"

class UBillboardComponent;
class UBoxComponent;
class USphereComponent;

class UA_DT_ArenaDataTable;
class AP_PoolingTrigger;

class AA_ArenaBlock;

UCLASS()
class INSIDEUS_API AA_Arena : public AActor
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	
	UPROPERTY(VisibleAnywhere)
		UBillboardComponent* billboard = nullptr;

	/*ArenaDataTable used to store the name of all ArenaPoint.*/
	UPROPERTY(EditAnywhere, Category = "ARENA TOOL")
		UA_DT_ArenaDataTable* arenaData = nullptr;

#pragma region TOOL_Point
	
	/*The current ArenaPoint that will be spawned.*/
	UPROPERTY(EditAnywhere, Category = "ARENA TOOL")
		TSubclassOf<AA_ArenaPoint> arenaPointType = TSubclassOf<AA_ArenaPoint>();

	/*The Name of the next new ArenaPoint.*/
	UPROPERTY(EditAnywhere, Category = "ARENA TOOL")
		FString pointName = FString("Arena_Point");
	
	/*Used to compare with new ArenaPoint array witch has to be deleted.*/
	UPROPERTY()
		TMap<FString, AA_ArenaPoint*> oldArenaPoints = TMap<FString, AA_ArenaPoint*>();
	
	UPROPERTY()
		TMap<FString, AA_ArenaPoint*> oldArenaMovementPoints = TMap<FString, AA_ArenaPoint*>();

#pragma endregion

#pragma region TOOL_Slice
	
	/*The current ArenaSlice that will be spawned.*/
	UPROPERTY(EditAnywhere, Category = "ARENA TOOL")
		TSubclassOf<AA_ArenaSlice> arenaSliceType = TSubclassOf<AA_ArenaSlice>();

	/*The Name of the next new ArenaSlice.*/
	UPROPERTY(EditAnywhere, Category = "ARENA TOOL")
		FString sliceName = FString("Arena_Slice");
	
	/*Used to compare with new ArenaSlice array witch has to be deleted.*/
	UPROPERTY()
		TMap<FString, AA_ArenaSlice*> oldArenaSlices = TMap<FString, AA_ArenaSlice*>();

#pragma endregion

#pragma region TOOL_Block
	
	/*The current ArenaBlock that will be spawned.*/
	UPROPERTY(EditAnywhere, Category = "ARENA TOOL")
		TSubclassOf<AA_ArenaBlock> arenaBlockType = TSubclassOf<AA_ArenaBlock>();

	/*The Name of the next new ArenaBlock.*/
	UPROPERTY(EditAnywhere, Category = "ARENA TOOL")
		FString blockName = FString("Arena_Block");

#pragma endregion

#endif
	
	#pragma region Components
	
	/*Static Mesh for the Arena Actor.*/
	UPROPERTY(VisibleAnywhere, Category = "Arena | Components")
		UStaticMeshComponent* arenaMesh = nullptr;

	#pragma endregion

	#pragma region Pooling
	
	UPROPERTY(EditAnywhere, Category = "Arena | Pooling")
		AP_PoolingTrigger* poolingTrigger = nullptr;

	#pragma endregion

	#pragma region Points
	
	/*All ArenaPoints currently stored in the Arena.*/
	UPROPERTY(EditAnywhere, Category = "Arena | Points")
		TMap<FString, AA_ArenaPoint*> arenaPoints = TMap<FString, AA_ArenaPoint*>();
	
	/*All MovementPoints currently stored in the Arena.*/
	UPROPERTY(EditAnywhere, Category = "Arena | Points")
		TMap<FString, AA_ArenaPoint*> movementPoints = TMap<FString, AA_ArenaPoint*>();

	#pragma endregion

	#pragma region Slices
	
	/*All ArenaSlices currently stored in the Arena.*/
	UPROPERTY(EditAnywhere, Category = "Arena | Slices")
		TMap<FString, AA_ArenaSlice*> arenaSlices = TMap<FString, AA_ArenaSlice*>();

	#pragma endregion

	#pragma region Block
	
	/*Arena Block of the arena.*/
	UPROPERTY(EditAnywhere, Category = "Arena | Block")
		AA_ArenaBlock* arenaBlock = nullptr;

	#pragma endregion

	#pragma region Events
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArenaOpen);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "FOnArenaOpen"))
	FOnArenaOpen onArenaOpen = FOnArenaOpen();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArenaClose);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "FOnArenaClose"))
	FOnArenaClose onArenaClose = FOnArenaClose();


	#pragma endregion
	
	#pragma region Getters
	
public:
	FORCEINLINE AA_ArenaBlock* GetArenaBlock() const { return arenaBlock; }
	FORCEINLINE const TMap<FString, AA_ArenaPoint*>* GetMovementPoints() const { return &movementPoints; }

	/*Return the ArenaPoint witch name is corresponding. Can be nullptr.*/
	FORCEINLINE const AActor* GetArenaPoint(const FString& _name) const
	{
		if (!arenaPoints.Contains(_name))
			return GetArenaSlice(_name);
		return arenaPoints.FindChecked(_name);
	}
	
	/*Return one of all ArenaPoint stored. If no ArenaPoint, will return nullptr.*/
	FORCEINLINE const AActor* GetRandomArenaPoint() const
	{
		TArray<FString> _keys;
		arenaPoints.GetKeys(_keys);
		const int& _max = _keys.Num();
	
		if (_max == 0) return nullptr;

		const int& _random = FMath::RandRange(0, _max);
		return arenaPoints.FindChecked(_keys[_random]);
	}

	/*Return the ArenaSlice witch name is corresponding. Can be nullptr.*/
	FORCEINLINE const AA_ArenaSlice* GetArenaSlice(const FString& _name) const
	{
		if (!arenaSlices.Contains(_name)) return nullptr;
		return arenaSlices.FindChecked(_name);
	}
	
	/*Return the ArenaSlice witch name is corresponding. Can be nullptr.*/
	FORCEINLINE TArray<AA_ArenaSlice*> GetArenaSlices(const TArray<FString>& _names) const
	{
		TArray<AA_ArenaSlice*> _slices;
		const int& _max = _names.Num();
		for (int i = 0; i < _max; ++i)
		{
			const FString& _name = _names[i];
			if (arenaSlices.Contains(_name))
				_slices.Add(arenaSlices.FindChecked(_name));
		}
		return _slices;
	}
	
	/*Return true if an ArenaSlice have the player on it an set info (Name/Ptr). false otherwise.*/
	FORCEINLINE AA_ArenaSlice* GetArenaSliceWithPlayerPresence() const
	{
		for (const auto& _slice : arenaSlices)
			if (_slice.Value->GetPlayerPresence())
				return _slice.Value;
	
		return nullptr;
	}

	FORCEINLINE FOnArenaOpen& OnArenaOpen() { return onArenaOpen; }
	FORCEINLINE FOnArenaClose& OnArenaClose() { return onArenaClose; }


	#pragma endregion

private:
	UPROPERTY(EditAnywhere, Category = "Boss Phase")
	TEnumAsByte<EBossPhase> bossPhase = EBossPhase::UN;

	UPROPERTY()
	float deathAnimationTimer = 0;
	UPROPERTY()
	bool canPlayDeathAnimation = false;
	

public:
	FORCEINLINE const TEnumAsByte<EBossPhase>& GetArenaBossPhase() const { return bossPhase; }
	
public:
	AA_Arena();

private:
	virtual void BeginPlay() override;
	void InitializeArena();
	void InitializeArenaBlock();
	void InitializePoolingTrigger();
	void RegisterToArenaManager();
	
	//For Death Animation
	void SetPlayerAnimInstanceIsInArena(bool _status);
	virtual void Tick(float DeltaSeconds) override;

public:
	/*Call by Boss event when the Player die or Boss reset.*/
	UFUNCTION(BlueprintCallable) void OpenArena();
	
	/*Call by Boss event when the Boss is activated.*/
	UFUNCTION(BlueprintCallable) void CloseArena();

	
	UFUNCTION(BlueprintCallable) void BeginOpenArena();

	#if WITH_EDITOR
private:
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	/*Spawn a new ArenaPoint with pointName.*/
	UFUNCTION(CallInEditor, Category = "ARENA TOOL") void SpawnArenaPoint();
	/*Spawn a new ArenaSlice with sliceName.*/
	UFUNCTION(CallInEditor, Category = "ARENA TOOL") void SpawnArenaSlice();
	/*Spawn a new ArenaBlock with blockName.*/
	UFUNCTION(CallInEditor, Category = "ARENA TOOL") void SpawnArenaBlock();
	/*Spawn a new Arena Movement Point with pointName.*/
	UFUNCTION(CallInEditor, Category = "ARENA TOOL") void SpawnArenaMovementPoint();

	template<class ClassType>
	ClassType* SpawnAndAttach(TSubclassOf<ClassType>& _type, const FString& _name)
	{
		const FVector& _location = GetActorLocation();
		ClassType* _spawned = GetWorld()->SpawnActor<ClassType>(_type, _location, FRotator(0));
		_spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		_spawned->SetActorLabel(_name);
		return _spawned;
	}
	
	/*Refresh all Points & Slices data.*/
	UFUNCTION(CallInEditor, Category = "ARENA TOOL") void RefreshArenaData() const;

#endif
};