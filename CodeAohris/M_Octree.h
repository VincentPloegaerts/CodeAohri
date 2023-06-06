#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "M_Octree.generated.h"

class UBillboardComponent;

UCLASS()
class INSIDEUS_API AM_Octree : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
		bool bIsHiddingActors = false;
		
	UPROPERTY(EditAnywhere, Category = "Octree values | Debug")
		bool bShowDebug = true;

	UPROPERTY(EditAnywhere, Category = "Octree values | Debug")
		float fThickness = 200.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "Octree values")
		int iAccuracy = 1;

	UPROPERTY(EditAnywhere, Category = "Octree values", meta = (UIMin = "0", UIMax = "10000", ClampMin = "0", ClampMax = "10000"))
		int iCapacity = 50;

	UPROPERTY(EditAnywhere, Category = "Octree values | Debug")
		FColor debugColor = FColor::Cyan;
	
	UPROPERTY(EditAnywhere, Category = "Octree values")
		FBox box = FBox();
	
	UPROPERTY()
		AM_Octree* master = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Octree values")
		TSubclassOf<AM_Octree> octreeType = TSubclassOf<AM_Octree>();
	
	UPROPERTY()
		TArray<AM_Octree*> octreeChildren = TArray<AM_Octree*>();

	UPROPERTY()
		TArray<AM_Octree*> octreesWithoutChildren = TArray<AM_Octree*>();

	UPROPERTY()
		TArray<TSoftObjectPtr<AActor>> actors = TArray<TSoftObjectPtr<AActor>>();
		
	UPROPERTY(EditAnywhere, Category = "Octree values")
		TArray<TSoftObjectPtr<AActor>> actorsToIgnore = TArray<TSoftObjectPtr<AActor>>();
	
public:
	FORCEINLINE void Setup(const int& _accuracy, const FBox& _boundingBox, AM_Octree* _master, const TArray<TSoftObjectPtr<AActor>>& _actors)
	{
		iAccuracy = _accuracy;
		box = _boundingBox;
		master = _master;
		actors = _actors;
	}
	FORCEINLINE void AddOctreeWithoutChildren(AM_Octree* _octree) { octreesWithoutChildren.Add(_octree); }
	FORCEINLINE bool HasChildren() const { return octreeChildren.Num() > 0; }
	FORCEINLINE const FBox& GetBox() const { return box; }
	FORCEINLINE const TArray<AM_Octree*>& GetChildren() const { return octreeChildren; }
	FORCEINLINE const TArray<AM_Octree*>& GetOctreesWithoutChildren() const { return octreesWithoutChildren; }
	
public:
	AM_Octree();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual bool ShouldTickIfViewportsOnly() const override;
	
	void DrawOctrees() const;
	UFUNCTION(CallInEditor, Category = "Octree values") void Update();
	UFUNCTION(CallInEditor, Category = "Octree values") void SwitchVisibility();
	UFUNCTION(CallInEditor, Category = "Octree values") void Clear();
	void GetActors();
	void SpawnChildren();
	void ResetOctree();
	
public:
	void ChangeActorsVisibility(bool _visible);
};