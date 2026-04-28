// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Item.h"
#include "Items/Inventory.h"
#include "TechSandbox/TechSandbox.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Begin Play called!"));

	UWorld* World = GetWorld();
	FVector Location = GetActorLocation();
	FVector Forward = GetActorForwardVector();

	DRAW_LINE(Location, Location + Forward * 100.f)
	DRAW_SPHERE(Location, 10.f)

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Red, TEXT("Begin Play"));
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem::Interact_Implementation(APawn* Interactor)
{
	if (!Interactor)
	{
		return;
	}

	UInventory* InventoryComponent = Interactor->FindComponentByClass<UInventory>();
	if (!InventoryComponent)
	{
		return;
	}

	FItemData ItemToAdd = ItemData;

	if (InventoryComponent->AddItem(ItemToAdd))
	{
		Destroy();
	}
}

bool AItem::CanInteract_Implementation(APawn* Interactor) const
{
	return Interactor && Interactor->FindComponentByClass<UInventory>() != nullptr;
}

FText AItem::GetInteractionText_Implementation() const
{
	return ItemData.Name.IsNone()
		? FText::FromString(TEXT("Pick Up"))
		: FText::Format(FText::FromString(TEXT("Pick Up {0}")), FText::FromName(ItemData.Name));
}
