// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMoving.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "AI_controller.h"

UStartMoving::UStartMoving(const FObjectInitializer& initializer)
{

}

EBTNodeResult::Type UStartMoving::ExecuteTask(UBehaviorTreeComponent& comp, uint8_t* node)
{
	AAI_controller& controller = *Cast<AAI_controller>(comp.GetAIOwner());
	const FVector& target_loc = controller.GetBlackboard()->GetValueAsVector(GetSelectedBlackboardKey());
	controller.MoveToLocation(target_loc);

	return EBTNodeResult::Succeeded;
}
