// Fill out your copyright notice in the Description page of Project Settings.


#include "ChangeSpeedService.h"
#include "AI_controller.h"
#include "AI_test/AI_testCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UChangeSpeedService::UChangeSpeedService()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Set speed");
}

void UChangeSpeedService::OnBecomeRelevant(UBehaviorTreeComponent& owner, uint8_t* node)
{
	Super::OnBecomeRelevant(owner, node);
	Cast<AAI_testCharacter>(owner.GetAIOwner()->GetPawn())->GetCharacterMovement()->MaxWalkSpeed = _speed;
}

FString UChangeSpeedService::GetStaticServiceDescription() const
{
	return FString("Update actor speed");
}
