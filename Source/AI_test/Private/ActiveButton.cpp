// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveButton.h"
#include "Kismet/GameplayStatics.h"
#include "AI_test/AI_testCharacter.h"
#include "AI_test/Private/AI_controller.h"

// Sets default values
AActiveButton::AActiveButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

// Called when the game starts or when spawned
void AActiveButton::BeginPlay()
{
	Super::BeginPlay();
	
}

void AActiveButton::Interract()
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAI_testCharacter::StaticClass(), actors);

	for (const auto& actor : actors)
	{
		const auto& controller = Cast<AAI_testCharacter>(actor)->GetController();
		if (controller->IsA(AAI_controller::StaticClass()))
		{
			Cast<AAI_controller>(controller)->SetActivation(state);
		}
	}

	state = !state;
}

// Called every frame
void AActiveButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

