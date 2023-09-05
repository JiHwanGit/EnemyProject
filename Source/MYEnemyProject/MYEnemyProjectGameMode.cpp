// Copyright Epic Games, Inc. All Rights Reserved.

#include "MYEnemyProjectGameMode.h"
#include "MYEnemyProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMYEnemyProjectGameMode::AMYEnemyProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
