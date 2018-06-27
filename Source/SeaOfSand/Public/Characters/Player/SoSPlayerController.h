// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SoSPlayerController.generated.h"

class IPlayerInputsInterface;
class ASoSPlayerCharacter;
class USoSPlayerInventory;
class ASoSPlayerHUD;

UCLASS()
class SEAOFSAND_API ASoSPlayerController : public APlayerController
{
	GENERATED_BODY()
		
public:

	virtual void SetupInputComponent() override;

protected:

	virtual void BeginPlay() override;

public:

	void UpdateCurrentPawn();

	void ToggleVehicleHud();

	FVector GetCrosshairHitLocation() const;

	// Get Pitch/Yaw from current pawn camera
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	FRotator GetAimOffsets() const;

protected:

	APawn* CurrentPlayerPawn;

	ASoSPlayerCharacter* PlayerCharacter;

	USoSPlayerInventory* PlayerInventory;

	ASoSPlayerHUD* PlayerHUD;

	UPROPERTY(EditDefaultsOnly)
	float CrossHairXLocation = 0.5;

	UPROPERTY(EditDefaultsOnly)
	float CrosshairYLocation = 0.5;

	UPROPERTY(EditDefaultsOnly)
	float CrosshairTraceRange = 100000.f;

	void StartFiring();

	void StopFiring();
	
	void HolsterUnholster();
	
	void Reload();

	void NextWeapon();

	void PrevWeapon();

	// Get crosshair world hit location
	bool GetLookDirection(FVector2D ScreenLocation, FVector &LookDirection) const;

	FVector GetLookVectorHitLocation(FVector LookDirection) const;

public:

	ASoSPlayerHUD* GetPlayerHUD() const;
}; 
