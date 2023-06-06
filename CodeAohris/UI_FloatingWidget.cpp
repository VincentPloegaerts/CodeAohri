#include "UI_FloatingWidget.h"

AUI_FloatingWidget::AUI_FloatingWidget()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	if (staticMesh)
	{
		staticMesh->SetupAttachment(RootComponent);
	}
	
	widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	if (widget)
	{
		widget->SetupAttachment(staticMesh);
	}
	
}
void AUI_FloatingWidget::BeginPlay()
{
	Super::BeginPlay();
	InitFloatingText();
	AddImpulse();
	SetLifeSpan(fLifeSpan);
}

void AUI_FloatingWidget::InitFloatingText()
{
	floatingText = Cast<UUI_FloatingText>(widget->GetWidget());
}
void AUI_FloatingWidget::AddImpulse() const
{
	const float _x = FMath::FRandRange(impulseMin.X, impulseMax.X);
	const float _y = FMath::FRandRange(impulseMin.Y, impulseMax.Y);
	const float _z = FMath::FRandRange(impulseMin.Z, impulseMax.Z);
	
	staticMesh->AddImpulse(FVector(_x, _y, _z), "None", true);
}