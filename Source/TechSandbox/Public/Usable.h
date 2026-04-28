// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Usable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUsable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TECHSANDBOX_API IUsable
{
	GENERATED_BODY()

public:
	virtual void SecondaryAction();
	virtual void PrimaryAction();
};
