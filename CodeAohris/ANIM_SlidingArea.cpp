

#include "ANIM_SlidingArea.h"
#include "IU_Player.h"
#include "C_PlayerAnimInstance.h"
#include "IU_PlayerMovementComponent.h"
#include "HUD_Main.h"
#include "UI_PauseMenuWidget.h"

AANIM_SlidingArea::AANIM_SlidingArea()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	boxCollider->SetupAttachment(RootComponent);
}

void AANIM_SlidingArea::BeginPlay()
{
	Super::BeginPlay();
	boxCollider->OnComponentBeginOverlap.AddDynamic(this,&AANIM_SlidingArea::OnOverlapBegin);

}

void AANIM_SlidingArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AANIM_SlidingArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	AIU_Player* _player = Cast<AIU_Player>(OtherActor);
	if(!_player)return;

	_player->MovementComponent()->SetIsSliding(true);
	_player->MovementComponent()->OnSliding().Broadcast();

	AHUD_Main* _hud = Cast<AHUD_Main>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (_hud)
	{
		_hud->GetWidget<UUI_PauseMenuWidget>(WIDGET_PAUSE)->LockButtons();
	}

	UC_PlayerAnimInstance* _playerAnimInst = Cast<UC_PlayerAnimInstance>(_player->GetSkeletalMesh()->GetAnimInstance());
	if(!_playerAnimInst)return;

	_playerAnimInst->UpdateIsSliding(true);

	
}



