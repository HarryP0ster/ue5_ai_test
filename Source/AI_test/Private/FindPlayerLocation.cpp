// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPlayerLocation.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "AI_controller.h"

UFindPlayerLocation::UFindPlayerLocation(const FObjectInitializer& initializer)
{

}

EBTNodeResult::Type UFindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& comp, uint8_t* node)
{
	AAI_controller& controller = *Cast<AAI_controller>(comp.GetAIOwner());
	controller.GetBlackboard()->SetValueAsVector(TEXT("PointOfInterest"), controller.GetTargetActor()->GetActorLocation());

	return EBTNodeResult::Succeeded;
}
