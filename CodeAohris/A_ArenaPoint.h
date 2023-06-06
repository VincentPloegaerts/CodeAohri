#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/BillboardComponent.h"

#include "A_ArenaPoint.generated.h"

UCLASS()
class INSIDEUS_API AA_ArenaPoint : public AActor
{
	GENERATED_BODY()

	
#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,meta = (AllowPrivateAccess = True))
	UBillboardComponent* billboard = nullptr;
#endif
	
	//The Current Arena owning this point.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,meta = (AllowPrivateAccess = True))
	AActor* arenaOwner = nullptr;

	//Allow to add a visible mesh for this Arena Point
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,meta = (AllowPrivateAccess = True))
	UStaticMeshComponent* staticMesh = nullptr;
	
public:	
	AA_ArenaPoint();

	UFUNCTION(BlueprintCallable) void SetArenaOwner(AActor* _arena);
};
