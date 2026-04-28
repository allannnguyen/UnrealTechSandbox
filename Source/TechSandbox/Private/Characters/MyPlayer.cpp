// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/MyPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interactable.h"
#include "Blueprint/UserWidget.h"
#include "Components/PrimitiveComponent.h"
#include "TechSandbox/TechSandbox.h"

AMyPlayer::AMyPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* LocalController = GetLocalViewingPlayerController())
	{
		if (ULocalPlayer* LocalPlayer = LocalController->GetLocalPlayer())
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
}

void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (InteractAction)
		{
			EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &AMyPlayer::Interact);
		}
		if (LeftClickAction)
		{
			EnhancedInput->BindAction(LeftClickAction, ETriggerEvent::Started, this, &AMyPlayer::LeftClick);
		}
		if (RightClickAction)
		{
			EnhancedInput->BindAction(RightClickAction, ETriggerEvent::Started, this, &AMyPlayer::BeginZoom);
			EnhancedInput->BindAction(RightClickAction, ETriggerEvent::Completed, this, &AMyPlayer::EndZoom);
			EnhancedInput->BindAction(RightClickAction, ETriggerEvent::Canceled, this, &AMyPlayer::EndZoom);
		}
	}
}

void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateFocus();
}

void AMyPlayer::UpdateFocus()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		SetFocusedActor(nullptr);
		return;
	}
	FVector ViewLocation;
	FRotator ViewRotation; 
	PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);

	const FVector ViewForward = ViewRotation.Vector();
	const FVector TraceEnd = ViewLocation + (ViewForward * InteractionDistance);

	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(PlayerFocusTrace), false, this);
	GetWorld()->SweepMultiByChannel(
		HitResults,
		ViewLocation,
		TraceEnd,
		FQuat::Identity,
		InteractionTraceChannel,
		FCollisionShape::MakeSphere(FocusTraceRadius),
		QueryParams
	);

	AActor* BestActor = nullptr;
	float BestScore = -FLT_MAX;

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor || !HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			continue;
		}

		if (!IInteractable::Execute_CanInteract(HitActor, this))
		{
			continue;
		}

		const float Score = ScoreInteractableCandidate(HitActor, ViewLocation, ViewForward);
		if (Score > BestScore)
		{
			BestScore = Score;
			BestActor = HitActor;
		}
	}

	SetFocusedActor(BestActor);

}

float AMyPlayer::ScoreInteractableCandidate(const AActor* Actor, const FVector& ViewLocation, const FVector& ViewForward) const
{
	const FVector ToActor = (Actor->GetActorLocation() - ViewLocation);
	const float Distance = ToActor.Length();

	if (Distance > InteractionDistance)
	{
		return -FLT_MAX;
	}

	const FVector DirectionToActor = ToActor.GetSafeNormal();
	const float Alignment = FVector::DotProduct(ViewForward, DirectionToActor);

	return (Alignment * 1000.0f) - Distance;
}

void AMyPlayer::SetFocusedActor(AActor* NewFocusedActor)
{
	if (CurrentFocusedActor == NewFocusedActor)
	{
		return;
	}

	if (CurrentFocusedActor && CurrentFocusedActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		IInteractable::Execute_OnFocusEnd(CurrentFocusedActor, this);
	}

	CurrentFocusedActor = NewFocusedActor;

	if (CurrentFocusedActor && CurrentFocusedActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		IInteractable::Execute_OnFocusBegin(CurrentFocusedActor, this);
	}
	BP_OnFocusedActorChanged(CurrentFocusedActor);
}

void AMyPlayer::Interact()
{
	if (!CurrentFocusedActor)
	{
		return;
	}

	if (!IInteractable::Execute_CanInteract(CurrentFocusedActor, this))
	{
		return;
	}

	IInteractable::Execute_Interact(CurrentFocusedActor, this);
}

void AMyPlayer::LeftClick()
{
	if (!HoldingWeapon)
	{
		return;
	}
}

void AMyPlayer::BeginZoom()
{
	if (!CanUseSecondaryAction())
	{
		return;
	}

	HoldingWeapon->StartSecondaryAction();

	if (HoldingWeapon->SecondaryActionType == ESecondaryActionType::Zoom)
	{
		bIsZooming = true;
		bUseControllerRotationYaw = true;
		BP_SetZooming(true);
	}
}

void AMyPlayer::EndZoom()
{
	if (!HoldingWeapon)
	{
		return;
	}

	if (HoldingWeapon->bIsUsingSecondaryAction)
	{
		HoldingWeapon->StopSecondaryAction();
	}

	if (bIsZooming)
	{
		bIsZooming = false;
		bUseControllerRotationYaw = false;
		BP_SetZooming(false);
	}
}

bool AMyPlayer::CanZoom() const
{
	if (!CanUseSecondaryAction())
	{
		return false;
	}

	return HoldingWeapon->SecondaryActionType == ESecondaryActionType::Zoom;
}

bool AMyPlayer::CanUseSecondaryAction() const
{
	if (!HoldingWeapon)
	{
		return false;
	}

	if (bIsReloading)
	{
		return false;
	}

	return HoldingWeapon->CanStartSecondaryAction();
}

bool AMyPlayer::EquipWeaponV1(AWeaponBase* NewWeapon)
{
	if (!NewWeapon)
	{
		return false;
	}

	if (HoldingWeapon == NewWeapon)
	{
		return true;
	}

	UnequipWeaponV1();

	if (!AttachWeaponToCharacter(NewWeapon))
	{
		return false;
	}

	NewWeapon->SetOwner(this);
	NewWeapon->SetInstigator(this);

	if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(NewWeapon->GetRootComponent()))
	{
		PrimitiveComponent->SetSimulatePhysics(false);
		PrimitiveComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	HoldingWeapon = NewWeapon;
	return true;
}

void AMyPlayer::UnequipWeaponV1()
{
	if (!HoldingWeapon)
	{
		return;
	}

	EndZoom();

	HoldingWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	HoldingWeapon->SetOwner(nullptr);
	HoldingWeapon->SetInstigator(nullptr);
	HoldingWeapon->Destroy();

	if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(HoldingWeapon->GetRootComponent()))
	{
		PrimitiveComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	HoldingWeapon = nullptr;
}

bool AMyPlayer::AttachWeaponToCharacter(AWeaponBase* WeaponToAttach) const
{
	if (!WeaponToAttach || !GetMesh())
	{
		return false;
	}

	WeaponToAttach->AttachToComponent(
		GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		WeaponToAttach->SocketName
	);

	return true;
}
