// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

/**
 * 
 */
class IInteractable : public AActor
{
public:
	IInteractable();
	~IInteractable();

	virtual void Interract() = 0;
};
