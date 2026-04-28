// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageSystem.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UDamageSystem::UDamageSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDamageSystem::BeginPlay()
{
	Super::BeginPlay();
}

float UDamageSystem::Heal(float Amount)
{
	Health = UKismetMathLibrary::FClamp(Health + Amount, 0, MaxHealth);
	return Health;
}
bool UDamageSystem::TakeDamage(FDamageInfo DamageInfo)
{
	Health = UKismetMathLibrary::FClamp(Health - DamageInfo.Amount, 0, MaxHealth);
	IsDead = Health <= 0;
	return true;
}

float UDamageSystem::GetHealth() const
{
	return Health;
}

float UDamageSystem::GetMaxHealth() const
{
	return MaxHealth;
}


// Called every frame
void UDamageSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

