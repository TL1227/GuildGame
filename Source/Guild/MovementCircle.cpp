// Fill out your copyright notice in the Description page of Project Settings.

#include "MovementCircle.h"
#include "MyPlayerController.h"

// Sets default values
AMovementCircle::AMovementCircle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>("Root");
    RootComponent = Root;
}

// Called when the game starts or when spawned
void AMovementCircle::BeginPlay()
{
	Super::BeginPlay();
}

FVector AMovementCircle::ClampToBounds(const FVector& Pos, const float padding) const
{
    const float paddedRadius = Radius - padding;

    const FVector Center = GetActorLocation();
    FVector ToPos = Pos - Center;

    //square first for optimisation 
    if (ToPos.SizeSquared() <= paddedRadius * paddedRadius)
    {
        return Pos;
    }
    else
    {
		return Center + ToPos.GetSafeNormal() * paddedRadius;
    }
}

bool AMovementCircle::IsInside(const FVector& Pos) const
{
    return FVector::DistSquared(Pos, GetActorLocation()) <= Radius * Radius;
}

// Called every frame
void AMovementCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    DrawDebugCircle(
        GetWorld(),
        GetActorLocation() - FVector(0.0f, 0.0f, 95.0f), //TODO: get rid of this magic number
        Radius,
        64,
        FColor::Green,
        false,
        -1,
        0,
        3,
        FVector(1, 0, 0),
        FVector(0, 1, 0),
        false
    );
}

