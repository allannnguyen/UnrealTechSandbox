// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TECHSANDBOX_API FItemData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool InInventory = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Description = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Stackable = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStackSize = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Image = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Healable = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HealAmount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemActorClass = nullptr;
};
