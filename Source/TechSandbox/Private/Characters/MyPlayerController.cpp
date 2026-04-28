// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Items/Inventory.h"
#include "Items/InventoryWidget.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (PlayerMappingContext)
			{
				InputSubsystem->AddMappingContext(PlayerMappingContext, PlayerMappingPriority);
			}
		}
	}
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (ToggleInventoryAction)
		{
			EnhancedInput->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &AMyPlayerController::HandleToggleInventoryAction);
		}
	}
}

void AMyPlayerController::ToggleInventory()
{
	if (!InventoryWidget && InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(this, InventoryWidgetClass);
	}
	
	if (!InventoryWidget) return;

	if (APawn* ControlledPawn = GetPawn())
	{
		if (UInventory* InventoryComponent = ControlledPawn->FindComponentByClass<UInventory>())
		{
			InventoryWidget->InitializeWithInventory(InventoryComponent);
		}
	}
	
	if (InventoryWidget->IsInViewport())
	{
		InventoryWidget->RemoveFromParent();
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
	else
	{
		InventoryWidget->AddToViewport();
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(InputMode);
		bShowMouseCursor = true;
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void AMyPlayerController::HandleToggleInventoryAction()
{
	ToggleInventory();
}
