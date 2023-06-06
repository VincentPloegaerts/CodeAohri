#include "BT_TN_ImmuneStart.h"

#include "B_Boss.h"
#include "IU_HealthSystemComponent.h"
#include "IU_Macros.h"

void UBT_TN_ImmuneStart::ImmuneTask()
{
	if (UIU_HealthSystemComponent* _health = boss->GetHealthComponent())
	{
		_health->SetCanTakeDamages(false);
	}
	else
	{
		LOG("UBT_TN_ImmuneStart::ImmuneTask -> Boss health component is invalid");
	}
}

void UBT_TN_ImmuneStart::PostInitProperties()
{
	Super::PostInitProperties();
	
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	NodeName = FString("Immune Start");
#endif
}

FString UBT_TN_ImmuneStart::GetStaticDescription() const
{
	return FString("Start the Boss Immunity");
}