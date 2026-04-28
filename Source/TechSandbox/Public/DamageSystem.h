// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageInfo.h"
#include "Components/ActorComponent.h"
#include "DamageSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TECHSANDBOX_API UDamageSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamageSystem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 100.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.0;
	bool IsDead = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	float Heal(float Amount);

	UFUNCTION(BlueprintCallable)
	bool TakeDamage(FDamageInfo DamageInfo);

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
