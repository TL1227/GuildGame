// Fill out your copyright notice in the Description page of Project Settings.
#include "Aoe.h"

// Sets default values
AAoe::AAoe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAoe::BeginPlay()
{
	Super::BeginPlay();

	//TODO: We could use this if we want to destroy AOEs on turn change.
	//		I'm not yet sure if that's a good idea though

	//CombatSystem = GetGameInstance()->GetSubsystem<UCombatSystem>();
	//CombatSystem->OnTurnChanged.AddUObject(this, &AAoe::CheckTurnChanged);
}

void AAoe::CheckTurnChanged()
{
	Destroy();
}

// Called every frame
void AAoe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

