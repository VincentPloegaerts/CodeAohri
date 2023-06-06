// Fill out your copyright notice in the Description page of Project Settings.


#include "GPE_ArenaExit.h"

#include "B_Boss.h"
#include "IU_HealthSystemComponent.h"

AGPE_ArenaExit::AGPE_ArenaExit()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	damageBox = CreateDefaultSubobject<UBoxComponent>("Damage Box");
	damageBox->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
	blockingBox = CreateDefaultSubobject<UBoxComponent>("Blocking Box");
	blockingBox->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
}

void AGPE_ArenaExit::BeginPlay()
{
	Super::BeginPlay();
	damageBox->OnComponentBeginOverlap.AddDynamic(this,&AGPE_ArenaExit::OnCollision);
	damageBox->OnComponentEndOverlap.AddDynamic(this,&AGPE_ArenaExit::OnExitCollision);
	if (bossRef)
		bossRef->GetHealthComponent()->OnDeath()->AddDynamic(this,&AGPE_ArenaExit::Deactivate);
}

void AGPE_ArenaExit::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AGPE_ArenaExit::OnCollision(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep,
                                 const FHitResult& _sweepResult)
{
	if (targetHealth)return;
	if (const AIU_Player* _player = Cast<AIU_Player>(_otherActor))
	{
		targetHealth = _player->HealthComponent();
		targetHealth->TakeDamage(damageFirstEnter);
		FTimerDelegate _delegate;
		_delegate.BindUFunction(this,"DamageTarget",targetHealth,damagePerDuration);
		GetWorld()->GetTimerManager().SetTimer(timer,_delegate,delayDamages,true,-1);
	}
		
}

void AGPE_ArenaExit::OnExitCollision(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex)
{
	if (!targetHealth)return;
	if (Cast<AIU_Player>(_otherActor))
	{
		targetHealth = nullptr;
		GetWorld()->GetTimerManager().ClearTimer(timer);
	}
}

void AGPE_ArenaExit::DamageTarget(UIU_HealthSystemComponent* _health, const float& _damage)
{
	if (_health)
		_health->TakeDamage(_damage);
}

void AGPE_ArenaExit::Deactivate()
{
	damageBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	blockingBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorTickEnabled(false);
	onDeactivate.Broadcast();
}
