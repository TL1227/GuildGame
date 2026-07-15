// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Combatant.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(ExploreCapsuleRadius, 96.0f);
	CombatantComponent = CreateDefaultSubobject<UCombatant>(TEXT("CombatantComponent"));
	CombatantComponent->SetTeamId(1);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacter::SetInCombat(bool value)
{
	InCombat = value;
	GetCapsuleComponent()->SetCapsuleRadius(CombatCapsuleRadius);
}

UCombatant* AEnemyCharacter::GetCombatantComponent()
{
	return CombatantComponent;
}
