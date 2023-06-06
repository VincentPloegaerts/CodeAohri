#include "IU_FSMBase.h"
#include "IU_PlayerFSMBrain.h"

void UIU_FSMBase::Init(UIU_PlayerFSMBrain* _brain)
{
	brain = _brain;
}

void UIU_FSMBase::Enter()
{
	//UE_LOG(LogTemp, Warning, TEXT("Entered %s State"), *GetFName().ToString());
}

void UIU_FSMBase::Exit()
{
	//UE_LOG(LogTemp, Warning, TEXT("Exited %s State"), *GetFName().ToString());
}