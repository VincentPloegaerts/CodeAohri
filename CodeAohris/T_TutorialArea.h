#pragma once
#include "CoreMinimal.h"

#include "GameFramework/InputSettings.h"
#include "Components/BoxComponent.h"
#include "IU_Player.h"
#include "T_TutorialElement.h"

#include "GameFramework/Actor.h"
#include "T_TutorialArea.generated.h"

UCLASS()
class INSIDEUS_API AT_TutorialArea : public AActor
{
	GENERATED_BODY()

	#pragma region Areas
	
	UPROPERTY(EditAnywhere, Category = "Tutorial area values | Areas")
		UBoxComponent* widgetArea = nullptr;

	UPROPERTY(EditAnywhere, Category = "Tutorial area values | Areas")
		UBoxComponent* slowArea = nullptr;

	#pragma endregion

	#pragma region SlowFactor

	UPROPERTY(VisibleAnywhere, Category = "Tutorial area values | SlowFactor")
		bool bCanIncreaseSlowFactor = false;

	UPROPERTY(VisibleAnywhere, Category = "Tutorial area values | SlowFactor")
		float fCurrentSlowFactor = 100.0f;
	
	UPROPERTY(EditAnywhere, Category = "Tutorial area values | SlowFactor", meta = (UIMin = "0.001", UIMax = "100", ClampMin = "0.001", ClampMax = "100.0"))
		float fAdditionalSlowFactor = 0.001f;

	UPROPERTY(EditAnywhere, Category = "Tutorial area values | SlowFactor", meta = (UIMin = "0.01", UIMax = "100.0", ClampMin = "0.01", ClampMax = "100.0"))
		float fMinSlowFactor = 0.3f;

	#pragma endregion

	#pragma region Elements

	UPROPERTY(VisibleAnywhere, Category = "Tutorial area values | Elements")
		bool canSkip = true;
	
	UPROPERTY(VisibleAnywhere, Category = "Tutorial area values | Elements")
		bool bAllElementsAreDone = false;
	
	UPROPERTY(VisibleAnywhere, Category = "Tutorial area values | Elements")
		int iCurrentElement = 0;
	
	UPROPERTY(EditAnywhere, Category = "Tutorial area values | Elements")
		TArray<UT_TutorialElement*> allElements = TArray<UT_TutorialElement*>();

	UPROPERTY()
		UT_TutorialElement* currentElement = nullptr;
		
	UPROPERTY()
		UUserWidget* currentWidget = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Tutorial area values | Elements")
		UT_AdditionalCondition* condition = nullptr;

	#pragma endregion

	#pragma region Pointers
	
	UPROPERTY()
		UInputSettings* inputSettings = nullptr;
	
	UPROPERTY()
		AIU_Player* player = nullptr;

	#pragma endregion
	
public:	
	AT_TutorialArea();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable) void OnActorEnterInWidgetArea(UPrimitiveComponent* _this, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _index, bool _sweep, const FHitResult& _result);
	UFUNCTION(BlueprintCallable) void OnActorEnterInSlowArea(UPrimitiveComponent* _this, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _index, bool _sweep, const FHitResult& _result);
	UFUNCTION(BlueprintCallable) void OnActorLeaveSlowArea(UPrimitiveComponent* _this, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _index);
	
	void ExecuteElement();
	UFUNCTION(BlueprintCallable) void StopTutorial();
	
	void UpdateCurrentElement();
	void UpdateInputs(bool _toAdd) const;
	void IncreaseSlowFactor();
};