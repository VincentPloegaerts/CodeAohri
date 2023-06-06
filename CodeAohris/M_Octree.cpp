#include "M_Octree.h"
#include "M_GameMode.h"
#include "GameFramework/PhysicsVolume.h"
#include "Kismet/GameplayStatics.h"

AM_Octree::AM_Octree()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}
void AM_Octree::BeginPlay()
{
	if (iAccuracy == 1)
	{
		AM_GameMode* _gameMode = GetWorld()->GetAuthGameMode<AM_GameMode>();
		if (!_gameMode) return;

		_gameMode->SetOctree(this);
	}
}
void AM_Octree::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	DrawOctrees();
}
bool AM_Octree::ShouldTickIfViewportsOnly() const
{
	return bShowDebug;
}

/* Draw current octree */
void AM_Octree::DrawOctrees() const
{
	if (!bShowDebug) return;
	
	// Draw current octree as box
	DrawDebugBox(GetWorld(), box.GetCenter(), box.GetExtent(), debugColor, false, -1, 0, fThickness);

	// Draw a solid box if actors are invisible
	if (bIsHiddingActors)
	{
		DrawDebugSolidBox(GetWorld(), box.GetCenter(), box.GetExtent(), FColor::Green.WithAlpha(20), false, -1.0f, 0);
	}
}

/* Update actors and possible children octree */
void AM_Octree::Update()
{
	// Clear all children
	Clear();
	
	// Get all meshes in the world
	GetActors();

	// If there is to many meshes, subdivide them in different octree
	if (actors.Num() > iCapacity)
	{
		SpawnChildren();
	}

	else if (master)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddOctreeWithoutChildren"));
		master->AddOctreeWithoutChildren(this);
	}
}

/* Unshow for each octree debugs */
void AM_Octree::SwitchVisibility()
{
	// Switch current visibility
	bShowDebug = !bShowDebug;

	// Check if this octree has children
	if (HasChildren())
	{
		// Iterate all children
		const int& _childrenCount = octreeChildren.Num();
		for (int _childIndex(0); _childIndex < _childrenCount; _childIndex++)
		{
			// Change his visibility
			octreeChildren[_childIndex]->SwitchVisibility();
		}
	}
}

/* Reset all actors (= only for the main) and chidlren */
void AM_Octree::Clear()
{
	// Do this only for the first one
	if (iAccuracy == 1)
	{
		actors.Empty();
		master = this;
	}
	
	// Reset all octrees and meshes
	ResetOctree();
}

/* Get all actors wich is into the octree */
void AM_Octree::GetActors()
{
	TArray<AActor*> _allActors;
	
	// Do this only for the first one
	if (iAccuracy <= 1)
	{
		// Get all actors in the world
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), _allActors);
	}
	
	// Otherwise get all actors of the owner
	else
	{
		// Get all parent's actors before empty it
		const int& _actorsCount = actors.Num();
		for	(int _actorIndex(0); _actorIndex < _actorsCount; _actorIndex++)
		{
			TSoftObjectPtr<AActor> _actor = actors[_actorIndex];
			if (!_actor) continue;
			_allActors.Add(_actor.Get());
		}

		// Reset his actors before filling it again
		actors.Empty();
	}

	// Iterate all actors
	const int& _actorsCount = _allActors.Num();
	for	(int _actorIndex(0); _actorIndex < _actorsCount; _actorIndex++)
	{
		// Get the current actor
		const AActor* _actor = _allActors[_actorIndex];
		if (!_actor) continue;

		// Check if this actor isn't hiddenable
		if (actorsToIgnore.Contains(_actor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Ignored actor name : %s"), *_actor->GetName());
			continue;
		}
		
		// Check if this actor is into the current Octree
		// const FVector& _actorLocation = _actor->GetActorLocation();
		// const FBox& _actorBoundingBox = _actor->GetStreamingBounds();
		// if (!box.IsInside(_actorBoundingBox)) continue;

		// Check if this actor contains a static mesh
		UActorComponent* _component = _actor->GetComponentByClass(UStaticMeshComponent::StaticClass());
		if (!Cast<UStaticMeshComponent>(_component)) continue;

		// Then add it to actors
		actors.Add(_actor);
	}
}

/* Spawn and setup 8 children */
void AM_Octree::SpawnChildren()
{
	// Iterate 8 times (=> Octree)
	for (int _childIndex(0); _childIndex < 8; _childIndex++)
	{
		// Spawn a new octree
		AM_Octree* _child = GetWorld()->SpawnActor<AM_Octree>(octreeType);
		if (!_child) continue;
		_child->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		
		// Get the center and extent of the current bounding box
		FVector _initialExtent = box.GetExtent(), _extent = box.GetExtent();
		
		// Init the new octree' values
		_extent.X = _childIndex % 2 == 0 ? 0 :_extent.X;
		_extent.Y = _childIndex % 4 >= 2 ? _extent.Y : 0;
		_extent.Z = _childIndex >= 4 ? _extent.Z : 0;
		
		const FVector& _min = box.Min + _extent;
		const FVector& _max = _min + _initialExtent;
		const FBox& _boundingBox = FBox(_min, _max);
		const int& _accuracy = iAccuracy + 1;

		_child->Setup(_accuracy, FBox(_boundingBox), master, actors);

		// Add it to the children
		octreeChildren.Add(_child);
		
		// Init the new octree
		_child->Update();
	}
}

/* Destroy all children */
void AM_Octree::ResetOctree()
{
	// Iterate all children
	const int& _childCount = octreeChildren.Num();
	for (int _childIndex(0); _childIndex < _childCount; _childIndex++)
	{
		// Get each children
		AM_Octree* _octree = octreeChildren[_childIndex]; 
		if (!_octree) continue;
		
		// Destroy childs
		_octree->ResetOctree();
		
		// Destroy this octree
		_octree->Destroy();
	}

	// Clear all actors and children stored 
	octreeChildren.Empty();
}

/* Set the visibility status for all actors */
void AM_Octree::ChangeActorsVisibility(bool _visible)
{
	// Check if the visibility gonna change
	if (bIsHiddingActors != _visible) return;

	// Set his current visibity
	bIsHiddingActors = !_visible;

	// Goes through all the actors contained
	const int& _actorsCount = actors.Num();
	for (int _actorIndex(0); _actorIndex < _actorsCount; _actorIndex++)
	{
		// Get each actor
		const AActor* _actor = actors[_actorIndex].Get();
		if (!_actor) continue;
		
		// Get his SceneComponent to change the visibility
		USceneComponent* _sceneComponent = _actor->GetRootComponent();
		if (!_sceneComponent) continue;
		_sceneComponent->SetVisibility(!bIsHiddingActors);
	}
}