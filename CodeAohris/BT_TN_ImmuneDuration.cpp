#include "BT_TN_ImmuneDuration.h"

#include "B_Boss.h"
#include "IU_HealthSystemComponent.h"
#include "IU_Macros.h"

void UBT_TN_ImmuneDuration::ImmuneTask()
{
	health = boss->GetHealthComponent();
	if (health)
	{
		health->SetCanTakeDamages(false);
	}
	else
	{
		LOG("UBT_TN_ImmuneDuration::ImmuneTask -> Boss health component is invalid");
		return;
	}
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UBT_TN_ImmuneDuration::StopImmune, fImmuneTime);
}

void UBT_TN_ImmuneDuration::StopImmune()
{
	if (health)
	{
		health->SetCanTakeDamages(true);
	}
}

void UBT_TN_ImmuneDuration::PostInitProperties()
{
	Super::PostInitProperties();

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	NodeName = FString("Immune Duration");
#endif
}
FString UBT_TN_ImmuneDuration::GetStaticDescription() const
{
	return FString("Immune the boss for ") + FString::SanitizeFloat(fImmuneTime, 2) + FString(" sec ");
}