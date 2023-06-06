#include "S_SaveActor.h"

#include "M_GameInstance.h"

#include "UI_GameSaveWidget.h"

#include "IU_Macros.h"

AS_SaveActor::AS_SaveActor()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root Component");

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	staticMesh->SetupAttachment(RootComponent);

	light = CreateDefaultSubobject<USpotLightComponent>("Point Light");
	light->SetupAttachment(RootComponent);

	widget = CreateDefaultSubobject<UWidgetComponent>("Widget");
	widget->SetupAttachment(RootComponent);
}

void AS_SaveActor::InitializeSaveActor(const int _index)
{
	index = _index;

	if (const UM_GameInstance* _gi = GetWorld()->GetGameInstance<UM_GameInstance>())
	{
		gameSave = _gi->GetGameSaveByIndex(index);

		if (!gameSaveWidget) InitGameSaveWidget();
		if (gameSaveWidget)
			gameSaveWidget->InitializeGameSaveWidget(index, gameSave);
	}
}

void AS_SaveActor::HoverSave()
{
	light->SetVisibility(true);
	staticMesh->SetMaterial(0, hoverMaterial);
	widget->SetHiddenInGame(false);	

	if (!gameSave)
		InitializeSaveActor(index);
	
	//...
}
void AS_SaveActor::UnhoverSave()
{
	light->SetVisibility(false);
	staticMesh->SetMaterial(0, gameSave ? normalSaveMaterial : normalEmptyMaterial);
	widget->SetHiddenInGame(true);
	
	//...	
}

void AS_SaveActor::ClearSaveAppearance()
{
	staticMesh->SetMaterial(0, normalEmptyMaterial);
}

void AS_SaveActor::SelectSave()
{
	//...	
}

void AS_SaveActor::DeleteSave()
{
	if (gameSaveWidget)
	{
		gameSaveWidget->DeleteSave();
		gameSaveWidget->InitializeGameSaveWidget(index, nullptr);	
	}
}

void AS_SaveActor::BeginPlay()
{
	Super::BeginPlay();
	
	InitGameSaveWidget();		
	
	UnhoverSave();
}

void AS_SaveActor::InitGameSaveWidget()
{
	if (gameSaveWidget) return;
	
	gameSaveWidget = Cast<UUI_GameSaveWidget>(widget->GetWidget());
	if (gameSaveWidget)
		gameSaveWidget->OnGameDeleted().AddUniqueDynamic(this, &AS_SaveActor::ClearSave);
}

void AS_SaveActor::ClearSave()
{
	gameSave = nullptr;
	UnhoverSave();
}
