#pragma once

#include "STRUCT_CustomMath.generated.h"
/*Struct used for rotating an object on an axis.*/
USTRUCT(BlueprintType)
struct FCustomMath
{
	GENERATED_BODY()
	//Return angle in Rad between inputed vectors
	static float AngleBetweenVectors(const FVector& _v1,const FVector& _v2);
	static float SlowInputs(const float& _a,const float& _b,const float& _input)
	{
		return  _b *  FMath::Exp(((_input - _a) * FMath::Loge( 1 / _b)) /  (1 -_a));
	}
};

inline float FCustomMath::AngleBetweenVectors(const FVector& _v1, const FVector& _v2)
{
	return FMath::Acos(_v1.Dot(_v2) / sqrt(_v1.SizeSquared() * _v2.SizeSquared()));
}
