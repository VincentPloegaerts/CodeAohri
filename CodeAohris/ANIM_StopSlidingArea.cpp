// Fill out your copyright notice in the Description page of Project Settings.


#include "ANIM_StopSlidingArea.h"
#include "IU_PlayerMovementComponent.h"
#include "IU_Player.h"
#include "HUD_Main.h"
#include "UI_PauseMenuWidget.h"

// Sets default values
AANIM_StopSlidingArea::AANIM_StopSlidingArea()
{
	PrimaryActorTick.bCanEverTick = true;

	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	boxCollider->SetupAttachment(RootComponent);

}

void AANIM_StopSlidingArea::BeginPlay()
{
	Super::BeginPlay();
	boxCollider->OnComponentBeginOverlap.AddDynamic(this,&AANIM_StopSlidingArea::OnOverlapBegin);
}

void AANIM_StopSlidingArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AANIM_StopSlidingArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
		
	AIU_Player* _player = Cast<AIU_Player>(OtherActor);
	if(!_player)return;

	_player->MovementComponent()->SetIsSliding(false);
	_player->MovementComponent()->OnSlidingEnd().Broadcast();

	AHUD_Main* _hud = Cast<AHUD_Main>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (_hud)
	{
		_hud->GetWidget<UUI_PauseMenuWidget>(WIDGET_PAUSE)->UnlockButtons();
	}

	UC_PlayerAnimInstance* _playerAnimInst = Cast<UC_PlayerAnimInstance>(_player->GetSkeletalMesh()->GetAnimInstance());
	if(!_playerAnimInst)return;

	_playerAnimInst->UpdateIsSliding(false);

}



