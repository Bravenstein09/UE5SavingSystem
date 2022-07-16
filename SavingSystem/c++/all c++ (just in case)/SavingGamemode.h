// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveManager.h"
#include "GameFramework/GameMode.h"
#include "SavingGamemode.generated.h"

/**
 * 
 */
UCLASS()
class SAVINGSYSTEMTESTING_API ASavingGamemode : public AGameMode
{
	GENERATED_BODY()

public: 

	ASavingGamemode();

	virtual void InitGameState() override;

};
