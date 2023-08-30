// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include <BehaviorTree/BehaviorTreeTypes.h>
#include "ReturnToPatroling.generated.h"

/**
 * 
 */
UCLASS()
class UReturnToPatroling : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UReturnToPatroling();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& comp, uint8_t* node);
};
