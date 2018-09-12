// Fill out your copyright notice in the Description page of Project Settings.

#include "SoSWeaponBase.h"
#include "SoSPlayerController.h"
#include "SoSPlayerCharacter.h"
#include "SoSASAbilityBase.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values
ASoSWeaponBase::ASoSWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	ProjectileOriginSocketName = "ProjectileOriginSocket";

	WeaponState = EWeaponState::Holstered;
}


// Called when the game starts or when spawned
void ASoSWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ASoSPlayerController>(GetWorld()->GetFirstPlayerController());
	PlayerCharacter = Cast<ASoSPlayerCharacter>(GetOwner());
	
}


///////////////////////////////////////////////////
// Getters and Setters


EWeaponType ASoSWeaponBase::GetWeaponType() const
{
	return WeaponType;
}


EWeaponState ASoSWeaponBase::GetWeaponState() const
{
	return WeaponState;
}


FWeaponAbilitiesData& ASoSWeaponBase::GetWeaponAbilities()
{
	return WeaponAbilities;
}


USkeletalMeshComponent* ASoSWeaponBase::GetWeaponMesh() const
{
	return WeaponMesh;
}

FName ASoSWeaponBase::GetProjectileOriginSocketName()
{
	return ProjectileOriginSocketName;
}

void ASoSWeaponBase::SetWeaponState(EWeaponState NewState)
{
	WeaponState = NewState;
}

