#include "IU_PlayerArrow.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "C_EnhancementOrb.h"
#include "IU_PunchingBall.h"
#include "IU_Player.h"
#include "A_ArenaSlice.h"
#include "B_Boss.h"
#include "B_BossCritPoint.h"
#include "GPE_Shot.h"
#include "IU_Macros.h"
#include "T_BossTuto.h"
#include "M_GameMode.h"
#include "T_BossTuto.h"
#include "UC_PlayerAttackComponent.h"


AIU_PlayerArrow::AIU_PlayerArrow()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Graphics"));
	mycollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	mycollider->SetupAttachment(RootComponent);
	staticMesh->SetupAttachment(RootComponent);
}


void AIU_PlayerArrow::BeginPlay()
{
	Super::BeginPlay();
	mycollider->OnComponentBeginOverlap.AddDynamic(this, &AIU_PlayerArrow::OnProjectileOverlap);
	SetLifeSpan(10);
	vLastLocation = GetActorLocation();
	if (state == DoubleGuided)
	{
		GetWorld()->GetTimerManager().SetTimer(doubleGuidedRotationTimer, this, &AIU_PlayerArrow::StopArrowRotation,
		                                       fRotationTime, false, fRotationTime);
	}
}

void AIU_PlayerArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveTo();
	MoveToTarget();
	CalculateDistanceTravelled();

}

void AIU_PlayerArrow::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult)
{
	bool _hit = false;
	bool _isCritPoint = false;
	bool _canSpawnFX = false;
	
	if (AB_Boss* _boss = Cast<AB_Boss>(OtherActor))
	{
		
		DamageBoss(_boss, damage);
		_hit = true;
		_canSpawnFX = true;
	}
	else if (AB_BossCritPoint* _critPoint = Cast<AB_BossCritPoint>(OtherActor))
	{
		/*if(_critPoint->GetEffectValue() < 0)
		{
			bCanSpawnPowerOrb = false;
		}*/
		_critPoint->DamageCritPoint(this);
		_hit = true;
		_canSpawnFX = true;
		if(_critPoint->GetEffectValue() >= 0)
			_isCritPoint = true;
	}
	else if (AGPE_Shot* _gpeShot = Cast<AGPE_Shot>(OtherActor))
	{
		if (UIU_HealthSystemComponent* _health = Cast<UIU_HealthSystemComponent>(_gpeShot->GetComponentByClass(UIU_HealthSystemComponent::StaticClass())))
			_health->TakeDamageWithImpactLocation(damage, true, GetActorLocation());
		_hit = true;
	}

	else if (AT_BossTuto* _bossTuto = Cast<AT_BossTuto>(OtherActor))
	{
		if (UIU_HealthSystemComponent* _health = Cast<UIU_HealthSystemComponent>(_bossTuto->GetComponentByClass(UIU_HealthSystemComponent::StaticClass())))
			_health->TakeDamageWithImpactLocation(damage, true, GetActorLocation());
		_hit = true;
		_canSpawnFX = true;
	}
	

	
	if (_hit)
	{
		onArrowHit();

		SetArrowFXHit(_isCritPoint);

		if (APawn* _pawn = GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			if (const AIU_Player* _chara = Cast<AIU_Player>(_pawn))
			{
				/*
						// const FVector _playerLocation = _chara->GetActorLocation();
						//
						// FVector _goodLoc = GetRandomRange(_chara);
						//
						//
						// int _count = 0;
						//
						// while (!CheckRange(_goodLoc,_playerLocation))
						// {
						// 	_goodLoc = GetRandomRange(_chara);
						// 	_count++;
						// 	if(_count >= 51)
						// 		return;
						// 	
						// }
				
				
					// 	const TArray<AActor*> _actorsToIgnore;
					// 	FHitResult _outHit;
					//
					//
					// 	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
					// 	                                                    _playerLocation,
					// 	                                                    _playerLocation - FVector(0, 0, 1000),
					// 	                                                    groundLayer,
					// 	                                                    false,
					// 	                                                    _actorsToIgnore,
					// 	                                                    EDrawDebugTrace::None,
					// 	                                                    _outHit,
					// 	                                                    true))
					// 	{
					// 		FVector _newLocation = FVector(_goodLoc.X, _goodLoc.Y, _outHit.ImpactPoint.Z);
					// 		AC_EnhancementOrb* _enhancementOrb = GetWorld()->SpawnActor<AC_EnhancementOrb>(
					// 			orbBPArray[currentDifficulty],
					// 			GetActorLocation() + FVector(0, 0, fHeightOfSpawningOrb),
					// 			GetActorRotation());
					//
					// 		if(!_enhancementOrb)return;
					// 		
					// 		FVector _playerLoc = FVector(_playerLocation.X, _playerLocation.Y, 0);
					// 		_enhancementOrb->SetFinalLocation(_newLocation);
					// 		_enhancementOrb->CreateBezierCurve();
					// 		_enhancementOrb->SetPlayerOwner(_chara);
					// 	}
					//
					// 	else
					// 	{
					// 		if ((!_boss) || !bCanSpawnPowerOrb)
					// 		{
					// 			Destroy();
					// 			return;
					// 		}
					//
					// 		FVector _newLocation = FVector(_goodLoc.X, _goodLoc.Y, 0);
					//
					// 		AC_EnhancementOrb* _enhancementOrb = GetWorld()->SpawnActor<AC_EnhancementOrb>(
					// 			orbBPArray[currentDifficulty],
					// 			GetActorLocation() + FVector(0, 0, fHeightOfSpawningOrb),
					// 			GetActorRotation());
					//
					// 		_enhancementOrb->SetFinalLocation(_newLocation);
					// 		_enhancementOrb->CreateBezierCurve();
					// 	}
				*/
				if(bCanSpawnPowerOrb)
				{
					if (AM_GameMode* _gm = Cast<AM_GameMode>(GetWorld()->GetAuthGameMode()))
					{
						if(!_chara->GetCharacterAttackComponent()->GetIsInTuto() || _gm->GetTutoLogic()->GetCurrentPhase() == BossFight)
							_chara->GetCharacterAttackComponent()->IncreasedCurrentShootingModeCount();

					}

					_chara->GetCharacterAttackComponent()->IncreasedCurrentSlowMoOrbCount();
					
					if(_canSpawnFX)
						_chara->GetCharacterAttackComponent()->PlayHitFXOnPlayer();
				}
			}

			if(bShowDebugs)
				UE_LOG(LogTemp, Warning, TEXT("%f damages from arrow"), damage);
		}
	}
	
	Destroy();
}


