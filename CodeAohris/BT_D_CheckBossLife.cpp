#include "BT_D_CheckBossLife.h"

#include "BT_TN_CastPattern.h"
#include "BT_TN_CastPatternSerie.h"
#include "B_Boss.h"
#include "IU_HealthSystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "IU_Macros.h"
#include "BehaviorTree/BTCompositeNode.h"

UBT_D_CheckBossLife::UBT_D_CheckBossLife()
{
	bNotifyActivation = true;
	bNotifyDeactivation = true;
}

void UBT_D_CheckBossLife::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	Super::OnNodeActivation(SearchData);
	
	if (const UObject* _object = SearchData.OwnerComp.GetBlackboardComponent()->GetValueAsObject("Boss"))
	{
		if (const AB_Boss* _boss = Cast<AB_Boss>(_object))
		{
			if (UIU_HealthSystemComponent* _bossHealthComponent = _boss->GetHealthComponent())
			{
				_bossHealthComponent->OnLifeUpdatedWithMaxLife()->AddDynamic(this, &UBT_D_CheckBossLife::ReCalculateLifeCondition);
				
				if (UBTCompositeNode* _node = GetParentNode())
				{
					TArray<FBTCompositeChild>& _children = _node->Children;

					const uint& _max = _children.Num();					
					for (uint i(0); i < _max; ++i)
					{
						FBTCompositeChild& _compositeChild = _children[i];

						TArray<TObjectPtr<UBTDecorator>>& _decorator = _compositeChild.Decorators;
						const uint& _max2 = _decorator.Num();
						for (uint j(0); j < _max2; ++j)
						{
							if (const UBT_D_CheckBossLife* _checkBossLife = Cast<UBT_D_CheckBossLife>(_decorator[j]))
							{
								if (GetExecutionIndex() == _checkBossLife->GetExecutionIndex())
								{
									serieOwner = Cast<UBT_TN_CastPatternSerie>(_compositeChild.ChildTask);
									if (serieOwner)
									{
										onCheckLifeConditionTrigger.AddDynamic(serieOwner, &UBT_TN_CastPatternSerie::SkipSerie);
									}
									else
									{
										patternOwner = Cast<UBT_TN_CastPattern>(_compositeChild.ChildTask);
										if (patternOwner)
										{
											onCheckLifeConditionTrigger.AddDynamic(patternOwner, &UBT_TN_CastPattern::SkipPattern);
										}
									}
									return;
								}
							}
						}
					}
				}
			}
		}
	}
}

bool UBT_D_CheckBossLife::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (const UObject* _object = OwnerComp.GetBlackboardComponent()->GetValueAsObject("Boss"))
	{
		if (const AB_Boss* _boss = Cast<AB_Boss>(_object))
		{
			if (const UIU_HealthSystemComponent* _bossHealthComponent = _boss->GetHealthComponent())
			{
				return CheckLife(bPercent ? _bossHealthComponent->GetHealthPercent() : _bossHealthComponent->GetHealth());
			}
			else
				LOG("UBT_D_CheckBossLife::CalculateRawConditionValue -> Health Component on Boss not valid")
		}
		else
			LOG("UBT_D_CheckBossLife::CalculateRawConditionValue -> Object Key is not the Boss")
	}
	else
		LOG("UBT_D_CheckBossLife::CalculateRawConditionValue -> Key Name not valid")
	
	return false;
}

void UBT_D_CheckBossLife::OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult)
{
	Super::OnNodeDeactivation(SearchData, NodeResult);
	if (const UObject* _object = SearchData.OwnerComp.GetBlackboardComponent()->GetValueAsObject("Boss"))
	{
		if (const AB_Boss* _boss = Cast<AB_Boss>(_object))
		{
			if (UIU_HealthSystemComponent* _bossHealthComponent = _boss->GetHealthComponent())
			{
				_bossHealthComponent->OnLifeUpdatedWithMaxLife()->RemoveDynamic(this, &UBT_D_CheckBossLife::ReCalculateLifeCondition);
			}
		}
	}
	if (serieOwner)
		onCheckLifeConditionTrigger.RemoveDynamic(serieOwner, &UBT_TN_CastPatternSerie::SkipSerie);
	else if (patternOwner)
		onCheckLifeConditionTrigger.RemoveDynamic(patternOwner, &UBT_TN_CastPattern::SkipPattern);
}

void UBT_D_CheckBossLife::ReCalculateLifeCondition(float _actualLife, float _totalLife)
{
	if (!(bPercent ? CheckLife(_actualLife / _totalLife) : CheckLife(_actualLife)))
	{
		onCheckLifeConditionTrigger.Broadcast();
	}
}

void UBT_D_CheckBossLife::PostInitProperties()
{
	Super::PostInitProperties();

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		NodeName = FString("Check Boss Life");
#endif
}

FString UBT_D_CheckBossLife::GetStaticSpecificDescription() const
{
	return FString("BOSS LIFE");
}