// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovementCircle.generated.h"

UCLASS()
class GUILD_API AMovementCircle : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY()
	USceneComponent* Root;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	AMovementCircle();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float Radius;

	FVector ClampToBounds(const FVector& Pos, const float padding) const;
	bool IsInside(const FVector& Pos) const;
};
