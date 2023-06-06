#include "P_PB_Wave.h"

#include "IU_HealthSystemComponent.h"
#include "IU_Macros.h"
#include "M_GameMode.h"
#include "P_PatternWave.h"
#include "IU_Player.h"
#include "P_DT_WaveDataTable.h"
#include "STRUCT_WaveSettings.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PoseableMeshComponent.h"

UP_PB_Wave::UP_PB_Wave()
{
	PrimaryComponentTick.bCanEverTick = false;
	/*bonesNameToReset.Add("externe");
	bonesNameToReset.Add("externe1");
	bonesNameToReset.Add("externe2");
	bonesNameToReset.Add("externe3");
	bonesNameToReset.Add("externe4");
	bonesNameToReset.Add("externe5");
	bonesNameToReset.Add("externe6");
	bonesNameToReset.Add("externe7");
	bonesNameToReset.Add("externe8");
	bonesNameToReset.Add("externe9");
	bonesNameToReset.Add("externe10");
	bonesNameToReset.Add("externe11");
	bonesNameToReset.Add("externe12");
	bonesNameToReset.Add("externe13");
	bonesNameToReset.Add("externe14");
	bonesNameToReset.Add("externe15");
	bonesNameToReset.Add("externe16");
	bonesNameToReset.Add("externe17");*/
	bonesNameToReset.Add("interne");
	/*onesNameToReset.Add("interne01");
	bonesNameToReset.Add("interne02");
	bonesNameToReset.Add("interne03");
	bonesNameToReset.Add("interne04");
	bonesNameToReset.Add("interne05");
	bonesNameToReset.Add("interne06");
	bonesNameToReset.Add("interne07");
	bonesNameToReset.Add("interne08");
	bonesNameToReset.Add("interne09");
	bonesNameToReset.Add("interne10");
	bonesNameToReset.Add("interne11");
	bonesNameToReset.Add("interne12");
	bonesNameToReset.Add("interne13");
	bonesNameToReset.Add("interne14");
	bonesNameToReset.Add("interne15");
	bonesNameToReset.Add("interne16");
	bonesNameToReset.Add("interne17");*/
	iTotalBones = bonesNameToReset.Num();
}

void UP_PB_Wave::ActivateBehavior(UP_DT_BaseDataTable* _dataTable)
{
	Super::ActivateBehavior(_dataTable);
	
	const UWorld* _world = GetWorld();
	bonesNameToReset.Empty();
	bonesNameToReset.Add("interne");
	iTotalBones = bonesNameToReset.Num();
	if (const AP_PatternWave* _wave = Cast<AP_PatternWave>(owner))
	{
		boxCollider = _wave->GetBoxCollider();
		UPoseableMeshComponent* _poseableMesh = _wave->GetPoseableMeshComponent();
		poseableMeshs.Add(_poseableMesh);

		
		if (_poseableMesh && boxCollider)
		{
			InitPoseableMeshs(_poseableMesh);
			
			fColliderLocalHeight = boxCollider->GetRelativeLocation().Z;
			vCurrentScale = _poseableMesh->GetComponentScale();
			fScale = vCurrentScale.X;
		
			if (const AM_GameMode* _gameMode = _world->GetAuthGameMode<AM_GameMode>())
			{
				player = _gameMode->GetPlayer();
				playerHealthComponent = player->HealthComponent();
				
				const UP_DT_WaveDataTable* _waveDataTable = Cast<UP_DT_WaveDataTable>(_dataTable);
				const FWaveSettings& _waveSettings = _waveDataTable->GetWaveSettings();
				fWaveSpeed = _waveSettings.fWaveSpeed;
				fWaveDamage = _waveSettings.fWaveDamage;		
				fTubeHeight = _waveSettings.fWaveHeight;
				fTubeSize = _waveSettings.fWavethickness;
				fMaxSize = _waveSettings.fMaxSize / 211.0f;
				bUseMaxSize = _waveSettings.bUseMaxSize;
				
				FVector _scale = _poseableMesh->GetComponentScale();
				_scale.Z = _waveSettings.fWaveHeight;
				_poseableMesh->SetWorldScale3D(_scale);				
				boxCollider->SetBoxExtent( FVector(fTubeSize * 12.5f,10.0f,fTubeHeight * fTubeHeight * 12.0f ));
				
				onBehaviorTick.AddDynamic(this, &UP_PB_Wave::ScaleWave);
				onBehaviorTick.AddDynamic(this, &UP_PB_Wave::MoveCollider);
				onBehaviorDeltaTick.AddDynamic(this, &UP_PB_Wave::UpdateScale);
				owner->OnActorBeginOverlap.AddDynamic(this, &UP_PB_Wave::OnBeginOverlap);
				owner->OnActorEndOverlap.AddDynamic(this, &UP_PB_Wave::OnEndOverlap);
				if (!bUseMaxSize)
					_world->GetTimerManager().SetTimer(timer, this, &UP_PB_Wave::ReachEndDuration, _waveSettings.fWaveDuration);
			}
			else
			{
				LOG("UP_PB_Wave::ActivateBehavior -> invalid gamemode");
				onBehaviorComplete.Broadcast();
			}
		}
		else
		{
			LOG("UP_PB_Wave::ActivateBehavior -> invalid poseable mesh or collider");
			onBehaviorComplete.Broadcast();
		}
	}
	else
	{
		LOG("UP_PB_Wave::ActivateBehavior -> invalid wave owner");
		onBehaviorComplete.Broadcast();
	}
}

