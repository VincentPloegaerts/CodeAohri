#include "IU_PunchingBall.h"
#include "IU_PlayerArrow.h"
#include "M_GameMode.h"
#include "M_UIManager.h"
#include "B_Boss.h"
#include "UI_DamageFloatingWidget.h"


AIU_PunchingBall::AIU_PunchingBall()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Graphics"));
	mycollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));

	healthComp = CreateDefaultSubobject<UIU_HealthSystemComponent>(TEXT("HealthComp"));
	AddOwnedComponent(healthComp);
	
	mycollider->SetupAttachment(RootComponent);
	staticMesh->SetupAttachment(RootComponent);
}

void AIU_PunchingBall::StartDamageCount(float _damage)
{
	canGlobalTimerIncrease = true;
	canCalculate = true;
	totalDamage += _damage;
	timeBeforeLastDamage = 0;
}

void AIU_PunchingBall::SetHarpoonData(int _iStackNeededToStun, float _fHarpoonStunDuration)
{
	if (!bossOwner) return;
	bossOwner->SetHarpoonData(_iStackNeededToStun, _fHarpoonStunDuration);
	bossOwner->CheckIfStunned();
}

void AIU_PunchingBall::ResetHarpoonStack()
{
	if (!bossOwner) return;
	bossOwner->ResetHarpoonStack();
}

void AIU_PunchingBall::BeginPlay()
{
	Super::BeginPlay();
	
	if (healthComp)
	{
		healthComp->OnDeath()->AddDynamic(this, &AIU_PunchingBall::DestroyPunchingBall);
	}
	
	//OnActorBeginOverlap.AddDynamic(this,&AIU_PunchingBall::OnOverlap);
}

void AIU_PunchingBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	IncreaseTimer();
	IncreaseGlobalTimer();
}

// void AIU_PunchingBall::OnOverlap(AActor* _me, AActor* _other)
// {
// 	// canGlobalTimerIncrease = true;
// 	// canCalculate = true;
// 	//
// 	// AIU_PlayerArrow* _arrow = Cast<AIU_PlayerArrow>(_other);
// 	//
// 	// if(!_arrow)
// 	// 	return;
// 	//
// 	// UE_LOG(LogTemp,Warning,TEXT("Arrow is : %s with %f damages"), *_arrow->GetFName().ToString(), _arrow->GetDamage());
// 	// totalDamage += _arrow->GetDamage();
// 	// timeBeforeLastDamage = 0;
// 	//
// 	// UE_LOG(LogTemp,Warning,(TEXT("tututu")));
// 	// Cast<AUI_DamageFloatingWidget>(GetWorld()->GetAuthGameMode<AM_GameMode>()->GetUIManager()->CreateFloatingWidget(FLOATING_WIDGET_DAMAGE, _arrow->GetActorLocation()))->SetType(_arrow->GetDamage(), HIT_BOSS);
// }

void AIU_PunchingBall::DestroyPunchingBall()
{
	UE_LOG(LogTemp, Warning, TEXT("DestroyPunchingBall"));
	Destroy();
}
void AIU_PunchingBall::IncreaseTimer()
{
	if(!canCalculate)return;
	timeBeforeLastDamage += GetWorld()->DeltaTimeSeconds;
	if(timeBeforeLastDamage > timeBeforeDpsCountStop)
	{
		UE_LOG(LogTemp, Error, TEXT("Total Damage : %f"),totalDamage);
		UE_LOG(LogTemp, Error, TEXT("%f DPS (on %f Seconds)"),totalDamage/(globalTimer - timeBeforeDpsCountStop),globalTimer - timeBeforeDpsCountStop);
		canCalculate = false;
		canGlobalTimerIncrease = false;
		timeBeforeLastDamage = 0;
		totalDamage = 0;
	}
}

void AIU_PunchingBall::IncreaseGlobalTimer()
{
	if(!canGlobalTimerIncrease || !canCalculate)return;
	globalTimer +=  GetWorld()->DeltaTimeSeconds;
	
}

