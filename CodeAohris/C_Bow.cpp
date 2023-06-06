


#include "C_Bow.h"

AC_Bow::AC_Bow()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	bowSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("bowSkeletalMesh"));
	bowSkeletalMesh->SetupAttachment(RootComponent);
	

}

void AC_Bow::BeginPlay()
{
	Super::BeginPlay();
	
}

void AC_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