void UP_PB_Wave::ActivateBehavior(FPatternData& _patternData)
{
	Super::ActivateBehavior(_patternData);
	
	const UWorld* _world = GetWorld();
	bonesNameToReset.Empty();
	bonesNameToReset.Add("interne");
	iTotalBones = bonesNameToReset.Num();
	if (const AP_PatternWave* _wave = Cast<AP_PatternWave>(owner))
	{
		boxCollider = _wave->GetBoxCollider();
		UPoseableMeshComponent* _poseableMesh = _wave->GetPoseableMeshComponent();
		poseableMeshs.Add(_poseableMesh);

		
		if (_poseableMesh && boxCollider)
		{
			InitPoseableMeshs(_poseableMesh);
			
			fColliderLocalHeight = boxCollider->GetRelativeLocation().Z;
			vCurrentScale = _poseableMesh->GetComponentScale();
			fScale = vCurrentScale.X;

		
			if (const AM_GameMode* _gameMode = _world->GetAuthGameMode<AM_GameMode>())
			{
				player = _gameMode->GetPlayer();
				playerHealthComponent = player->HealthComponent();
				
				const FWaveSettings& _waveSettings = _patternData.GetWaveSettings();
				fWaveSpeed = _waveSettings.fWaveSpeed;
				fWaveDamage = _waveSettings.fWaveDamage;
				fTubeHeight = _waveSettings.fWaveHeight;
				fTubeSize = _waveSettings.fWavethickness;
				fMaxSize = _waveSettings.fMaxSize;
				bUseMaxSize = _waveSettings.bUseMaxSize;
				
				FVector _scale = _poseableMesh->GetComponentScale();
				_scale.Z = _waveSettings.fWaveHeight;
				_poseableMesh->SetWorldScale3D(_scale);				
				boxCollider->SetBoxExtent( FVector(fTubeSize * 12.5f,10.0f,fTubeHeight * fTubeHeight * 12.0f ));
				
				onBehaviorTick.AddDynamic(this, &UP_PB_Wave::ScaleWave);
				onBehaviorTick.AddDynamic(this, &UP_PB_Wave::MoveCollider);
				onBehaviorDeltaTick.AddDynamic(this, &UP_PB_Wave::UpdateScale);
				owner->OnActorBeginOverlap.AddDynamic(this, &UP_PB_Wave::OnBeginOverlap);
				owner->OnActorEndOverlap.AddDynamic(this, &UP_PB_Wave::OnEndOverlap);
				if (!bUseMaxSize)
				_world->GetTimerManager().SetTimer(timer, this, &UP_PB_Wave::ReachEndDuration, _waveSettings.fWaveDuration);
			}
			else
			{
				LOG("UP_PB_Wave::ActivateBehavior -> invalid gamemode");
				onBehaviorComplete.Broadcast();
			}
		}
		else
		{
			LOG("UP_PB_Wave::ActivateBehavior -> invalid poseable mesh or collider");
			onBehaviorComplete.Broadcast();
		}
	}
	else
	{
		LOG("UP_PB_Wave::ActivateBehavior -> invalid wave owner");
		onBehaviorComplete.Broadcast();
	}
}

