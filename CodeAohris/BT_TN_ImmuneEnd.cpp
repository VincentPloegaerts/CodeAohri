#include "BT_TN_ImmuneEnd.h"

#include "B_Boss.h"
#include "IU_HealthSystemComponent.h"
#include "IU_Macros.h"

void UBT_TN_ImmuneEnd::ImmuneTask()
{
	if (UIU_HealthSystemComponent* _health = boss->GetHealthComponent())
	{
		_health->SetCanTakeDamages(true);
	}
	else
	{
		LOG("UBT_TN_ImmuneEnd::ImmuneTask -> Boss health component is invalid");
	}
}

void UBT_TN_ImmuneEnd::PostInitProperties()
{
	Super::PostInitProperties();

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	NodeName = FString("Immune End");
#endif
}

FString UBT_TN_ImmuneEnd::GetStaticDescription() const
{
	return FString("End the Boss Immunity");
}