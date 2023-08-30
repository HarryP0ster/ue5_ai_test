// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerTeamed.h"

APlayerControllerTeamed::APlayerControllerTeamed()
	: APlayerController()
{
	TeamId = FGenericTeamId(5);
}

FGenericTeamId APlayerControllerTeamed::GetGenericTeamId() const
{
	return TeamId;
}
