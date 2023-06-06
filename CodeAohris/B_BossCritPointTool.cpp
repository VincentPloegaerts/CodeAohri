#include "B_BossCritPointTool.h"

#include "B_Boss.h"
#include "B_BossCritPoint.h"
#include "B_DT_CritPointDataTable.h"
#include "IU_Macros.h"

#include "Engine/SkeletalMeshSocket.h"

AB_BossCritPointTool::AB_BossCritPointTool()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AB_BossCritPointTool::BeginPlay()
{
	Super::BeginPlay();
	
	Destroy();
}
void AB_BossCritPointTool::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	#if WITH_EDITOR
	if (autoSave)
	{
		elapsedTimeAutoSave += DeltaSeconds;
		if (elapsedTimeAutoSave > autoSaveTime)
		{
			elapsedTimeAutoSave = 0;
			SaveCritPoint();
		}
	}
	#endif
}
void AB_BossCritPointTool::Destroyed()
{
	#if WITH_EDITOR
	if (autoSave)
		SaveCritPoint();
	#endif	
	
	ClearCritPoint();
	
	Super::Destroyed();
}

TArray<FName> AB_BossCritPointTool::GetSocketNames() const
{
	if (!boss) return { "NO BOSS" };

	if (const USkeletalMeshComponent* _mesh = Cast<USkeletalMeshComponent>(boss->GetComponentByClass(USkeletalMeshComponent::StaticClass())))
	{
		if (const USkeletalMesh* _skeletal = _mesh->GetSkeletalMeshAsset())
		{
			const TArray<USkeletalMeshSocket*>& _socket =  _skeletal->GetActiveSocketList();
			const int _max = _socket.Num();
			if (_max == 0) return { "NO SOCKET" };
			
			TArray<FName> _names;
			for (int i = 0; i < _max; i++)
				_names.Add(_socket[i]->SocketName);

			return _names;
		}
		return { "NO SKELETAL" };
	}
	return { "NO SKELETAL MESH" };
}

