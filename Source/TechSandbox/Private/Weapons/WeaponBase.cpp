// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::StartPrimaryAction_Implementation()
{
	
}

bool AWeaponBase::CanStartSecondaryAction_Implementation() const
{
	return SecondaryActionType != ESecondaryActionType::None && !bIsUsingSecondaryAction;
}

void AWeaponBase::StartSecondaryAction_Implementation()
{
	bIsUsingSecondaryAction = true;
}

void AWeaponBase::StopSecondaryAction_Implementation()
{
	bIsUsingSecondaryAction = false;
}



