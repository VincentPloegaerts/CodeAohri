#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SpotLightComponent.h"
#include "Components/WidgetComponent.h"

#include "S_SaveActor.generated.h"

class UUI_GameSaveWidget;

UCLASS()
class INSIDEUS_API AS_SaveActor : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* staticMesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpotLightComponent* light = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UWidgetComponent* widget = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Save Actor | Settings")
	UMaterialInterface* normalEmptyMaterial = nullptr;
	UPROPERTY(EditAnywhere, Category = "Save Actor | Settings")
	UMaterialInterface* normalSaveMaterial = nullptr;
	UPROPERTY(EditAnywhere, Category = "Save Actor | Settings")
	UMaterialInterface* hoverMaterial = nullptr;	

	UPROPERTY()
	int index = 0;
	UPROPERTY()
	class US_GameSave* gameSave = nullptr;
	UPROPERTY()
	UUI_GameSaveWidget* gameSaveWidget = nullptr;

public:
	FORCEINLINE const int& GetSaveIndex() const { return index; }
	
public:	
	AS_SaveActor();
	
	void InitializeSaveActor(const int _index);
	
	void HoverSave();
	void UnhoverSave();
	void ClearSaveAppearance();
	void SelectSave();
	void DeleteSave();
	
private:
	virtual void BeginPlay() override;

	void InitGameSaveWidget();

	UFUNCTION() void ClearSave();
};
