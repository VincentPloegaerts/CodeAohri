
#include "GPE_PlatformWithHealth.h"


AGPE_PlatformWithHealth::AGPE_PlatformWithHealth()
{	
	healthSystemComponent = CreateDefaultSubobject<UIU_HealthSystemComponent>("HealthSystemComponent");
	AddOwnedComponent(healthSystemComponent);
}
