#include "BT_BlackBoard.h"

#include "BT_TN_CastPatternSerie.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

#include "BT_KeyNames.h"
#include "IU_Macros.h"
/*
const TArray<FName> UBT_BlackBoard::GetBoolKeyNames() const
{
	TArray<FName> _names;
	const int& _max = Keys.Num();
	for	(int i = 0; i < _max; i++)
	{
		const FBlackboardEntry& _key = Keys[i];
		if (Cast<UBlackboardKeyType_Bool>(_key.KeyType))
			_names.Add(_key.EntryName);
	}
	return _names;
}
*/
#if WITH_EDITOR
void UBT_BlackBoard::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetFName() == FName("behaviorTree"))
	{
		InitSaveAndLoadSystem();
	}
}
#endif
/*
#pragma region BlackBoard Keys Initialisation
void UBT_BlackBoard::InitializeBlackBoardKeys()
{
	TArray<FName> _keyNames;
	TMap<FName, bool> _keys = defaultKey;
	_keys.GetKeys(_keyNames);
	
	const int& _max = Keys.Num();
	for (int i = 0; i < _max; i++)
	{
		const FBlackboardEntry& _key = Keys[i];
		const FName& _name = _key.EntryName;
		
		if (_keyNames.Contains(_name))
			_keys[_name] = false;
	}
	
	if (_keys[KEY_BOSS]) AddKeyObject(KEY_BOSS);
	if (_keys[KEY_PLAYER]) AddKeyObject(KEY_PLAYER);
	if (_keys[KEY_STUN]) AddKeyBool(KEY_STUN);

	if (_keys[KEY_EXECUTE_ONCE_1]) AddKeyBool(KEY_EXECUTE_ONCE_1);
	if (_keys[KEY_EXECUTE_ONCE_2]) AddKeyBool(KEY_EXECUTE_ONCE_2);
	if (_keys[KEY_EXECUTE_ONCE_3]) AddKeyBool(KEY_EXECUTE_ONCE_3);
	if (_keys[KEY_EXECUTE_ONCE_4]) AddKeyBool(KEY_EXECUTE_ONCE_4);
	if (_keys[KEY_EXECUTE_ONCE_5]) AddKeyBool(KEY_EXECUTE_ONCE_5);
	
	if (_keys[KEY_ISMOVING]) AddKeyBool(KEY_ISMOVING);
	if (_keys[KEY_NODEWILLWAITFORENDOFMOVEMENT]) AddKeyBool(KEY_NODEWILLWAITFORENDOFMOVEMENT);
	
	UpdateParentKeys();
}

void UBT_BlackBoard::ClearKeys()
{
	Keys.Empty();
	UpdateParentKeys();
}

void UBT_BlackBoard::AddKeyObject(FName _keyName)
{
	FBlackboardEntry _key;
	_key.EntryName = _keyName;
	_key.KeyType = NewObject<UBlackboardKeyType_Object>();
	Keys.Add(_key);
}
void UBT_BlackBoard::AddKeyBool(FName _keyName)
{
	FBlackboardEntry _key;
	_key.EntryName = _keyName;
	_key.KeyType = NewObject<UBlackboardKeyType_Bool>();
	Keys.Add(_key);
}
#pragma endregion 
*/
#pragma region BT Save System
#if WITH_EDITOR
void UBT_BlackBoard::InitSaveAndLoadSystem()
{
	if (!behaviorTree)
	{
		behaviorTreeSaveName.Empty();
		behaviorTreeFolder.Empty();
		return;
	}
		
	behaviorTreeSaveName = behaviorTree->GetFName().ToString();
		
	const FString& _contentFolder = FPaths::ProjectContentDir();
	FString _BTFolder = behaviorTree->GetPathName();
	_BTFolder.RemoveFromStart("/Game/");
	_BTFolder.RemoveFromEnd(behaviorTreeSaveName + FString(".") + behaviorTreeSaveName);
	behaviorTreeFolder = FPaths::Combine(_contentFolder, _BTFolder);
}

void UBT_BlackBoard::SaveNodes() const
{
	FString _nodesStr;
	FPlatformMisc::ClipboardPaste(_nodesStr);

	if (!SaveToFile(_nodesStr))
		UE_LOG(LogTemp, Error, TEXT("Fail to Save Clipboard in File (Check Name & Folder)"));		
}
void UBT_BlackBoard::LoadNodes() const
{
	FString _nodesStr;

	if (LoadFromFile(_nodesStr))
		FPlatformMisc::ClipboardCopy(*_nodesStr);		
	else
		UE_LOG(LogTemp, Error, TEXT("Fail to Save Clipboard in File (Check Name & Folder"));		
}

FString UBT_BlackBoard::GetDirectory() const
{
	return behaviorTreeFolder;
}
FString UBT_BlackBoard::GetFileName() const
{
	return behaviorTreeSaveName + FString(".txt");
}

bool UBT_BlackBoard::SaveToFile(const FString& _nodesStr) const
{
	const FString& _directory = GetDirectory();
	const FString& _fileName = GetFileName();

	return FFileHelper::SaveStringToFile(_nodesStr, *(_directory / _fileName));
}
bool UBT_BlackBoard::LoadFromFile(FString& _nodesStr) const
{
	const FString& _directory = GetDirectory();
	const FString& _fileName = GetFileName();

	return FFileHelper::LoadFileToString(_nodesStr, *(_directory + _fileName));
}
#endif
#pragma endregion
/*
#pragma  region BT PatternData Generation
void UBT_BlackBoard::GenerateAllData()
{
	if (!behaviorTree)
	{
		LOG("UBT_BlackBoard::GenerateAllData -> invalid behavior tree");
		return;
	}

	const TObjectPtr<UBTCompositeNode>& _rootNode = behaviorTree->RootNode;

	if (!_rootNode.Get())return;
	
	const TArray<FBTCompositeChild>& _children = _rootNode->Children;
	
	const uint& _max = _children.Num();
	if (_max < 1)
	{
		LOG("UBT_BlackBoard::GenerateAllData -> there is no nodes");
		return;
	}
	
	for (uint i(0); i < _max; ++i)
	{
		const FBTCompositeChild& _compositeChild = _children[i];
		GenerateCompositeChild(_compositeChild.ChildComposite, _compositeChild.ChildTask);
	}
}
void UBT_BlackBoard::GenerateCompositeChild(const TObjectPtr<UBTCompositeNode>& _childComposite, const TObjectPtr<UBTTaskNode>& _nodeComposite)
{
	if (UBT_TN_CastPatternSerie* _serie = Cast<UBT_TN_CastPatternSerie>(_nodeComposite))
	{
		_serie->GenerateSerieData();
	}
	
	if (!_childComposite.Get())
	{
		return;
	}
	
	const TArray<FBTCompositeChild>& _children = _childComposite->Children;
	const uint& _max = _children.Num();
	
	if (_max < 1)
	{
		LOG("UBT_BlackBoard::GenerateCompositeChild -> there is no nodes");
		return;
	}
	
	for (uint i(0); i < _max; ++i)
	{
		const FBTCompositeChild& _compositeChild = _children[i];
		GenerateCompositeChild(_compositeChild.ChildComposite, _compositeChild.ChildTask);
	}
}
#pragma endregion 
#endif*/