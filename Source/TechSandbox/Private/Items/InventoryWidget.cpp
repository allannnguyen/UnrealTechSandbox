// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/InventoryWidget.h"
#include "Items/Inventory.h"

void UInventoryWidget::InitializeWithInventory(UInventory* InInventoryComponent)
{
	if (InventoryComponent == InInventoryComponent)
	{
		RefreshInventory();
		return;
	}

	if (InventoryComponent)
	{
		InventoryComponent->OnInventoryChanged.RemoveDynamic(this, &UInventoryWidget::HandleInventoryChanged);
	}

	InventoryComponent = InInventoryComponent;

	if (InventoryComponent)
	{
		InventoryComponent->OnInventoryChanged.AddUniqueDynamic(this, &UInventoryWidget::HandleInventoryChanged);
	}

	RefreshInventory();
}

void UInventoryWidget::RefreshInventory()
{
	const TArray<FItemData> Items = InventoryComponent ? InventoryComponent->GetItems() : TArray<FItemData>();
	BP_RebuildInventory(Items);
}

void UInventoryWidget::HandleInventoryChanged()
{
	RefreshInventory();
}

void UInventoryWidget::NativeDestruct()
{
	if (InventoryComponent)
	{
		InventoryComponent->OnInventoryChanged.RemoveDynamic(this, &UInventoryWidget::HandleInventoryChanged);
	}

	Super::NativeDestruct();
}

