// Fill out your copyright notice in the Description page of Project Settings.


#include "FindRandomLocation.h"
#include <Runtime/NavigationSystem/Public/NavigationSystem.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Object.h>
#include "AI_controller.h"

UFindRandomLocation::UFindRandomLocation(const FObjectInitializer& initializer)
{
	
}

EBTNodeResult::Type UFindRandomLocation::ExecuteTask(UBehaviorTreeComponent& comp, uint8_t* node)
{
	const auto& controller = *Cast<AAI_controller>(comp.GetAIOwner());
	const FVector& origin = controller.GetPatrolingPoint();
	FNavLocation target;

	const auto& navigation = *UNavigationSystemV1::GetCurrent(GetWorld());
	if (navigation.GetRandomPointInNavigableRadius(origin, _search_radius, target))
	{
		controller.GetBlackboard()->SetValueAsVector(TEXT("PointOfInterest"), target.Location);
	}

	return EBTNodeResult::Succeeded;
}
