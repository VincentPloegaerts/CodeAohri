#include "C_FrustumChecker.h"
#include "IU_Camera.h"
#include "M_GameMode.h"
#include "Kismet/GameplayStatics.h"

UC_FrustumChecker::UC_FrustumChecker()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UC_FrustumChecker::BeginPlay()
{
	Super::BeginPlay();
			
	if (const AIU_Camera* _camera = Cast<AIU_Camera>(GetOwner()))
	{
		cameraComponent = _camera->GetCameraComponent();
	}

	world = GetWorld();
	if (!world) return;
	
	if (AM_GameMode* _gameMode = world->GetAuthGameMode<AM_GameMode>())
	{
		octree = _gameMode->GetOctree();
		if (octree)
		{
			InitTimer(octree);
			return;
		}
		
		_gameMode->OnOctreeSetted()->AddDynamic(this, &UC_FrustumChecker::InitTimer);
	}
}

/* When the octree is setted, start a timer for checking method */
void UC_FrustumChecker::InitTimer(AM_Octree* _octree)
{
	octree = _octree;
	if (!world) return;

	localPlayer = world->GetFirstLocalPlayerFromController();
	if (localPlayer)
	{
		viewportClient = localPlayer->ViewportClient;
		if (localPlayer)
		{
			viewport = viewportClient->Viewport;
		}
	}
	
	FTimerHandle checkTimer = FTimerHandle();
	world->GetTimerManager().SetTimer(checkTimer, this, &UC_FrustumChecker::CheckOctree, fCheckRate, true, fCheckRate);
}

/* Check if the octree and its children is into the frustum */
void UC_FrustumChecker::CheckOctree()
{
	if (!octree) return;

	const TArray<AM_Octree*>& _octrees = octree->GetOctreesWithoutChildren();
	const int& _octreesCount = _octrees.Num();
	
	for (int _index(0); _index < _octreesCount; _index++)
	{
		AM_Octree* _octree = _octrees[_index];
		if (!_octree) return;
		
		// Check if it's into the frutum
		const bool _isInFrustum = IsInFrustum(_octree);

		// Update the visibility of the actors
		_octree->ChangeActorsVisibility(_isInFrustum);
	}
}

/* Check if the current octree is into the camera's frustum */
bool UC_FrustumChecker::IsInFrustum(const AM_Octree* _target) const
{
	if (!localPlayer || ! viewportClient || !viewport) return false;
	
	FSceneViewFamilyContext _viewFamily(FSceneViewFamily::ConstructionValues(viewport, world->Scene, viewportClient->EngineShowFlags).SetRealtimeUpdate(true));

	FVector _viewLocation;
	FRotator _viewRotation;
	if (const FSceneView* _sceneView = localPlayer->CalcSceneView(&_viewFamily, _viewLocation, _viewRotation, viewport))
	{
		const FBox& _box = _target->GetBox();
		const bool _isInFrustum = _sceneView->ViewFrustum.IntersectBox(_box.GetCenter(), _box.GetExtent());
		
		const FVector& _targetLocation = _target->GetActorLocation();
		const bool _isInRange = _sceneView->ViewFrustum.DistanceTo(_targetLocation) <= fRange;
		
		return _isInFrustum && _isInRange;
	}

	return false;
}