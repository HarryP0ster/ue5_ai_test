// Copyright Epic Games, Inc. All Rights Reserved.

#include "AI_testGameMode.h"
#include "AI_testCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAI_testGameMode::AAI_testGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