void AIU_PlayerArrow::SetDamage(float _damage)
{
	damage = _damage;
}

void AIU_PlayerArrow::CalculateDistanceTravelled()
{
	
	fDistanceTravelledTimer += GetWorld()->DeltaTimeSeconds;
	
	
	if (fDistanceTravelledTimer > 0.001f)
	{
		fCurrentDistanceTravelled = FVector::Distance(vLastLocation, GetActorLocation());
		fDistanceTravelledTimer = 0;
	}


	if(fCurrentDistanceTravelled >= fMaxArrowDistance * 0.9 && !bhasAlreadyMadeEndRangeFX)
	{
		onArrowNearMaxRange.Broadcast();
		bhasAlreadyMadeEndRangeFX = true;
	}
	
	if (fCurrentDistanceTravelled >= fMaxArrowDistance && !bIsInArena)
	{
		Destroy();
	}

	if (fCurrentDistanceTravelled >= fMaxArrowDistanceIfInArena && bIsInArena)
	{
		Destroy();
	}

}

void AIU_PlayerArrow::MoveTo()
{
	if (state == DoubleGuided)return;

	const FVector& _goodPos = GetActorLocation() + GetActorForwardVector() * speed;
	//const FVector& _goodPosLerp =  UKismetMathLibrary::VLerp(GetActorLocation(),GetActorLocation()+ GetActorForwardVector() * speed,GetWorld()->DeltaTimeSeconds) ;
	
	SetActorLocation(_goodPos);
}

void AIU_PlayerArrow::MoveToTarget()
{
	if (state != DoubleGuided || !currentTarget)return;

	const FVector& _arrowLocation = GetActorLocation();
	FRotator _rotationTarget = FRotator::ZeroRotator;
	
	if(!isInTuto)
		_rotationTarget = UKismetMathLibrary::FindLookAtRotation(_arrowLocation, currentTarget->GetActorLocation());

	else if(isInTuto)
		_rotationTarget = UKismetMathLibrary::FindLookAtRotation(_arrowLocation, currentTarget->GetActorLocation() + FVector(0,0,1000));

	const FRotator& _rotation = UKismetMathLibrary::RInterpTo_Constant(GetActorRotation(), _rotationTarget, GetWorld()->DeltaTimeSeconds, fRotationSpeed);

	const FVector& _goodPos = _arrowLocation + GetActorForwardVector() * speed;
	
	SetActorLocation(_goodPos);
	SetActorRotation(_rotation);
}

