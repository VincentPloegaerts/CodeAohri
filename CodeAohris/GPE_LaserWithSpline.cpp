#include "GPE_LaserWithSpline.h"


AGPE_LaserWithSpline::AGPE_LaserWithSpline()
{
	splineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
	splineComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	niagaraLaser->AttachToComponent(splineComponent,FAttachmentTransformRules::KeepRelativeTransform);
	damageBox->AttachToComponent(niagaraLaser,FAttachmentTransformRules::KeepRelativeTransform);
}

void AGPE_LaserWithSpline::BeginPlay()
{
	Super::BeginPlay();
	InitSpline();
}

void AGPE_LaserWithSpline::InitSpline()
{
	splineLenght = splineComponent->GetSplineLength();
	firstRotation = niagaraLaser->GetComponentRotation();
	baseBoxOffset = damageBox->GetRelativeLocation();
	initialCollisions = damageBox->GetCollisionResponseToChannels();
	if (laserMode == SplineMovement)
	{
		FTimerHandle _handle;
		FTimerHandle _handle2;
		GetWorld()->GetTimerManager().SetTimer(_handle, this, &AGPE_LaserWithSpline::SpawnLasers, respawnTime, true,delayBeforeActivation + respawnTime);
	}
}


void AGPE_LaserWithSpline::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!started)return;
#if WITH_EDITOR
	if (showDebug)
	{
		Debug();
	}
#endif
	
	switch (laserMode)
	{
	case SplineBackAndForth:
		UpdateSplineBackForth(DeltaSeconds);
		break;
	case SplineMovement:
		UpdateSplineMovement(DeltaSeconds);
		break;
	default:
		return;
	}
}



void AGPE_LaserWithSpline::MoveAlongSpline(const float& _actualDistanceOnSpline,const float& _nextDistanceOnSpline,UNiagaraComponent* _toMove) const
{
	const FVector& _location = splineComponent->GetLocationAtDistanceAlongSpline(_nextDistanceOnSpline,ESplineCoordinateSpace::World);
	_toMove->SetWorldLocation(_location);
	const FRotator& _oldRotation = splineComponent->GetRotationAtDistanceAlongSpline(_actualDistanceOnSpline, ESplineCoordinateSpace::World);
	const FRotator& _newRotation = splineComponent->GetRotationAtDistanceAlongSpline(_nextDistanceOnSpline, ESplineCoordinateSpace::World);
	
	_toMove->AddWorldRotation(_newRotation-_oldRotation);
}

void AGPE_LaserWithSpline::UpdateSplineBackForth(float _deltaSeconds)
{
	const float& _actualDistance = (timePattern / duration) * splineLenght;
	timePattern += _deltaSeconds * (isActive ? -1.0f : 1.0f);
	const float& _nextDistance = (timePattern / duration) * splineLenght;
	MoveAlongSpline(_actualDistance,_nextDistance, niagaraLaser);
	if (timePattern >= duration || timePattern < 0)
	{
		isActive = !isActive;
	}
}

void AGPE_LaserWithSpline::UpdateSplineMovement(float _deltaSeconds)
{
	int _rank = 0;	
	if (niagaraLaser->IsActive())
	{
		if (timePattern >= duration)
		{
			timePattern = 0.0f;
			niagaraLaser->SetWorldRotation(firstRotation);
			niagaraLaser->DeactivateImmediate();
			damageBox->SetActive(false);			
			damageBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			onLaserDeactivate.Broadcast();
		}
		else
		{
			const float& _actualDistance = (timePattern / duration) * splineLenght;
			timePattern += _deltaSeconds;
			const float& _nextDistance = (timePattern / duration) * splineLenght;
			MoveAlongSpline(_actualDistance,_nextDistance, niagaraLaser);
		}
	}
	for (const TPair<UNiagaraComponent*, UStaticMeshComponent*>& _pair : laserReserve)
	{
		if (_pair.Key->IsActive())
		{
			if (timers[_rank] >= duration)
			{
				timers[_rank] = 0.0f;
				_pair.Key->SetWorldRotation(firstRotation);
				_pair.Key->DeactivateImmediate();
				_pair.Value->SetActive(false);
				_pair.Value->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
				onLaserDeactivate.Broadcast();				
			}
			else
			{
				const float& _actualDistance = (timers[_rank] / duration) * splineLenght;
				timers[_rank] += _deltaSeconds;
				const float& _nextDistance = (timers[_rank] / duration) * splineLenght;
				MoveAlongSpline(_actualDistance,_nextDistance, _pair.Key);
			}
		}
		_rank++;
	}	
}

void AGPE_LaserWithSpline::SpawnLasers()
{
	const FVector& _startLocation =  splineComponent->GetLocationAtDistanceAlongSpline(0.0f,ESplineCoordinateSpace::World);	

	if (!niagaraLaser->IsActive())
	{
		niagaraLaser->Activate(true);
		damageBox->Activate();
		niagaraLaser->SetWorldLocation(_startLocation);
		damageBox->SetCollisionResponseToChannels(initialCollisions);
		return;
	}
	for (const TPair<UNiagaraComponent*, UStaticMeshComponent*>& _pair : laserReserve)
	{
		if (!_pair.Key->IsActive())
		{
			_pair.Key->Activate(true);
			_pair.Value->Activate();
			_pair.Key->SetWorldLocation(_startLocation);			
			_pair.Value->SetCollisionResponseToChannels(initialCollisions);
			return;
		}
	}	
	InitNewLaser(_startLocation);
}

void AGPE_LaserWithSpline::InitNewLaser(const FVector& _startLocation)
{
	UNiagaraComponent* _niagara =  NewObject<UNiagaraComponent>(niagaraLaser);
	_niagara->AttachToComponent(splineComponent,FAttachmentTransformRules::KeepRelativeTransform);
	_niagara->RegisterComponent();
	
	UStaticMeshComponent* _box =  NewObject<UStaticMeshComponent>(damageBox); //todo add extents
	_box->AttachToComponent(_niagara,FAttachmentTransformRules::KeepRelativeTransform);
	_box->RegisterComponent();
	
	_niagara->SetWorldLocation(_startLocation);
	_box->SetRelativeLocation(baseBoxOffset);
	
	_niagara->SetWorldRotation(firstRotation);
	
	_niagara->SetAsset(niagaraLaser->GetAsset());
	_box->SetStaticMesh(damageBox->GetStaticMesh());

	_niagara->Activate(true);
	_box->Activate(true);
	
	_box->SetCollisionResponseToChannels(initialCollisions);
	_box->SetHiddenInGame(damageBox->bHiddenInGame);
	
	_box->SetRelativeScale3D(damageBox->GetRelativeScale3D());
	_niagara->SetVectorParameter(nameSizeBeamProperty,beamEnd);
	
	laserReserve.Add(_niagara,_box);
	timers.Add(0.0f);
}

#if WITH_EDITOR
void AGPE_LaserWithSpline::Debug()
{
	const int& _max = debugPrecisionPercent;
	FVector _position = FVector(0);
	FVector _destination = FVector(0);
	const UWorld* _world =  GetWorld();
	for (float i =0;i<_max;i++)
	{
		const float _fuckDemInt = i+1;
		_position = splineComponent->GetLocationAtDistanceAlongSpline((i/_max) * splineLenght,ESplineCoordinateSpace::World);
		_destination = splineComponent->GetLocationAtDistanceAlongSpline((_fuckDemInt/_max) * splineLenght,ESplineCoordinateSpace::World);
		DrawDebugLine(_world,_position,_destination,FColor::White,false,-1,0,5);
	}
}
#endif