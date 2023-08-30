// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMoving.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Object.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Vector.h>
#include "AI_controller.h"

UStartMoving::UStartMoving(const FObjectInitializer& initializer)
{

}

EBTNodeResult::Type UStartMoving::ExecuteTask(UBehaviorTreeComponent& comp, uint8_t* node)
{
	AAI_controller& controller = *Cast<AAI_controller>(comp.GetAIOwner());

	if (BlackboardKey.SelectedKeyType.GetDefaultObject()->IsA(UBlackboardKeyType_Object::StaticClass()))
	{
		AActor* player = Cast<AActor>(controller.GetBlackboard()->GetValueAsObject(GetSelectedBlackboardKey()));

		if (player)
		{
			controller.MoveToActor(player);
		}
	}
	else if (BlackboardKey.SelectedKeyType.GetDefaultObject()->IsA(UBlackboardKeyType_Vector::StaticClass()))
	{
		FVector target = controller.GetBlackboard()->GetValueAsVector(GetSelectedBlackboardKey());
		controller.MoveToLocation(target);
	}


	return EBTNodeResult::Succeeded;
}
