// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Aoe.generated.h"

UCLASS()
class GUILD_API AAoe : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAoe();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	TArray<AActor*> GetHitActors();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
