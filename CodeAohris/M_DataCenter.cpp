#include "M_DataCenter.h"

AM_DataCenter::AM_DataCenter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AM_DataCenter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Init();
}

void AM_DataCenter::Init()
{
	TArray<FName> _rowNames = projectileDataTable->GetRowNames();
	// projectileDataTable->ForeachRow(_rowNames[0], );
	// projectileDatas.OnDataTableChanged(projectileDataTable, _rowNames[0]);

	// Si la data table change
	projectileDataTable->OnDataTableChanged().AddUFunction(this, FName("UpdateProjectilesDatas"));
}

void AM_DataCenter::UpdateProjectilesDatas()
{
	const int _count = projectileDatas.ballisticTypes.Num();
	for (int _index(0); _index < _count; _index++)
	{
		AP_ProjectileBallistic* _ballistic = projectileDatas.ballisticTypes[_index].GetDefaultObject();
		if (!_ballistic) continue;
		//_ballistic->UpdateDatas(projectileDatas.ballisticDatas);
	}
}

// les objets qui possèdent les structs les mettent à jours

////////////////////
////////////////////

// check tous les fichiers
// compare les noms
// si identiques
// stoque le BP
// Cast<BP.GetDefaultObject>().struct = rowName.Value;
