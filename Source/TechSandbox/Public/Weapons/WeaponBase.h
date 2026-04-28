// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UENUM(BlueprintType)
enum class ESecondaryActionType : uint8
{
	None,
	Zoom,
	Draw,
	Block,
	Charge
};

UCLASS()
class TECHSANDBOX_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UTexture2D> WeaponIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName SocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	ESecondaryActionType SecondaryActionType = ESecondaryActionType::None;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bIsUsingSecondaryAction = false;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void StartPrimaryAction();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool CanStartSecondaryAction() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void StartSecondaryAction();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void StopSecondaryAction();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