void UP_PB_Wave::InitPoseableMeshs(const UPoseableMeshComponent* _baseMesh)
{
	TArray<USceneComponent*> _components;
	_baseMesh->GetChildrenComponents(false, _components);
			
	const uint& _max = _components.Num();
	for (uint i(0); i < _max; ++i)
		if (UPoseableMeshComponent* _mesh = Cast<UPoseableMeshComponent>(_components[i]))
			poseableMeshs.Add(_mesh);
	
	iTotalPoseableMesh = poseableMeshs.Num();
}

void UP_PB_Wave::TickBehavior(const float& _deltaTime)
{
	onBehaviorDeltaTick.Broadcast(_deltaTime);
	onBehaviorTick.Broadcast();
}

void UP_PB_Wave::UpdateScale(const float& _deltaTime)
{
	fScale += _deltaTime * fWaveSpeed;
	if (bUseMaxSize && fScale > fMaxSize)
		onBehaviorComplete.Broadcast();
}

void UP_PB_Wave::ScaleWave()
{
	vCurrentScale.X = fScale;
	vCurrentScale.Y = fScale;
	vCurrentScale.Z = fTubeHeight;
	for (uint i(0); i < iTotalPoseableMesh; ++i)
	{
		UPoseableMeshComponent* _mesh = poseableMeshs[i];
#if WITH_EDITOR
		ROG_ERROR(!_mesh, "UP_PB_Wave::ScaleWave -> Invalid poseable mesh");
#endif
		_mesh->SetBoneScaleByName(nParentBone, vCurrentScale, EBoneSpaces::ComponentSpace);

		const FVector& _size = (_mesh->GetBoneScaleByName(nParentBone, EBoneSpaces::ComponentSpace));
		const float& _x = (_size.X / fTubeSize);
		const float& _y = (_size.Y / fTubeSize);
		for (uint j(0); j < iTotalBones; ++j)
		{
			_mesh->SetBoneScaleByName(bonesNameToReset[j],FVector(_size.X + (_x - 1 ) * (0.280f * fTubeSize),
																		 _size.Y + (_y - 1 ) * (0.280f * fTubeSize),
																		 fTubeHeight), EBoneSpaces::ComponentSpace);
		}
	}
}

void UP_PB_Wave::MoveCollider()
{
	const UPoseableMeshComponent* _mesh = poseableMeshs[0];
#if WITH_EDITOR
	ROG_ERROR(!_mesh, "UP_PB_Wave::MoveCollider -> Invalid poseable mesh");
#endif
	
	const FVector& _waveLocation = owner->GetActorLocation();
	
	FVector _direction(player->GetActorLocation() - _waveLocation);
	_direction.Z = 0;
	_direction.Normalize();
	const FVector& _normalized = _direction;
	_direction *= FVector::Dist(_waveLocation, _mesh->GetSocketLocation(nColliderCenterBoneTwo));
	_direction.Z = fColliderLocalHeight;
	boxCollider->SetWorldLocation(_waveLocation + _direction -( _normalized * fTubeSize *0.12f / vCurrentScale.X));	
	boxCollider->SetWorldRotation(_direction.Rotation());
}

void UP_PB_Wave::OnBeginOverlap(AActor* _mine, AActor* _other)
{
	if (_other == player)
	{
		onBehaviorTick.AddDynamic(this, &UP_PB_Wave::DamagePlayer);
	}
	
}

void UP_PB_Wave::DamagePlayer()
{
	playerHealthComponent->TakeDamage(fWaveDamage);
}

void UP_PB_Wave::OnEndOverlap(AActor* _mine, AActor* _other)
{
	if (_other == player)
		onBehaviorTick.RemoveDynamic(this, &UP_PB_Wave::DamagePlayer);
}

void UP_PB_Wave::ReachEndDuration()
{
	onBehaviorComplete.Broadcast();
}