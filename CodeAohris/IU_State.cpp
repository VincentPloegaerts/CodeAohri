#include "IU_State.h"
#include "IU_PlayerFSMBrain.h"

void UIU_State::Init(UIU_PlayerFSMBrain* _brain)
{
	Super::Init(_brain);
	
	if (brain && brain->PlayerOwner())
	{
		playerMovementComponent = brain->PlayerOwner()->MovementComponent();
		if (playerMovementComponent)
			PassData(playerMovementComponent->MovementStats());
	}
}

void UIU_State::Update(const float _deltaTime) { }

bool UIU_State::CanEnterState()
{
	return false;
}

void UIU_State::PassData(const FMovementData& _data) { }