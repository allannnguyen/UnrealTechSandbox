// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "RangedWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TECHSANDBOX_API ARangedWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Weapon")
	float ProjectileSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Weapon")
	TObjectPtr<USoundCue> FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Weapon")
	TObjectPtr<USoundCue> ReloadSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Weapon")
	int AmmoCapacity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Weapon")
	int CurrentAmmo;
};
