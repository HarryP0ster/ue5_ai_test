// Fill out your copyright notice in the Description page of Project Settings.


#include "ReturnToPatroling.h"
#include <Runtime/NavigationSystem/Public/NavigationSystem.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "AI_controller.h"

UReturnToPatroling::UReturnToPatroling()
{

}

EBTNodeResult::Type UReturnToPatroling::ExecuteTask(UBehaviorTreeComponent& comp, uint8_t* node)
{
	Cast<AAI_controller>(comp.GetAIOwner())->GetBlackboard()->SetValueAsBool(TEXT("ChaseActive"), false);
	Cast<AAI_controller>(comp.GetAIOwner())->GetBlackboard()->SetValueAsBool(TEXT("HearsPlayer"), false);
	Cast<AAI_controller>(comp.GetAIOwner())->GetBlackboard()->SetValueAsBool(TEXT("SeesPlayer"), false);

	return EBTNodeResult::Succeeded;
}
