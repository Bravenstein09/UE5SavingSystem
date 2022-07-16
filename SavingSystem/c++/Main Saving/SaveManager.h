// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveMetadata.h"
#include "SaveInterface.h"
#include "UObject/NoExportTypes.h"
#include "SaveManager.generated.h"

/**
 * 
 */
UCLASS()
class SAVINGSYSTEMTESTING_API USaveManager : public UObject
{
	GENERATED_BODY()

private:
	//the current save slot
	static FString CurrentSaveSlot;

	static TArray<TScriptInterface<ISaveInterface>> SaveInterfaces;

public:
	//initialize the class, must be done when the game first launches
	static void Init();

	//goes through all of the actors that have the save interface or need to be saved.
	static void QueryAllSaveInterfaces();


	//saves the current state of the game
	UFUNCTION(BlueprintCallable, Category = "Saving and Loading")
		static void SaveGame();


	//loads the saved state of the game
	UFUNCTION(BlueprintCallable, Category = "Saving and Loading")
		static void LoadGame();

	//deletes the specified slot
	UFUNCTION(BlueprintCallable, Category = "Saving and Loading")
		static void DeleteSlot(const FString& slot);

	//gets a new, unused slot
	UFUNCTION(BlueprintPure, Category = "Saving and Loading")
		static UPARAM(DisplayName = "Slot") FString GetNewSaveSlot(bool& slot_found);

	// sets the current save slot to the specified value
	UFUNCTION(BlueprintCallable, Category = "Saving and Loading")
		static void SetCurrentSaveSlot(const FString& slot);

	// gets the current save slot to the specified value
	UFUNCTION(BlueprintPure, Category = "Saving and Loading")
		static FString GetCurrentSaveSlot();

	//gets all the saved games
	UFUNCTION(BlueprintPure, Category = "Saving and Loading")
		static TArray<FSaveMetadata> GetAllSaveMetadata();
};
