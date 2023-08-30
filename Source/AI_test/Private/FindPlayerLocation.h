// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include <BehaviorTree/BehaviorTreeTypes.h>
#include "FindPlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class UFindPlayerLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UFindPlayerLocation(const FObjectInitializer& initializer = FObjectInitializer::Get());
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& comp, uint8_t* node);
};