void AB_BossCritPointTool::CreateCritPoint()
{
	if (!boss) {
		LOG_ERROR("Crit Point Tool -> Failed to Create Crit Point (Boss is null).")
		return;
	}
	USkeletalMeshComponent* _mesh = Cast<USkeletalMeshComponent>(boss->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	if (!_mesh)	{
		LOG_ERROR("Crit Point Tool -> Failed to Create Crit Point (No Skeletal Mesh found).")
		return;
	}
	if (!IsSocketValid(socketName))	{
		LOG_ERROR("Crit Point Tool -> Failed to Create Crit Point (Socket Name not valid).")
		return;
	}
	if (!critPointType.Get()) {
		LOG_ERROR("Crit Point Tool -> Failed to Create Crit Point (Crit Point Type is null).")
		return;
	}
	
	if (AB_BossCritPoint* _spawned = GetWorld()->SpawnActor<AB_BossCritPoint>(critPointType, GetActorLocation(), GetActorRotation()))
	{
		_mesh->GetSocketByName(socketName)->AttachActor(_spawned, _mesh);
#if WITH_EDITOR
		_spawned->SetActorLabel(critPointName);
#endif
		critPoints.Add(_spawned, FCritPointData(socketName, critPointType));
	}

	if (autoSave)
		SaveCritPoint();
}
void AB_BossCritPointTool::SaveCritPoint()
{
	UB_DT_CritPointDataTable* _critPointDataTable = boss->GetCritPointDataTable();
	if (!_critPointDataTable)
	{
		LOG_ERROR("Crit Point Tool -> Failed to Save Crit Points (No Crit Point Data Table in Boss).")
		return;
	}
	
	TArray<FCritPointData> _critPointData = { };
	
	for (const auto& _current : critPoints)
	{
		if (const AB_BossCritPoint* _critPoint = _current.Key)
		{
			FCritPointData _data = _current.Value;
			FCritPointSettings _settings;
			_critPoint->GetCritPointSettings(_settings);
		
			_data.SetCritPointSettings(_settings);
#if WITH_EDITOR
			_data.SetActorName(_critPoint->GetActorLabel());
#endif
		
			_critPointData.Add(_data);
		}
	}
	if (_critPointData.Num() == 0) return;
	
	_critPointDataTable->SetCritPointData(_critPointData);
	LOG("Crit Point Tool -> Crit Points saved.")
}
void AB_BossCritPointTool::LoadCritPoint()
{
	ClearCritPoint();
	
	if (!boss)
	{
		LOG_ERROR("Crit Point Tool -> Failed to Load Crit Point (Boss is null).")
		return;
	}
	const UB_DT_CritPointDataTable* _critPointDataTable = boss->GetCritPointDataTable();
	if (!_critPointDataTable)
	{
		LOG_ERROR("Crit Point Tool -> Failed to Load Crit Point (No Crit Point Data Table in Boss).")
		return;
	}
	USkeletalMeshComponent* _mesh = Cast<USkeletalMeshComponent>(boss->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	if (!_mesh)
	{
		LOG_ERROR("Crit Point Tool -> Failed to Load Crit Point (No Skeletal Mesh found).")
		return;
	}

	const TArray<FCritPointData>& _areasData = _critPointDataTable->GetCritPointData();
	const int _max = _areasData.Num();
	
	for (int i = 0; i < _max; i++)
	{
		const FCritPointData& _current = _areasData[i];
		const FName& _socketName = _current.GetSocketName();
		const TSubclassOf<AB_BossCritPoint>& _type = _current.GetCritPointType();
		if (!_type.Get() || !IsSocketValid(_socketName)) continue;

		if (AB_BossCritPoint* _spawned = GetWorld()->SpawnActor<AB_BossCritPoint>(_type, GetActorLocation(), GetActorRotation()))
		{
			_mesh->GetSocketByName(_socketName)->AttachActor(_spawned, _mesh);
			_spawned->SetCritPointSettings(_current.GetCritPointSettings());
#if WITH_EDITOR
			_spawned->SetActorLabel(_current.GetActorName());
#endif
			
			critPoints.Add(_spawned, FCritPointData(_socketName, _type, _current.GetCritPointSettings()));
		}	
	}

	LOG("Crit Point Tool -> Crit Points loaded.")
}
void AB_BossCritPointTool::ClearCritPoint()
{
	for (const auto& _current : critPoints)
	{
		if (AB_BossCritPoint* _critPoint = _current.Key)
			_critPoint->Destroy();
	}
	critPoints.Empty();
	LOG("Crit Point Tool -> Crit Points cleared.")
}
void AB_BossCritPointTool::ResetCritPoint()
{
	LoadCritPoint();
	
	if (autoSave)
		SaveCritPoint();

	LOG("Crit Point Tool -> Crit Points reset.")
}

#if WITH_EDITOR
void AB_BossCritPointTool::PreEditChange(FProperty* PropertyAboutToChange)
{
	Super::PreEditChange(PropertyAboutToChange);

	if (PropertyAboutToChange->GetName() == "critPoints")
	{
		oldCritPoints = critPoints;
	}
}
void AB_BossCritPointTool::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetName() == "critPoints")
	{
		CheckDeletedCritPoint();
	}

	if (PropertyChangedEvent.Property->GetName() == "autoSave")
	{
		elapsedTimeAutoSave = 0;
	}
}

void AB_BossCritPointTool::CheckDeletedCritPoint()
{
	if (oldCritPoints.Num() > critPoints.Num())	//Element deleted in array, Destroy Actor in scene is required
	{
		for (const auto& _current : oldCritPoints)
		{
			if (_current.Key && !critPoints.Contains(_current.Key))	//Valid CritPoint ptr & check if the current CritPoint is not in CritPoints => means it has been removed from array 
				_current.Key->Destroy();
		}
	}
	oldCritPoints.Empty();
		
	if (autoSave)
		SaveCritPoint();
}
#endif 
