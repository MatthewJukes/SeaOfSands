// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoSASAttributes.generated.h"

UENUM(BlueprintType)
enum class EASAttributeName : uint8
{
	HealthMax,
	HealthCurrent,
	ArmourMax,
	ArmourCurrent,
	EnergyMax,
	EnergyCurrent,
	Speed
};

USTRUCT(BlueprintType)
struct FASAttributeData
{
	GENERATED_USTRUCT_BODY()

	float HealthMaxValue;

	float HealthCurrentValue;

	float ArmourMaxValue;

	float ArmourCurrentValue;

	float EnergyMaxValue;

	float EnergyCurrentValue;

	float SpeedValue;
};
