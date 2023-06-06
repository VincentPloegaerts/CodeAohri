#include "T_TutorialArea.h"
#include "IU_Player.h"
#include "IU_Macros.h"
#include "IU_PlayerFSMBrain.h"
#include "Blueprint/UserWidget.h"

AT_TutorialArea::AT_TutorialArea()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	slowArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SlowBoxComponent"));
	slowArea->SetupAttachment(RootComponent);

	slowArea->SetLineThickness(25.0f);
	slowArea->ShapeColor = FColor::Green;
	
	widgetArea = CreateDefaultSubobject<UBoxComponent>(TEXT("WidgetBoxComponent"));
	widgetArea->SetupAttachment(slowArea);

	widgetArea->SetLineThickness(20.0f);
	widgetArea->ShapeColor = FColor::Cyan;
}

void AT_TutorialArea::BeginPlay()
{
	Super::BeginPlay();

	inputSettings = UInputSettings::GetInputSettings();
	UpdateCurrentElement();
	widgetArea->OnComponentBeginOverlap.AddDynamic(this, &AT_TutorialArea::OnActorEnterInWidgetArea);
	slowArea->OnComponentBeginOverlap.AddDynamic(this, &AT_TutorialArea::OnActorEnterInSlowArea);
	slowArea->OnComponentEndOverlap.AddDynamic(this, &AT_TutorialArea::OnActorLeaveSlowArea);
}

void AT_TutorialArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	IncreaseSlowFactor();
}

void AT_TutorialArea::OnActorEnterInWidgetArea(UPrimitiveComponent* _this, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _index, bool _sweep, const FHitResult& _result)
{
	if (bAllElementsAreDone) return;

	player = Cast<AIU_Player>(_otherActor);
	if (!player || player->CanSkipTutorial()) return;

	// Display current element's widget
	if (currentElement)
	{
		currentWidget = currentElement->GetWidget(GetWorld());
		if (currentWidget)
		{
			currentWidget->AddToViewport();
		}
	}
}

void AT_TutorialArea::OnActorEnterInSlowArea(UPrimitiveComponent* _this, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _index, bool _sweep, const FHitResult& _result)
{
	if (bAllElementsAreDone) return;

	player = Cast<AIU_Player>(_otherActor);
	if (!player || player->CanSkipTutorial()) return;

	// Increase slow factor
	bCanIncreaseSlowFactor = true;

	// Execute current element
	ExecuteElement();
}

void AT_TutorialArea::OnActorLeaveSlowArea(UPrimitiveComponent* _this, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _index)
{
	canSkip = false;
	StopTutorial();
}

void AT_TutorialArea::ExecuteElement()
{
	canSkip = true;
	
	// Stop tutorial if the player has already done it
	if (player->MovementFSM()->CurrentState()->StateType() == currentElement->GetStateType())
	{
		StopTutorial();
		return;
	}
	
	// Bind the new inputs required for this tutorial
	UpdateInputs(true);

	if (player)
	{
		// If this element contains a condition
		condition = currentElement->GetAdditionalCondition();
		if (condition)
		{
			// It's required to finish this element
			condition->OnConditionDone()->AddDynamic(this, &AT_TutorialArea::StopTutorial);
			condition->InitCondition(player);
		}

		else
		{
			// Otherwise, juste need to press a button
			player->InputComponent->BindAction(currentElement->GetTutorialName(), IE_Pressed, this, &AT_TutorialArea::StopTutorial);
		}
	}
}

void AT_TutorialArea::StopTutorial()
{
	// Stop player's 'SlowFactor'
	bCanIncreaseSlowFactor = false;

	if (player)
	{
		// Reset slow factor on the player
		player->ApplySlow(1.0f);
	}
	
	if (currentElement)
	{
		// If there was a condition, clear all conditions' events
		if (condition)
		{
			condition->OnConditionDone()->RemoveDynamic(this, &AT_TutorialArea::StopTutorial);
			condition->ResetCondition();
		}
			
		else if (player && player->InputComponent)
		{
			// Otherwise, juste remove the binding button
			player->InputComponent->RemoveActionBinding(currentElement->GetTutorialName(), IE_Pressed);
		}

		// Remove the old inputs
		UpdateInputs(false);

		// Remove current widget from viewport
		if (currentWidget)
		{
			currentWidget->RemoveFromParent();
		}

		// If there is another element, execute it
		iCurrentElement++;
		if (iCurrentElement < allElements.Num())
		{
			// Active again player's 'SlowFactor'
			bCanIncreaseSlowFactor = true;

			// Skip to the new element
			UpdateCurrentElement();
			ExecuteElement();
		
			return;
		}
	}
	
	// Otherwise, reset all
	iCurrentElement = 0;
	
	// Update tutorial area status
	bAllElementsAreDone = true;
}

void AT_TutorialArea::UpdateCurrentElement()
{
	// If all elements are already run
	if (iCurrentElement >= allElements.Num())
	{
		LOG("You forgot to add tutorial element into : %s !", *GetName());
		StopTutorial();
		return;
	}

	// Take current element's datas
	currentElement = allElements[iCurrentElement];

	// If there are none left stop the tutorial
	if (!currentElement)
	{
		StopTutorial();
	}
}

void AT_TutorialArea::UpdateInputs(bool _toAdd) const
{
	if (!inputSettings || !currentElement) return;

	// Get all binds needed for the current element
	const TArray<FInputActionKeyMapping>& _binds = currentElement->GetBindsAction();
	const int _bindsCount = _binds.Num();

	// Run through all the binds
	for (int _bindIndex = 0; _bindIndex < _bindsCount; _bindIndex++)
	{
		if (_toAdd)
		{
			// Add the new bind
			inputSettings->AddActionMapping(_binds[_bindIndex]);
		}

		else
		{
			// Remove the old bind
			inputSettings->RemoveActionMapping(_binds[_bindIndex]);
		}
	}
}

void AT_TutorialArea::IncreaseSlowFactor()
{
	if (!bCanIncreaseSlowFactor || !player) return;
	
	// Decrease the slow factor
	fCurrentSlowFactor -= fAdditionalSlowFactor;
	
	// If the current slow factor has reached the minimum amount
	if (fCurrentSlowFactor <= fMinSlowFactor)
	{
		// Set it to the minimum
		fCurrentSlowFactor = fMinSlowFactor;

		// Stop to decrease the slow factor
		bCanIncreaseSlowFactor = false;
	}

	// Apply the new slow factor on the player
	player->ApplySlow(fCurrentSlowFactor / 100.0f);
}