// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventory;

/**
 * 
 */
UCLASS()
class TECHSANDBOX_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InitializeWithInventory(UInventory* InInventoryComponent);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RefreshInventory();

	UFUNCTION(BlueprintPure, Category = "Inventory")
	UInventory* GetInventoryComponent() const { return InventoryComponent; }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UInventory> InventoryComponent = nullptr;

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void BP_RebuildInventory(const TArray<FItemData>& Items);

	UFUNCTION()
	void HandleInventoryChanged();

	virtual void NativeDestruct() override;
	
};
