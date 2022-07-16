#pragma once

#include "Runtime/CoreUObject/Public/UObject/Class.h"
#include "Runtime/Core/Public/Misc/DateTime.h"
#include "SaveMetadata.generated.h"




USTRUCT(BlueprintType)
struct FSaveMetadata
{
	GENERATED_BODY()

		//name of the save slot
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
		FString Slotname;

	//date the slot was saved
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
		FDateTime Date;
};