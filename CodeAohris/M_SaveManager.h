#pragma once
#include "CoreMinimal.h"

#if WITH_EDITOR
#include "ISourceControlModule.h"
#include "ISourceControlProvider.h"
#include "SourceControlOperations.h"
#endif

#include "ENUM_SaveType.h"
#include "JsonObjectConverter.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "M_SaveManager.generated.h"

UCLASS()
class INSIDEUS_API UM_SaveManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	#if !WITH_EDITOR
	inline static FString saveRoot = "GameSave";
	#endif
	#if WITH_EDITOR
	inline static FString saveRoot = "EditorSave";
	#endif
	
	inline static FString extension = ".json";

	static FORCEINLINE FString CreateDirectoryIfNotExist(const FString& _folder)
	{
		IPlatformFile& _platform = FPlatformFileManager::Get().GetPlatformFile();
		const FString& _path = GetFolderPath(_folder);
		if (!_platform.FileExists(*_path))
		{
			_platform.CreateDirectoryTree(*_path);
		}

		return _path;
	}
	static FORCEINLINE FString GetFolderPath(const FString& _folder)
	{
		FString _path;

		#if (UE_BUILD_SHIPPING || UE_BUILD_DEVELOPMENT)
		_path = FPaths::ProjectContentDir();
		#endif

		#if WITH_EDITOR
		_path = FPaths::ProjectDir();
		#endif

		return FString(_path / saveRoot / _folder);
	}
	static FORCEINLINE FString GetGlobalPath(const FString& _folder, const FString& _fileName)
	{
		return GetFolderPath(_folder) / _fileName + extension;
	}

public:
	#pragma region Save
	
	template<typename StructType>
	static void Save(const StructType& _object, const ESaveType& _saveType, const FString& _fileName)
	{
		// Convert datas into file text
		FString _dataJson = FString();
		FJsonObjectConverter::UStructToJsonObjectString<StructType>(_object, _dataJson);

		// Get the good environment
		const FString& _saveTypeFolderName = *UEnum::GetValueAsString<ESaveType>(_saveType);
		const FString& _saveFileFolder = CreateDirectoryIfNotExist(_saveTypeFolderName / _fileName);
		
		IPlatformFile& _platform = FPlatformFileManager::Get().GetPlatformFile();
		TArray<FString> _files;
		_platform.FindFiles(_files, *_saveFileFolder, *extension);
		const FString& _finalFileName = _saveFileFolder / _fileName + "_" + FString::FromInt(_files.Num()) + extension;
		
		// Save the file text in the environment
		FFileHelper::SaveStringToFile(_dataJson, *_finalFileName);

		#if WITH_EDITOR
		ISourceControlProvider& SourceControlProvider = ISourceControlModule::Get().GetProvider();
		ECommandResult::Type CommandResult = SourceControlProvider.Execute(ISourceControlOperation::Create<FMarkForAdd>(), { _finalFileName });
		#endif
	}
	static void Save(const FString& _dataJson, const ESaveType& _saveType, const FString& _fileName);

	#pragma endregion

	#pragma region Load

	//Load JsonData into the given struct. return if Load is a success (false otherwise)
	template<typename StructType>
	static bool Load(StructType& _object, const ESaveType& _saveType, const FString& _fileName)
	{
		// Load the file text from the environment
		FString _dataJson = FString();
		const FString& _saveTypeFolderName = *UEnum::GetValueAsString<ESaveType>(_saveType);
		const FString& _saveFileFolder = CreateDirectoryIfNotExist(_saveTypeFolderName / _fileName);
		
		IPlatformFile& _platform = FPlatformFileManager::Get().GetPlatformFile();
		TArray<FString> _files;
		_platform.FindFiles(_files, *_saveFileFolder, *extension);
		if (_files.IsEmpty()) return false;
		
		FFileHelper::LoadFileToString(_dataJson, *_files.Last());
		
		// Convert file text into datas
		FJsonObjectConverter::JsonObjectStringToUStruct<StructType>(_dataJson, &_object);

		return true;
	}
	//Load JsonData into the given string. return if Load is a success (false otherwise)
	static bool Load(FString& _jsonData, const ESaveType& _saveType, const FString& _fileName);

	#pragma endregion 
};