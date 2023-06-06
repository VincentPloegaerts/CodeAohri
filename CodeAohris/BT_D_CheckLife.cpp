#include "BT_D_CheckLife.h"

#include "IU_Macros.h"


bool UBT_D_CheckLife::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	//Declaration Check in Child class
	return false;
}

bool UBT_D_CheckLife::CheckLife(const float& _life) const
{
	if (checkType == ABOVE)
		return CheckAbove(_life);
	if (checkType == UNDER)
		return CheckUnder(_life);
	return CheckBetween(_life);
}

bool UBT_D_CheckLife::CheckAbove(const float& _life) const
{
	return _life >= (bPercent ? checkPercent : checkValue);
}

bool UBT_D_CheckLife::CheckUnder(const float& _life) const
{
	return _life <= (bPercent ? checkPercent : checkValue);
}

bool UBT_D_CheckLife::CheckBetween(const float& _life) const
{
	if (bPercent)
		return _life <= maxCheckPercent && _life > minCheckPercent;
	return _life <= maxCheckValue && _life > minCheckValue;
}

FString UBT_D_CheckLife::GetStaticDescription() const
{
	const FString& _baseStr = FString("Check if ") + GetStaticSpecificDescription() + FString(" is ");

	FString _checkStr;
	
	if (checkType == ECheckEnum::ABOVE)
		_checkStr = FString("ABOVE ");
	else if (checkType == ECheckEnum::UNDER)
		_checkStr = FString("UNDER ");
	else
		_checkStr = FString("BETWEEN ");

	FString _endStr;
	
	if (checkType == ECheckEnum::BETWEEN)
		_endStr = bPercent ? FString::FromInt(minCheckPercent * 100.f) + FString("% ") + FString(" - ") + FString::FromInt(maxCheckPercent * 100.0f) + FString("% ") : FString::FromInt(minCheckValue) + FString(" - ") + FString::FromInt(maxCheckValue) + FString(" ");		
	else
		_endStr = bPercent ? FString::FromInt(checkPercent * 100.f) + FString("% ") : FString::FromInt(checkValue) + FString(" ");

	return _baseStr + _checkStr + _endStr;
}

FString UBT_D_CheckLife::GetStaticSpecificDescription() const
{
	return FString("NONE");
}
#if WITH_EDITOR
void UBT_D_CheckLife::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.Property->GetFName() == FName("minCheckPercent"))
	{
		if (minCheckPercent > maxCheckPercent)
		{
			maxCheckPercent = minCheckPercent + 0.1f;
			maxCheckPercent = maxCheckPercent > 1.0f ? 1.0f : maxCheckPercent;
		}
	}
	if (PropertyChangedEvent.Property->GetFName() == FName("maxCheckPercent"))
	{
		if (maxCheckPercent < minCheckPercent )
		{
			minCheckPercent = maxCheckPercent - 0.1f;
			minCheckPercent = minCheckPercent < 0.0f ? 0 : minCheckPercent;	
		}
	}

	if (PropertyChangedEvent.Property->GetFName() == FName("minCheckValue"))
	{
		if (minCheckValue > maxCheckValue)
			maxCheckValue = minCheckValue + 1;
	}
	if (PropertyChangedEvent.Property->GetFName() == FName("maxCheckValue"))
	{
		if (maxCheckValue < minCheckValue )
			minCheckValue = maxCheckValue - 1;
	}
}
#endif
