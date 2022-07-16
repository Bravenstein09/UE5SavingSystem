// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SAVINGSYSTEMTESTING_API ISaveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//get a unique name to use when saving |||||| Expect you to return a unique name to be used when saving information about your project
	UFUNCTION(BlueprintNativeEvent, Category = "SaveInterface")
	FString GetUniqueSaveName();

	//called right before the owning actor is saved
	UFUNCTION(BlueprintNativeEvent, Category = "SaveInterface")
	void OnBeforeSave();

};
