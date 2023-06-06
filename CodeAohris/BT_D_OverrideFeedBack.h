#pragma once

#include "CoreMinimal.h"
#include "STRUCT_TripleDeluxeFeedBack.h"
#include "BehaviorTree/BTDecorator.h"
#include "BT_D_OverrideFeedBack.generated.h"

UCLASS()
class INSIDEUS_API UBT_D_OverrideFeedBack : public UBTDecorator
{
	GENERATED_BODY()

	//UPROPERTY(EditAnywhere)
	//FOverrideFeedBack overrideFeedBack = FOverrideFeedBack();

	UPROPERTY(EditAnywhere)
	FTripleDeluxeFeedBack overrideTripleFeedBack = FTripleDeluxeFeedBack();

public:
	UBT_D_OverrideFeedBack();

private:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;
	
	UFUNCTION() void OverrideCasterFeedBack() const;
};