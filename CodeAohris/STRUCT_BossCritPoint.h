#pragma once

#include "STRUCT_BossCritPoint.generated.h"

class AB_BossCritPoint;

USTRUCT()
struct FCritPointSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float effect = 0;

	UPROPERTY(EditAnywhere)
	FVector location = FVector::ZeroVector;
	UPROPERTY(EditAnywhere)
	FRotator rotation = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere)
	FVector scale = FVector::ZeroVector;
	UPROPERTY(EditAnywhere)	//Sphere & Capsule
	float radius = 0;
	UPROPERTY(EditAnywhere)	//Capsule
	float height = 0;
	UPROPERTY(EditAnywhere)	//box
	FVector extent = FVector::ZeroVector;
};

USTRUCT()
struct FCritPointData
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	FString actorName = "";

	UPROPERTY(EditAnywhere)
	FName socketName;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AB_BossCritPoint> critPointType;
	UPROPERTY(EditAnywhere)
	FCritPointSettings critPointSettings;

public:
	const FString& GetActorName() const { return actorName; }
	const FName& GetSocketName() const { return socketName; }
	const TSubclassOf<AB_BossCritPoint>& GetCritPointType() const { return critPointType; }
	const FCritPointSettings& GetCritPointSettings() const { return critPointSettings; }
	
public:
	FCritPointData() { }
	FCritPointData(const FName& _socketName, const TSubclassOf<AB_BossCritPoint>& _critPointType)
	{
		socketName = _socketName;
		critPointType = _critPointType;
	}
	FCritPointData(const FName& _socketName, const TSubclassOf<AB_BossCritPoint>& _critPointType, const FCritPointSettings& _critPointSettings)
	{
		socketName = _socketName;
		critPointType = _critPointType;
		critPointSettings = _critPointSettings;
	}

	void SetCritPointSettings(const FCritPointSettings& _critPointSettings)
	{
		critPointSettings = _critPointSettings;
	}

	void SetActorName(FString _name) { actorName = _name; }	
};
