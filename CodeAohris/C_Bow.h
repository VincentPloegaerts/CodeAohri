
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_Bow.generated.h"

UCLASS()
class INSIDEUS_API AC_Bow : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USkeletalMeshComponent* bowSkeletalMesh = nullptr;
public:	

	AC_Bow();


public :
	FORCEINLINE USkeletalMeshComponent* GetBowSkeletalMesh(){return bowSkeletalMesh;}
protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
