// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include <BehaviorTree/BehaviorTreeTypes.h>
#include "FindRandomLocation.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UFindRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UFindRandomLocation(const FObjectInitializer& initializer = FObjectInitializer::Get());
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& comp, uint8_t* node);

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float _search_radius = 2000.f;
};
