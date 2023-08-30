// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameMode.h"
#include "PlayerControllerTeamed.h"

ACustomGameMode::ACustomGameMode()
{
	PlayerControllerClass = APlayerControllerTeamed::StaticClass();
}
