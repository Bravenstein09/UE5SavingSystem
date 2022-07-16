// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "SaveManager.h"

void UMyGameInstance::Init()
{
	Super::Init();
	USaveManager::Init();
}