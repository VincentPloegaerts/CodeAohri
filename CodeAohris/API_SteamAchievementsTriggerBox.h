
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"

#include "API_SteamAchievementsTriggerBox.generated.h"

UCLASS()
class INSIDEUS_API AAPI_SteamAchievementsTriggerBox : public AActor
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere)
		UBoxComponent* boxCollider = nullptr;

		UPROPERTY(EditAnywhere, Category = "Settings")
		bool isInDesert = false;

		UPROPERTY(EditAnywhere, Category = "Settings")
		bool isInTemple = false;


	
public:	
	AAPI_SteamAchievementsTriggerBox();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	


	UFUNCTION(BlueprintCallable) void OnActorOverlap(UPrimitiveComponent* _this, AActor* _otherActor, UPrimitiveComponent* _otherComp,
		int32 _index, bool _sweep, const FHitResult& _result);



};
