// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TECHSANDBOX_API FDamageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount = 0.0f;
	
};
