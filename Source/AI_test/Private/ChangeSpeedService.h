// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "ChangeSpeedService.generated.h"

/**
 * 
 */
UCLASS()
class UChangeSpeedService : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UChangeSpeedService();
	void OnBecomeRelevant(UBehaviorTreeComponent& owner, uint8_t* node);
protected:
	FString GetStaticServiceDescription() const override;
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float _speed = 200.f;
};
