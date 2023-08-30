// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "AI_controller.generated.h"

/**
 * 
 */
UCLASS()
class AAI_controller : public AAIController
{
	GENERATED_BODY()
public:

	AAI_controller(const FObjectInitializer& initializer = FObjectInitializer::Get());
	AAI_controller(AAI_controller& other) = delete;

	void BeginPlay() override;
	void OnPossess(APawn* InPawn) override;
	void SetActivation(bool state);

	inline UBlackboardComponent* GetBlackboard() const { return Blackboard; };

	inline const AActor* GetTargetActor() const { return _target; };

	FGenericTeamId GetGenericTeamId() const override { return _team; };

	const FVector& GetPatrolingPoint() const { return _patrolPoint; };

protected:
	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

private:
	UFUNCTION()
	void OnPerceptionUpdated(AActor* actor, FAIStimulus stimulus);

	UFUNCTION()
	void ProcessDamage(AActor* self, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCause);

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* _btreecomp;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* _btree;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Sight* _sight;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Hearing* _hearing;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Damage* _damage;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class USoundWave* soundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay", meta = (AllowPrivateAccess = "true"))
	uint8 _team = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay", meta = (AllowPrivateAccess = "true"))
	float _health = 100.f;

	class AActor* _target;

	FVector _patrolPoint;
};
