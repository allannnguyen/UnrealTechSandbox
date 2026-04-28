// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TECHSANDBOX_API UInventory : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int InventorySize = 12;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemData> Inventory;

public:	
	// Sets default values for this component's properties
	UInventory();

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	bool AddItem(const FItemData& Item);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(const FName& ItemName);

	UFUNCTION(BlueprintCallable)
	bool HasItem(const FName& ItemName) const;

	const TArray<FItemData>& GetItems() const { return Inventory; }

	UFUNCTION(BlueprintPure, Category = "Inventory")
	TArray<FItemData> GetItemsCopy() const { return Inventory; }

	UFUNCTION(BlueprintPure, Category = "Inventory")
	int32 GetInventorySize() const { return InventorySize; }

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
