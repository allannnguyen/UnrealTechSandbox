// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inventory.h"

// Sets default values for this component's properties
UInventory::UInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventory::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UInventory::AddItem(const FItemData& Item)
{
	if (Inventory.Num() < InventorySize)
	{
		Inventory.Add(Item);
		OnInventoryChanged.Broadcast();
		return true;
	}
	return false;
}

bool UInventory::RemoveItem(const FName& ItemName)
{
	int lastFoundIndex = INDEX_NONE;
	for (int i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i].Name == ItemName)
		{
			lastFoundIndex = i;
		}
	}
	
	if (lastFoundIndex == INDEX_NONE) return false;
	
	FItemData& item = Inventory[lastFoundIndex];
	
	item.Amount--;
	
	if (item.Amount == 0)
	{
		Inventory.RemoveAt(lastFoundIndex);
	}

	OnInventoryChanged.Broadcast();
	return true;
}

bool UInventory::HasItem(const FName& ItemName) const
{
	for (const FItemData& curItem : Inventory)
	{
		if (ItemName == curItem.Name) { return true; }
	}
	return false;
}



// Called every frame
void UInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

