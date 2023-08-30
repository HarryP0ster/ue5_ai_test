// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_controller.h"
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Perception/AISense_Sight.h>
#include <Perception/AISense_Hearing.h>
#include <Perception/AISense_Damage.h>
#include <Perception/AISenseConfig_Sight.h>
#include <Perception/AISenseConfig_Hearing.h>
#include <Perception/AISenseConfig_Damage.h>
#include <Perception/AIPerceptionComponent.h>
#include <Perception/AIPerceptionTypes.h>
#include <UObject/ScriptDelegates.h>
#include "AI_test/AI_testCharacter.h"
#include <Kismet/GameplayStatics.h>
#include <Sound/SoundWave.h>
#include <BrainComponent.h>

#define StartBrainless

AAI_controller::AAI_controller(const FObjectInitializer& initializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("/Script/AIModule.BehaviorTree'/Game/ThirdPerson/AI/BT.BT'"));
	static ConstructorHelpers::FObjectFinder<USoundWave> audioRes(TEXT("/Script/Engine.SoundWave'/Game/LevelPrototyping/Sounds/alert.alert'"));

	if (obj.Succeeded())
		_btree = obj.Object;

	if (audioRes.Succeeded())
		soundCue = audioRes.Object;

	_btreecomp = initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("Behavior"));
	Blackboard = initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("Blackboard"));
	SetPerceptionComponent(*initializer.CreateDefaultSubobject<UAIPerceptionComponent>(this, TEXT("AIPerception Component")));

	_sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	_sight->SightRadius = 3000.f;
	_sight->NearClippingRadius = 0.f;
	_sight->PeripheralVisionAngleDegrees = 90.f;
	_sight->LoseSightRadius = 3500.f;
	_sight->AutoSuccessRangeFromLastSeenLocation = 500.f;
	_sight->DetectionByAffiliation.bDetectEnemies = true;
	_sight->DetectionByAffiliation.bDetectNeutrals = false;
	_sight->DetectionByAffiliation.bDetectFriendlies = false;
	_sight->SetMaxAge(0.5f);
	GetAIPerceptionComponent()->SetDominantSense(*_sight->GetSenseImplementation());
	GetAIPerceptionComponent()->ConfigureSense(*_sight);
	GetAIPerceptionComponent()->SetSenseEnabled(TSubclassOf<UAISense_Sight>(), true);

	_hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	_hearing->HearingRange = 5000.f;
	_hearing->DetectionByAffiliation.bDetectEnemies = true;
	_hearing->DetectionByAffiliation.bDetectNeutrals = true;
	_hearing->DetectionByAffiliation.bDetectFriendlies = false;
	_hearing->SetMaxAge(0.5f);
	GetAIPerceptionComponent()->ConfigureSense(*_hearing);
	GetAIPerceptionComponent()->SetSenseEnabled(TSubclassOf<UAISense_Hearing>(), true);

	_damage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	_damage->SetMaxAge(1.f);
	GetAIPerceptionComponent()->ConfigureSense(*_damage);
	GetAIPerceptionComponent()->SetSenseEnabled(TSubclassOf<UAISense_Damage>(), true);
	
	FScriptDelegate d{};
	d.BindUFunction(this, TEXT("OnPerceptionUpdated"));
	GetAIPerceptionComponent()->OnTargetPerceptionUpdated.Add(d);

	SetGenericTeamId(FGenericTeamId(_team));
}

void AAI_controller::BeginPlay()
{
	Super::BeginPlay();

	if (_btree)
	{
		RunBehaviorTree(_btree);
		_btreecomp->StartTree(*_btree);
	}

#ifdef StartBrainless
	SetActivation(false);
#endif
}

void AAI_controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (_btree != nullptr && Blackboard != nullptr)
		Blackboard->InitializeBlackboard(*_btree->BlackboardAsset);

	_patrolPoint = InPawn->GetActorLocation();

	FScriptDelegate d{};
	d.BindUFunction(this, TEXT("ProcessDamage"));
	InPawn->OnTakeAnyDamage.Add(d);
}

void AAI_controller::SetActivation(bool state)
{
	if (state)
	{
		PerceptionComponent->Activate(true);
		_btreecomp->StartTree(*_btree);
	}
	else
	{
		StopMovement();
		PerceptionComponent->Deactivate();
		_btreecomp->StopTree();
	}
}

ETeamAttitude::Type AAI_controller::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other)) 
	{
		if (OtherPawn->GetController()->GetInterfaceAddress(IGenericTeamAgentInterface::UClassType::StaticClass()))
		{
			if (Cast<IGenericTeamAgentInterface>(OtherPawn->GetController())->GetGenericTeamId() == GetGenericTeamId())
			{
				return ETeamAttitude::Friendly;
			}
			else
			{
				return ETeamAttitude::Hostile;
			}
		}
	}

	return ETeamAttitude::Neutral;
}

void AAI_controller::ProcessDamage(AActor* self, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCause)
{
	UAISense_Damage::ReportDamageEvent(GetWorld(), self, DamageCause, Damage, FVector(), FVector());

	_health -= Damage;

	if (_health <= 0)
	{
		GetPawn()->Destroy();
	}
}

void AAI_controller::OnPerceptionUpdated(AActor* actor, FAIStimulus stimulus)
{

	if (!PerceptionComponent->IsActive()) return;

	switch (stimulus.Type)
	{
	case 0:
		Blackboard->SetValueAsBool(TEXT("SeesPlayer"), stimulus.WasSuccessfullySensed());
		if (soundCue && !Blackboard->GetValueAsBool("ChaseActive"))
			UGameplayStatics::PlaySound2D(GetWorld(), soundCue, 0.35f);
		Blackboard->SetValueAsBool(TEXT("ChaseActive"), true);
		_target = actor;
		break;
	case 1:
		if (actor != GetPawn() 
			&& Cast<IGenericTeamAgentInterface>(Cast<APawn>(actor)->GetController())->GetGenericTeamId() != GetGenericTeamId())
		{
			Blackboard->SetValueAsBool(TEXT("HearsPlayer"), stimulus.WasSuccessfullySensed());
			Blackboard->SetValueAsVector(TEXT("PointOfInterest"), actor->GetActorLocation());
			_target = actor;
		}
		break;
	case 2:
		Blackboard->SetValueAsBool(TEXT("SeesPlayer"), true);
		if (soundCue && !Blackboard->GetValueAsBool("ChaseActive"))
			UGameplayStatics::PlaySound2D(GetWorld(), soundCue, 0.35f);
		Blackboard->SetValueAsBool(TEXT("ChaseActive"), true);
		break;
	default:
		break;
	}
}