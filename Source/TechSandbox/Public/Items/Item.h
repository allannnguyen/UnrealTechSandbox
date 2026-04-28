// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "ItemData.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class TECHSANDBOX_API AItem : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AItem();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FItemData ItemData;

	UFUNCTION(BlueprintPure, Category = "Item")
	const FItemData& GetItemData() const { return ItemData; }

	virtual void Interact_Implementation(APawn* Interactor) override;
	virtual bool CanInteract_Implementation(APawn* Interactor) const override;
	virtual FText GetInteractionText_Implementation() const override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
