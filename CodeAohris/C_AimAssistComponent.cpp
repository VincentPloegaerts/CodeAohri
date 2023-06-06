#include "C_AimAssistComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UC_AimAssistComponent::UC_AimAssistComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UC_AimAssistComponent::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle _checkTimer;
	GetWorld()->GetTimerManager().SetTimer(_checkTimer, this, &UC_AimAssistComponent::Init, fStartCheck);
}

void UC_AimAssistComponent::Init()
{
	// UE_LOG(LogTemp, Warning, TEXT("Aim => Init"));
		
	if (!camera)
	{
		camera = Cast<AIU_Camera>(GetOwner());
	}

	if (camera)
	{
		// if (const TSoftObjectPtr<AActor>& _target = camera->GetTarget())
		// {
		// 	actorsToIgnore.Add(_target);
		// }

		// UE_LOG(LogTemp, Warning, TEXT("Aim => GOOD CAMERA"));
		
		if (AActor* _target = camera->GetTarget().Get())
		{
			actorsToIgnore.Add(_target);
			// UE_LOG(LogTemp, Warning, TEXT("Aim => Add target : %s"), *_target->GetName());
		}
	}
	
	FTimerHandle _checkTimer;
	GetWorld()->GetTimerManager().SetTimer(_checkTimer, this, &UC_AimAssistComponent::CheckToTarget, fCheckRate, true, FMath::RandRange(0.1f, 0.5f));
}

void UC_AimAssistComponent::CheckToTarget() const 
{
	static float _slowFactor = 100.0f;
	
	TArray<FHitResult> _results;
	if (UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), camera->GetActorLocation(), camera->GetActorLocation() + camera->GetActorForwardVector() * fCheckRange,
														hittableLayer, false, actorsToIgnore, bShowDebugs ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
														_results, true, FLinearColor::Blue, FLinearColor::Green, 0.1f))
	{
		const int& _resultsCount = _results.Num();
		for (int _resultIndex = 0; _resultIndex < _resultsCount; ++_resultIndex)
		{
			const FHitResult& _result = _results[_resultIndex];
		
			const UActorComponent* _component = _result.Component.Get();
			if (!_component) return;

			const UBoxComponent* _boxComponent = Cast<UBoxComponent>(_component);
			if (!_boxComponent) return;

			if (const AActor* _target = _boxComponent->GetOwner())
			{
				UActorComponent* _actorComponent = _target->GetComponentByClass(hittableObjectType);
				if (!_actorComponent)return;
				const UC_HittableComponent* _hittable = Cast<UC_HittableComponent>(_actorComponent);
				if (!_hittable) return;
				_slowFactor = _hittable->GetSlowFactor(_boxComponent);
			}
		}
	}

	camera->SetAimAssistSpeedFactor(_slowFactor / 100.0f);
}