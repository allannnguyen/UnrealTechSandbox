// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UInventoryWidget;
class UInputAction;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class TECHSANDBOX_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> PlayerMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	int32 PlayerMappingPriority = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> ToggleInventoryAction;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidget;
	
	UFUNCTION(BlueprintCallable)
	void ToggleInventory();

protected:
	UFUNCTION()
	void HandleToggleInventoryAction();
	
};
