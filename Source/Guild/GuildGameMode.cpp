// Copyright Epic Games, Inc. All Rights Reserved.

#include "GuildGameMode.h"
#include "GuildCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGuildGameMode::AGuildGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
