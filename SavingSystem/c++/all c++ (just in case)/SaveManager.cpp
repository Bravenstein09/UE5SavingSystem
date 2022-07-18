// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveManager.h"
#include "SaveGameMetadata.h"
#include "SaveGameData.h"
#include "kismet/GameplayStatics.h"
#include "Engine/Engine.h"

FString USaveManager::CurrentSaveSlot;
TArray<TScriptInterface<ISaveInterface>> USaveManager::SaveInterfaces;

static const FString kMetadataSaveSlot = "SaveGameMetadata";
static const int32 kMaxSaveSlots = 50;

void USaveManager::Init()
{
	CurrentSaveSlot = "Default";

	//make sure the metadata file exists || in case that the game has not been ran before
	USaveGame* saveGameMetadata = UGameplayStatics::LoadGameFromSlot(kMetadataSaveSlot, 0);

	if (saveGameMetadata == nullptr)
	{
		//file doesnt exist so we need to create one
		USaveGame* saveGameObect = UGameplayStatics::CreateSaveGameObject(USaveGameMetadata::StaticClass());

		UGameplayStatics::SaveGameToSlot(saveGameObect, kMetadataSaveSlot, 0);
	}
}

void USaveManager::QueryAllSaveInterfaces()
{
	//clearing the old entries
	SaveInterfaces.Empty();

	//Get all of the actors that implement the save interface
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsWithInterface(GWorld, USaveInterface::StaticClass(), actors);

	for (AActor* actor : actors)
	{
		SaveInterfaces.Add(actor);
	}
}

void USaveManager::SaveGame()
{
	//Create a new save game data instance
	USaveGameData* saveGameData = Cast<USaveGameData>(UGameplayStatics::CreateSaveGameObject(USaveGameData::StaticClass()));

	//Go over all of the actors that want to be saved and save them

	for (auto& saveInterface : SaveInterfaces)
	{
		if (saveInterface.GetObject() == nullptr)
			continue;

		//Let the object know it's about to be saved
		saveInterface->Execute_OnBeforeSave(saveInterface.GetObject());

		//Find unique save data by name
		FString uniqueSaveName = saveInterface->Execute_GetUniqueSaveName(saveInterface.GetObject());
		FSaveData& saveData = saveGameData->SerializedData.Add(uniqueSaveName);

		FMemoryWriter memoryWriter = FMemoryWriter(saveData.Data);
		memoryWriter.ArIsSaveGame = true;

		saveInterface.GetObject()->Serialize(memoryWriter);
	}

	//saving the game to your current slot
	UGameplayStatics::SaveGameToSlot(saveGameData, CurrentSaveSlot, 0);

	//Update the metadata file with the new slot 
	USaveGameMetadata* saveGameMetadata = Cast<USaveGameMetadata>(UGameplayStatics::LoadGameFromSlot(kMetadataSaveSlot, 0));

	FSaveMetadata& saveMetadata = saveGameMetadata->SavedGamesMetadata.FindOrAdd(CurrentSaveSlot);
	saveMetadata.Slotname = CurrentSaveSlot;
	saveMetadata.Date = FDateTime::Now();

	// save the changes to the metadata file
	UGameplayStatics::SaveGameToSlot(saveGameMetadata, kMetadataSaveSlot, 0);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, "Saved: " + CurrentSaveSlot);
}

void USaveManager::LoadGame()
{
	USaveGameData* saveGameData = Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot(CurrentSaveSlot, 0));

	if (saveGameData == nullptr)
	{
		// no saves exist yet for this slot so save a defualt one.
		SaveGame();

		//reload it 
		saveGameData = Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot(CurrentSaveSlot, 0));
	}



	//Loop over all of the actors who need loaded data.
	for (auto& saveInterface : SaveInterfaces)
	{
		if (saveInterface.GetObject() == nullptr)
			continue;

		//Find This object's unique name and save data

		FString uniqueSaveName = saveInterface->Execute_GetUniqueSaveName(saveInterface.GetObject());
		FSaveData* saveData = saveGameData->SerializedData.Find(uniqueSaveName);
		if (saveData == nullptr)
			continue;


		//Deserialize object's data 
		FMemoryReader memoryReader(saveData->Data);
		memoryReader.ArIsSaveGame = false;

		saveInterface.GetObject()->Serialize(memoryReader);
	}



	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, "Loaded: " + CurrentSaveSlot);
}

void USaveManager::DeleteSlot(const FString& slot)
{
	//Delete the desired slots
	UGameplayStatics::DeleteGameInSlot(slot, 0);

	//loading the metadata file
	USaveGameMetadata* saveGameMetadata = Cast<USaveGameMetadata>(UGameplayStatics::LoadGameFromSlot(kMetadataSaveSlot, 0));

	saveGameMetadata->SavedGamesMetadata.Remove(slot);

	//save the metadata file
	UGameplayStatics::SaveGameToSlot(saveGameMetadata, kMetadataSaveSlot, 0);
}

FString USaveManager::GetNewSaveSlot(bool& slot_found)
{
	slot_found = false;

	//loading the metadata file
	USaveGameMetadata* saveGameMetadata = Cast<USaveGameMetadata>(UGameplayStatics::LoadGameFromSlot(kMetadataSaveSlot, 0));

	for (int32 i = 0; i < kMaxSaveSlots; ++i)
	{
		FString slotName = "Save Slot " + FString::FromInt(i);

		if (saveGameMetadata->SavedGamesMetadata.Contains(slotName) == false)
		{
			//we found a free slot !!! return it
			slot_found = true;
			return slotName;
		}
	}

	return FString();

}

void USaveManager::SetCurrentSaveSlot(const FString& slot)
{
	CurrentSaveSlot = slot;
}

FString USaveManager::GetCurrentSaveSlot()
{
	return CurrentSaveSlot;
}

TArray<FSaveMetadata> USaveManager::GetAllSaveMetadata()
{
	TArray<FSaveMetadata> metadata;

	//loading the metadata file
	USaveGameMetadata* saveGameMetadata = Cast<USaveGameMetadata>(UGameplayStatics::LoadGameFromSlot(kMetadataSaveSlot, 0));

	metadata.Reserve(saveGameMetadata->SavedGamesMetadata.Num());

	//add each saved game's metadata to the array 
	for (const auto& saved_game : saveGameMetadata->SavedGamesMetadata)
	{
		metadata.Push(saved_game.Value);

	}

	return metadata;
}

void USaveManager::SetCurrentLevel(const FName& slot)
{
	//Update the metadata file with the new level
	USaveGameMetadata* saveGameMetadata = Cast<USaveGameMetadata>(UGameplayStatics::LoadGameFromSlot(kMetadataSaveSlot, 0));
	FSaveMetadata& saveMetadata = saveGameMetadata->SavedGamesMetadata.FindOrAdd(CurrentSaveSlot);

	saveMetadata.CurrentLevel = slot;

	// save the changes to the metadata file
	UGameplayStatics::SaveGameToSlot(saveGameMetadata, kMetadataSaveSlot, 0);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, "CurrentLevelSet " + CurrentSaveSlot);
}
