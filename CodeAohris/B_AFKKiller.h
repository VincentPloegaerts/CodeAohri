#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "B_AFKKiller.generated.h"

class UP_PatternCaster;
class AB_AI_AFKKiller;

UCLASS()
class INSIDEUS_API AB_AFKKiller : public APawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "AFK Killer")
	UP_PatternCaster* patternCaster = nullptr;

	UPROPERTY()
	AB_AI_AFKKiller* aiAFKKiller = nullptr;

public:
	FORCEINLINE AB_AI_AFKKiller* GetAIAFKKiller() const { return aiAFKKiller; }
	FORCEINLINE UP_PatternCaster* GetPatternCaster() const { return patternCaster; }
	
public:
	AB_AFKKiller();

protected:
	virtual void BeginPlay() override;

};