// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "PlayerControllerTeamed.generated.h"

/**
 * 
 */
UCLASS()
class APlayerControllerTeamed : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	APlayerControllerTeamed();

private:
	FGenericTeamId TeamId;
	FGenericTeamId GetGenericTeamId() const override;
};
