// Fill out your copyright notice in the Description page of Project Settings.


#include "C_EnhancementOrb.h"
#include "Kismet/KismetMathLibrary.h"
#include "IU_Player.h"
#include "UC_PlayerAttackComponent.h"


AC_EnhancementOrb::AC_EnhancementOrb()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Graphics"));
	orbCollider = CreateDefaultSubobject<USphereComponent>(TEXT("HitCollider"));
	magnetOrbCollider = CreateDefaultSubobject<USphereComponent>(TEXT("MagnetOrbCollider"));

	staticMesh->SetupAttachment(RootComponent);
	orbCollider->SetupAttachment(RootComponent);
	magnetOrbCollider->SetupAttachment(RootComponent);
	
}


void AC_EnhancementOrb::BeginPlay()
{
	Super::BeginPlay();
	// orbCollider->OnComponentBeginOverlap.AddDynamic(this,&AC_EnhancementOrb::OnOverlap);
	// magnetOrbCollider->OnComponentBeginOverlap.AddDynamic(this,&AC_EnhancementOrb::OnMagnetOverlap);
	// if(orbBaseNiagaraComp)
	// {
	// 	UNiagaraComponent* _niag2Comp = UNiagaraFunctionLibrary::SpawnSystemAttached(orbBaseNiagaraComp,GetRootComponent(),NAME_None,
	// 								GetRootComponent()->GetComponentLocation() + FVector(0,0,fFXBaseLocationOffset),GetRootComponent()->GetComponentRotation(),EAttachLocation::KeepWorldPosition, true);
	//
	// }
}


void AC_EnhancementOrb::Tick(float DeltaTime)
{
	// Super::Tick(DeltaTime);
	// GoToPlayer();
	// IncreaseTimer();
	// MagnetMoveToPlayer();
	// SetHeight();

}

// void AC_EnhancementOrb::GoToPlayer()
// {
// 	SetHeight();
// 	if(bIsAtLocation)return;
// 	
//
// 	FVector _realnew =UKismetMathLibrary::VInterpTo_Constant(GetActorLocation(),_pointsArray[iIndex],GetWorld()->DeltaTimeSeconds, fSpeed);
// 	
// 	if(FVector::Dist(GetActorLocation(),vFinalLocation) < 0.8)
// 	{
// 		bIsAtLocation = true;
// 		bIsOrbGrounded = true;
// 		OnOrbHitGround();
// 		SetLifeSpan(fDurationBeforeDestruction);
// 		bCanBeginDestruction = true;
// 		iIndex = 0;
// 	}
//
// 	if(FVector::Dist(GetActorLocation(),_pointsArray[iIndex]) < 0.8)
// 		iIndex++;
// 	
// 	SetActorLocation(_realnew);
// }
//
// void AC_EnhancementOrb::IncreaseTimer()
// {
// 	if(!bCanBeginDestruction)return;
// 	
// 	fDestructionTimer += GetWorld()->DeltaTimeSeconds;
// 	if(fDestructionTimer > fDurationBeforeDestruction)
// 	{
// 		OnOrbDestroyed();
// 		Destroy();
// 	}
// 	
// }
//
// void AC_EnhancementOrb::MagnetMoveToPlayer()
// {
// 	if(!playerOwner || !bCanGoToPlayer || !bIsOrbGrounded)return;
// 	
// 	FVector _location = UKismetMathLibrary::VInterpTo_Constant(GetActorLocation(), playerOwner->GetActorLocation(), GetWorld()->DeltaTimeSeconds, fMagnetSpeed);
// 	SetActorLocation(_location);
// 	fMagnetSpeed += fMagnetSpeedIncrease;	
// }
//
// void AC_EnhancementOrb::SetHeight()
// {
// 	if(!bIsAtLocation || bisAtFinalLocation)return;
//
// 	FVector _pos = GetActorLocation();
// 	FVector _offset = FVector(0,0,fAdditionalHeight);
// 	SetActorLocation(_pos  + _offset);
// 	
// 	bisAtFinalLocation = true;
// }
//
// void AC_EnhancementOrb::CreateBezierCurve()
// {
// 	FVector _controlsPoints[4] {GetActorLocation(), GetActorLocation() + FVector(0,0,fMaxHeight),vFinalLocation + FVector(0,0,fMaxHeight),vFinalLocation };
//
// 	
// 	FVector::EvaluateBezier(_controlsPoints,iNumberOfPoints,_pointsArray);
// 	
// }
//
//
// void AC_EnhancementOrb::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
//
// 	AIU_Player* _player = Cast<AIU_Player>(OtherActor);
// 	if(!_player)return;
//
// 	//_player->GetCharacterAttackComponent()->IncreaseOrbNumber();
// 	//_player->GetCharacterAttackComponent()->ApplyOrbNumberEffect();
// 	_player->GetCharacterAttackComponent()->IncreasedCurrentShootingModeCount();
// 	_player->GetCharacterAttackComponent()->IncreasedCurrentSlowMoOrbCount();
// 	
// 	OnOrbPickedUp();
//
// 	if(orbPickedUpNiagaraComp)
// 	{
//
// 		FVector _playerLocation = _player->GetActorLocation();
// 		
// 		
// 		UNiagaraComponent* _niag2Comp = UNiagaraFunctionLibrary::SpawnSystemAttached(orbPickedUpNiagaraComp,OtherActor->GetRootComponent(),NAME_None,
// 										OtherActor->GetRootComponent()->GetComponentLocation() + FVector(0,0,fFXSpawnLocationOffset),OtherActor->GetRootComponent()->GetComponentRotation(),EAttachLocation::KeepWorldPosition, true);
//
// 		
// 	}
// 	
// 	
// 	Destroy();
// }
//
// void AC_EnhancementOrb::OnMagnetOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	
// 	if(AIU_Player* _chara = Cast<AIU_Player>(OtherActor))
// 	{
// 		bCanGoToPlayer = true;
// 		onMagnetBegin.Broadcast(GetActorLocation(),_chara->GetActorLocation());
// 	}
//
// }
//
// void AC_EnhancementOrb::SetFinalLocation(FVector _newLocation)
// {
// 	bIsAtLocation = false;
// 	vFinalLocation = _newLocation;
// }

