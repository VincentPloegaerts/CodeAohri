#include "C_HittableComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UC_HittableComponent::UC_HittableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	externalBox = CreateDefaultSubobject<UBoxComponent>(TEXT("External box"));
	
	internalBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Internal box"));
	internalBox->SetupAttachment(externalBox);
}

void UC_HittableComponent::BeginPlay()
{
	Super::BeginPlay();
	externalBox->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}

float UC_HittableComponent::GetSlowFactor(const UBoxComponent* _collidedBox) const
{
	if (_collidedBox == externalBox)
	{
		return fExternalFactor;
	}

	if (_collidedBox == internalBox)
	{
		return fInternalFactor;
	}

	return 100.0f;
}