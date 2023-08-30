// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include <BehaviorTree/BehaviorTreeTypes.h>
#include "StartMoving.generated.h"

/**
 * 
 */
UCLASS()
class UStartMoving : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UStartMoving(const FObjectInitializer& initializer = FObjectInitializer::Get());
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& comp, uint8_t* node);
};
