#include "P_PatternSocket.h"

AP_PatternSocket::AP_PatternSocket()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
}