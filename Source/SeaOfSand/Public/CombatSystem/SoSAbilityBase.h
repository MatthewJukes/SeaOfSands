// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoSAbilityBase.generated.h"


class ASoSWeaponBase;
class USoSCombatComponent;


UENUM()
enum class EAbilityResourceType : uint8
{
	Energy,
	Health
};


UENUM()
enum class EAbilityCastType : uint8
{
	Default,
	Instant,
	Cast //TODO define types as needed
};


UCLASS(BlueprintType, Blueprintable)
class SEAOFSAND_API USoSAbilityBase : public UObject
{
	GENERATED_BODY()

protected:

	USoSAbilityBase();

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	void InitializeAbility();

	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
	bool StartAbility(AActor* Source, ASoSWeaponBase* Weapon, float ClassSpecificFloatValue);
	virtual bool StartAbility_Implementation(AActor* Source, ASoSWeaponBase* Weapon, float ClassSpecificFloatValue);

	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
	bool ReleashAbility(AActor* Source, ASoSWeaponBase* Weapon, float ClassSpecificFloatValue);
	virtual bool ReleashAbility_Implementation(AActor* Source, ASoSWeaponBase* Weapon, float ClassSpecificFloatValue);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ability")
	void ReadyCombo();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ability")
	void EndCombo();

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	void ActionComplete();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Ability")
	void HitboxActivate();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Ability")
	void HitboxDeactivate();

protected:

	float LastTimeActivated;

	float LastChargeRemainder;

	int32 CurrentCharges;

	bool bComboReady;

	USoSCombatComponent* OwningCombatComp;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	EAbilityCastType CastType;

	UPROPERTY(EditDefaultsOnly, Category = "Ability", meta = (ClampMin = "0.001", UIMin = "0.001"))
	float Cooldown;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float Cost;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	bool bHasCharges;

	UPROPERTY(EditDefaultsOnly, Category = "Ability", meta = (ClampMin = "0.001", UIMin = "0.001", EditCondition = "bHasCharges"))
	float ChargeTime;

	UPROPERTY(EditDefaultsOnly, Category = "Ability", meta = (ClampMin = "1", UIMin = "1", EditCondition = "bHasCharges"))
	int32 MaxCharges;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	EAbilityResourceType ResourceType;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	UTexture2D* AbilityIcon;

public:

	UFUNCTION(BlueprintCallable, Category = "Ability")
	EAbilityCastType GetCastType();

	UFUNCTION(BlueprintCallable, Category = "Ability")
	float GetLastTimeActivated() const;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	float GetLastChargeRemainder() const;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	float GetCooldown() const;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	float GetCost() const;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool GetHasCharges() const;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	float GetChargeTime() const;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool GetComboReady() const;  

	UFUNCTION(BlueprintCallable, Category = "Ability")
	int32 GetMaxCharges() const;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	int32 GetCurrentCharges() const;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	UTexture2D* GetAbilityIcon() const;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	EAbilityResourceType GetResourceType() const;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	USoSCombatComponent* GetOwningCombatComp() const;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void SetComboReady(bool bNewComboReady);

	void SetLastTimeActivated(float NewTime);

	void SetLastChargeRemainder(float NewRemainder);

	void SetCurrentCharges(int32 Charges);

	void SetOwningCombatComp(USoSCombatComponent* CombatComp);
};