// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SAVINGSYSTEMTESTING_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	/** Virtual function to allow custom gameinstances an opprotunity to set up what it needs*/
	virtual void Init() override;

};
