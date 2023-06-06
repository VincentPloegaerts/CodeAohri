// Fill out your copyright notice in the Description page of Project Settings.


#include "GPE_Breakableplatform.h"

AGPE_BreakablePlatform::AGPE_BreakablePlatform()
{
	RootComponent = staticMesh;
	meshCollider = CreateDefaultSubobject<UStaticMeshComponent>("MeshCollider");
	meshCollider->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
}

void AGPE_BreakablePlatform::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void AGPE_BreakablePlatform::HitPlatform(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
	UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	if (staticMesh->GetCollisionResponseToChannel(ECC_Destructible) == ECollisionResponse::ECR_Overlap)
		return;
	GetWorld()->GetTimerManager().SetTimer(timerHandle,this,&AGPE_BreakablePlatform::DestroyPlatform,timeBeforeDestruction,false,-1);
}

void AGPE_BreakablePlatform::DestroyPlatform()
{
	staticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible,ECollisionResponse::ECR_Overlap);
	onPlatformBreaks.Broadcast();
	if (shouldReconstruct)
		GetWorld()->GetTimerManager().SetTimer(timerHandle,this,&AGPE_BreakablePlatform::ReformPlatform,timeBeforeReconstruction,false,-1);
}

void AGPE_BreakablePlatform::ReformPlatform()
{
	staticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible,ECollisionResponse::ECR_Block);
	//staticMesh->SetCollisionEnabled(ECollisionEnabled::);
	onPlatformReconstruct.Broadcast();
}

void AGPE_BreakablePlatform::Init()
{
	meshCollider->OnComponentBeginOverlap.AddDynamic(this,&AGPE_BreakablePlatform::HitPlatform);	
}