void AIU_PlayerArrow::StopArrowRotation()
{
	state = DoubleGuided;
}

FVector AIU_PlayerArrow::GetRandomRange(AIU_Player* _chara)
{
	float _spawnLocation = UKismetMathLibrary::RandomFloatInRange(
		_chara->GetCharacterAttackComponent()->GetMinDropRadius(),
		_chara->GetCharacterAttackComponent()->GetMaxDropRadius());

	float _randomAngle = UKismetMathLibrary::RandomFloatInRange(90, 270);  //Values needed so enhancement orb always spawn in front of the player

	float _x1 = _chara->GetActorLocation().X + _spawnLocation * UKismetMathLibrary::Cos(
		_randomAngle * (UKismetMathLibrary::GetPI() / 180));
	float _y1 = _chara->GetActorLocation().Y + _spawnLocation * UKismetMathLibrary::Sin(
		_randomAngle * (UKismetMathLibrary::GetPI() / 180));



	return FVector(_x1, _y1, 0);
}

bool AIU_PlayerArrow::CheckRange(FVector _toCheck, FVector _playerLocation)
{
	const TArray<AActor*> _actorsToIgnore;
	FHitResult _outHit;
	
	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
	                                                    _toCheck + FVector(0, 0, _playerLocation.Z + 500),
	                                                    _toCheck + FVector(0, 0, _playerLocation.Z - 1000),
	                                                    groundLayer,
	                                                    false,
	                                                    _actorsToIgnore,
	                                                    EDrawDebugTrace::None,
	                                                    _outHit,
	                                                    true))
	{

	
		if (Cast<AA_ArenaSlice>(_outHit.GetActor()))
		{
			return true;
		}

	
		return false;
	}
	
		return false;
}


void AIU_PlayerArrow::SetArrowFX()
{
	if (state == NONE) return;

	if (state == Simple && niagaraSimpleShootComp)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			niagaraSimpleShootComp, RootComponent, NAME_None, FVector::ZeroVector, FRotator(0.f),
			EAttachLocation::Type::KeepRelativeOffset, true);
	}

	if (state == Harpoon && niagaraHarpoonShootComp)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			niagaraHarpoonShootComp, RootComponent, NAME_None, FVector::ZeroVector, FRotator(0.f),
			EAttachLocation::Type::KeepRelativeOffset, true);
	}

	if (state == DoubleGuided && niagaraThornShootComp)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			niagaraThornShootComp, RootComponent, NAME_None, FVector::ZeroVector, FRotator(0.f),
			EAttachLocation::Type::KeepRelativeOffset, true);
	}
}

void AIU_PlayerArrow::SetArrowFXHit(bool isCritPoint)
{
	if (state == NONE) return;
	
	if(state == Simple && isCritPoint && niagaraCritPointHit)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
													   niagaraCritPointHit, GetActorLocation(), FRotator(0.f));
		return;
	}

	if(state == Harpoon && isCritPoint && niagaraHarpoonCritPointHit)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
													   niagaraHarpoonCritPointHit, GetActorLocation(), FRotator(0.f));
		return;
	}

	if(state == DoubleGuided && isCritPoint && niagaraDoubleGuidedCritPointHit)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
													   niagaraDoubleGuidedCritPointHit, GetActorLocation(), FRotator(0.f));
		return;
	}

	if (state == Simple && niagaraSimpleShootHit)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
		                                               niagaraSimpleShootHit, GetActorLocation(), FRotator(0.f));
	}

	if (state == Harpoon && niagaraHarpoonShootHit)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
		                                               niagaraHarpoonShootHit, GetActorLocation(), FRotator(0.f));
	}

	if (state == DoubleGuided && niagaraThornShootHit)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
		                                               niagaraThornShootHit, GetActorLocation(), FRotator(0.f));
	}

		
}

void AIU_PlayerArrow::DamageBoss(AB_Boss* _boss, const float _damage) const
{
	if (!_boss) return;
	_boss->GetHealthComponent()->TakeDamageWithImpactLocation(_damage, true, GetActorLocation());
	if (bCanStun && _boss)
	{
		_boss->SetHarpoonData(iStackNeededToStun, fHarpoonStunDuration);
		_boss->CheckIfStunned();
	}
	else if (!bCanStun && _boss)
	{
		_boss->ResetHarpoonStack();
	}
}

