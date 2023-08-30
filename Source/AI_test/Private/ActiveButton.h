// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UInteractable.h"
#include "ActiveButton.generated.h"

UCLASS()
class AActiveButton : public AActor, public IUInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActiveButton();
	void Interract() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowProtectedAccess = "true"))
	UStaticMeshComponent* mesh;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool state = true;
};
