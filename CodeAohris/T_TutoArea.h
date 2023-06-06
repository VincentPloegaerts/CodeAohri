#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "STRUCT_GameInput.h"
#include "STRUCT_InputData.h"

#include "Components/BoxComponent.h"

#include "T_TutoArea.generated.h"

class UUI_TutoElementWidget;
class AIU_Player;

UCLASS()
class INSIDEUS_API AT_TutoArea : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Components")
	UBoxComponent* tutoCollider = nullptr;
	UPROPERTY(EditAnywhere, Category = "Components")
	UBoxComponent* slowCollider = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Tuto Area | Widget Settings")
	TSubclassOf<UUI_TutoElementWidget> tutoElementWidgetType;
	UPROPERTY(EditAnywhere, Category = "Tuto Area | Tutorial", meta = (UIMin = 0, UIMax = 1))
	float initialSlow = 0.2;
	UPROPERTY(EditAnywhere, Category = "Tuto Area | Tutorial", meta = (UIMin = 0, UIMax = 1))
	float maxSlow = 1;
	UPROPERTY(EditAnywhere, Category = "Tuto Area | Tutorial", meta = (UIMin = 0, UIMax = 5))
	float slowForce = 1;
	
	UPROPERTY()
	bool tutoStarted = false;
	UPROPERTY()
	bool slowStarted = false;
	UPROPERTY()
	float currentSlow = 0;
	
	UPROPERTY()
	class AHUD_Game* hud = nullptr;
	UPROPERTY()
	class UUI_TutoWidget* tutoWidget = nullptr;
	UPROPERTY()
	AIU_Player* player = nullptr;
	
public:	
	AT_TutoArea();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION() void OnTutoBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnTutoEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION() void OnSlowBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnSlowEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent) void OnInitializeTutorial(AIU_Player* _player);
	void StartTutorial();
	void StopTutorial();
	UFUNCTION(BlueprintCallable) void CompleteTutorial();

	void ShowTutoWidget() const;
	void HideTutoWidget() const;
	void StartSlow();
	void StopSlow();
	
	UFUNCTION(BlueprintCallable) void BindActionAsCondition(const FInputActionData& _inputAction, TEnumAsByte<EInputEvent> _inputEventType);

#if WITH_EDITOR
	void DrawTutoAreaDebug();
#endif
};
