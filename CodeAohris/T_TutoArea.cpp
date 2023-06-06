#include "T_TutoArea.h"

#include "IU_Player.h"

#include "HUD_Game.h"
#include "UI_TutoWidget.h"

#include "IU_Macros.h"

AT_TutoArea::AT_TutoArea()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root Component");
	
	tutoCollider = CreateDefaultSubobject<UBoxComponent>("Tuto Collider");
	tutoCollider->SetupAttachment(RootComponent);

	slowCollider = CreateDefaultSubobject<UBoxComponent>("Slow Collider");
	slowCollider->SetupAttachment(RootComponent);
}

void AT_TutoArea::BeginPlay()
{
	Super::BeginPlay();

	tutoCollider->OnComponentBeginOverlap.AddUniqueDynamic(this, &AT_TutoArea::OnTutoBeginOverlap);
	tutoCollider->OnComponentEndOverlap.AddUniqueDynamic(this, &AT_TutoArea::OnTutoEndOverlap);
	slowCollider->OnComponentBeginOverlap.AddUniqueDynamic(this, &AT_TutoArea::OnSlowBeginOverlap);
	slowCollider->OnComponentEndOverlap.AddUniqueDynamic(this, &AT_TutoArea::OnSlowEndOverlap);
	
	if (const APlayerController* _controller = GetWorld()->GetFirstPlayerController())
	{
		hud = _controller->GetHUD<AHUD_Game>();
		if (hud)
			tutoWidget = hud->GetWidget<UUI_TutoWidget>(WIDGET_TUTO);

		player = Cast<AIU_Player>(_controller->GetPawn());
	}

	if (player)
		OnInitializeTutorial(player);
}
void AT_TutoArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	#if WITH_EDITOR
	DrawTutoAreaDebug();
	#endif
	
	if (!slowStarted) return;
	
	currentSlow = FMath::Lerp(currentSlow, maxSlow, DeltaTime * slowForce);
	player->ApplySlow(1 - currentSlow);	
}

void AT_TutoArea::OnTutoBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (player == OtherActor)
	{
		StartTutorial();
		ShowTutoWidget();
	}
}
void AT_TutoArea::OnTutoEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (player == OtherActor)
	{
		StopTutorial();
		HideTutoWidget();
	}
}

void AT_TutoArea::OnSlowBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (player == OtherActor)
		StartSlow();
}
void AT_TutoArea::OnSlowEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (player == OtherActor)
		StopSlow();
}

void AT_TutoArea::StartTutorial()
{
	tutoStarted = true;
}
void AT_TutoArea::StopTutorial()
{
	tutoStarted = false;

	HideTutoWidget();
	StopSlow();
}
void AT_TutoArea::CompleteTutorial()
{
	if (!tutoStarted) return;
	
	StopTutorial();
	Destroy();
}

void AT_TutoArea::ShowTutoWidget() const
{
	if (tutoWidget)
		tutoWidget->InitializeTutoWidget(tutoElementWidgetType);
	if (hud)	
		hud->ShowGameWidget(WIDGET_TUTO);
}
void AT_TutoArea::HideTutoWidget() const
{
	if (hud)	
		hud->HideGameWidget(WIDGET_TUTO);
}

void AT_TutoArea::StartSlow()
{
	slowStarted = true;
	currentSlow = initialSlow;
}
void AT_TutoArea::StopSlow()
{
	slowStarted = false;
	player->ApplySlow(1);
}

void AT_TutoArea::BindActionAsCondition(const FInputActionData& _inputAction, TEnumAsByte<EInputEvent> _inputEventType)
{
	if (!player) return;
	player->InputComponent->BindAction(_inputAction.ActionName, _inputEventType, this, &AT_TutoArea::CompleteTutorial);
}

#if WITH_EDITOR
void AT_TutoArea::DrawTutoAreaDebug()
{
	const FColor& _tutColor = tutoStarted ? FColor::Blue : FColor::Red;
	const FColor& _slowColor = slowStarted ? FColor::Green : FColor::Red;
	DrawDebugBox(GetWorld(), tutoCollider->GetComponentLocation(), tutoCollider->GetScaledBoxExtent(), tutoCollider->GetComponentQuat(), _tutColor, false, -1, 0, 2);
	DrawDebugBox(GetWorld(), slowCollider->GetComponentLocation(), slowCollider->GetScaledBoxExtent(), slowCollider->GetComponentQuat(), _slowColor, false, -1, 0, 2);
}
#endif