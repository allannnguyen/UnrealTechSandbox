// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/Character.h"
#include "Weapons/WeaponBase.h"
#include "MyPlayer.generated.h"

UCLASS(Blueprintable)
class TECHSANDBOX_API AMyPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AMyPlayer();

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> PlayerMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	int32 PlayerMappingPriority = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TObjectPtr<UInputAction> LeftClickAction;
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TObjectPtr<UInputAction> RightClickAction;
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TObjectPtr<AWeaponBase> HoldingWeapon = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	bool bIsZooming = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	bool bIsReloading = false;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TObjectPtr<UInputAction> InteractAction;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<AActor> CurrentFocusedActor = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float FocusTraceRadius = 40.0f;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float InteractionDistance = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	TEnumAsByte<ECollisionChannel> InteractionTraceChannel = ECC_Visibility;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact();
	float ScoreInteractableCandidate(const AActor* Actor, const FVector& ViewLocation, const FVector& ViewForward) const;
	void UpdateFocus();
	void SetFocusedActor(AActor* NewFocusedActor);
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void BP_OnFocusedActorChanged(AActor* NewFocusedActor);
	
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void LeftClick();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void BeginZoom();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void EndZoom();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	bool CanZoom() const;

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	bool CanUseSecondaryAction() const;

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	bool EquipWeaponV1(AWeaponBase* NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void UnequipWeaponV1();

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapons")
	void BP_SetZooming(bool bZooming);
protected:
	AActor* FindInteractableActor(FHitResult& OutHit) const;
	bool AttachWeaponToCharacter(AWeaponBase* WeaponToAttach) const;
};
