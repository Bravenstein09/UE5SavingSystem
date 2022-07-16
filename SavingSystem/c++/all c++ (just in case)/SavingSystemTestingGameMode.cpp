// Copyright Epic Games, Inc. All Rights Reserved.

#include "SavingSystemTestingGameMode.h"
#include "SavingSystemTestingCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASavingSystemTestingGameMode::ASavingSystemTestingGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
