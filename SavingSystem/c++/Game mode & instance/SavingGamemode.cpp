// Fill out your copyright notice in the Description page of Project Settings.


#include "SavingGamemode.h"
#include "SaveManager.h"
#include "UObject/ConstructorHelpers.h"

ASavingGamemode::ASavingGamemode()
{
	// we should like spawn the player where the player start is but i literlaly hate alsdf;as
}

void ASavingGamemode::InitGameState()
{
	USaveManager::QueryAllSaveInterfaces();
	USaveManager::LoadGame();

	Super::InitGameState();
}
