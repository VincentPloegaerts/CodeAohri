#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

#include "BT_KeyNames.h"

#include "BT_BlackBoard.generated.h"

class UA_DT_ArenaDataTable;
class AB_Boss;

UCLASS()
class INSIDEUS_API UBT_BlackBoard : public UBlackboardData
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
protected:
	UPROPERTY(EditAnywhere, Category = "Arena")
	UA_DT_ArenaDataTable* arenaDataTable;
	UPROPERTY(EditAnywhere, Category = "Skeletal Mesh")
	USkeletalMesh* skeletalMesh = nullptr;
/*
	UPROPERTY()
	TMap<FName, bool> defaultKey
	{
		{KEY_BOSS, true},
		{KEY_PLAYER, true},
		{KEY_STUN, true},
		{KEY_EXECUTE_ONCE_1, true},
		{KEY_EXECUTE_ONCE_2, true},
		{KEY_EXECUTE_ONCE_3, true},
		{KEY_EXECUTE_ONCE_4, true},
		{KEY_EXECUTE_ONCE_5, true},
		{KEY_ISMOVING, true},
		{KEY_NODEWILLWAITFORENDOFMOVEMENT, true}
	};
*/
#pragma region BT Save System
	UPROPERTY(EditAnywhere, Category = "Save")
	UBehaviorTree* behaviorTree = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Save")
	FString behaviorTreeFolder = FString("None");
	UPROPERTY(EditAnywhere, Category = "Save")
	FString behaviorTreeSaveName = FString("None");
#pragma endregion
#endif
/*
public:
	//Get all boolean key in BlackBoard
	const TArray<FName> GetBoolKeyNames() const;
*/
#if WITH_EDITOR
public:
	FORCEINLINE UA_DT_ArenaDataTable* GetArenaDataTable() const { return arenaDataTable; }
	FORCEINLINE USkeletalMesh* GetSkeletalMesh() const { return skeletalMesh; }
#endif
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;	
/*
#pragma region BlackBoard Keys Initialisation
public:
	//Allow to Initialize essential keys (Boss, Player, IsStun) and some optional keys to the BlackBoard (BlackBoard Window need to be re-open)
	UFUNCTION() void InitializeBlackBoardKeys();
	//Remove all Blackboad Keys
	UFUNCTION() void ClearKeys();
private:
	//Add an Object Key in the BlackBoard
	void AddKeyObject(FName _keyName);
	//Add a Boolean Key in the BlackBoard
	void AddKeyBool(FName _keyName);
#pragma endregion
	*/
#pragma region BT Save System
public:
	//Get Default FileName (based on BehaviorTree name) and Folder (Based on BehaviorTree location)
 	UFUNCTION(CallInEditor, Category = "Save", DisplayName = "Refresh") void InitSaveAndLoadSystem();

	//Save the Nodes data in Clipboard (Ctrl + C required in BehaviorTree Graph to Copy Nodes) into a file (Will create the file if it's required) 
	UFUNCTION(CallInEditor, Category = "Save") void SaveNodes() const;
	//Load the node into the Clipboard (Ctrl + V required in BehaviorTree Graph to paste Nodes)
	UFUNCTION(CallInEditor, Category = "Save") void LoadNodes() const;

	FString GetDirectory() const;
	FString GetFileName() const;

	//Save the given FString into a file
	bool SaveToFile(const FString& _nodesStr) const;
	//Load string into a file and set it in the given FString
	bool LoadFromFile(FString& _nodesStr) const;
#pragma endregion
/*
#pragma  region BT PatternData Generation
	//Generate all the pattern data inside the Behavior Tree (WARNING : Will erase every Override Data)
	UFUNCTION(CallInEditor, Category = "Data Generation", DisplayName = "Generate All Data (Will destroy every override)") void GenerateAllData();
	void GenerateCompositeChild(const TObjectPtr<UBTCompositeNode>& _childComposite, const TObjectPtr<UBTTaskNode>& _nodeComposite);
#pragma endregion */
#endif
};