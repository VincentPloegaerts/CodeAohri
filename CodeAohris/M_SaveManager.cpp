#include "M_SaveManager.h"

void UM_SaveManager::Save(const FString& _dataJson, const ESaveType& _saveType, const FString& _fileName)
{
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

bool UM_SaveManager::Load(FString& _jsonData, const ESaveType& _saveType, const FString& _fileName)
{
	// Load the file text from the environment
	const FString& _saveTypeFolderName = *UEnum::GetValueAsString<ESaveType>(_saveType);
	const FString& _saveFileFolder = CreateDirectoryIfNotExist(_saveTypeFolderName / _fileName);
		
	IPlatformFile& _platform = FPlatformFileManager::Get().GetPlatformFile();
	TArray<FString> _files;
	_platform.FindFiles(_files, *_saveFileFolder, *extension);
	if (_files.IsEmpty()) return false;
		
	FFileHelper::LoadFileToString(_jsonData, *_files.Last());
	
	return true;
}