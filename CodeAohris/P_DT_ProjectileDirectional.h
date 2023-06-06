#pragma once

#include "CoreMinimal.h"
#include "P_DT_ProjectileGlobal.h"
#include "P_DT_ProjectileDirectional.generated.h"


UCLASS()
class INSIDEUS_API UP_DT_ProjectileDirectional : public UP_DT_ProjectileGlobal
{
	GENERATED_BODY()

#if WITH_EDITOR
	virtual void SaveProjectileData() override;
	virtual void LoadProjectileData() override;
#endif
};
