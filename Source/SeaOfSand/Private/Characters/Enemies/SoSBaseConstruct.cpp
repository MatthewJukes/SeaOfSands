// Fill out your copyright notice in the Description page of Project Settings.

#include "SoSBaseConstruct.h"
#include "GameFramework/CharacterMovementComponent.h"

ASoSBaseConstruct::ASoSBaseConstruct()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);

}


void ASoSBaseConstruct::BeginPlay()
{
	Super::BeginPlay();


}

