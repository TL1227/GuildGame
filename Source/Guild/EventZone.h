// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SphereComponent.h>
#include "EventZone.generated.h"

UENUM(BlueprintType)
enum class EEventType : uint8
{
	Dialogue,
	Combat,
	Quest,
	BossEncounter
};

UCLASS()
class GUILD_API AEventZone : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "EventZone", meta = (AllowPrivateAccess = "true"))
	USphereComponent* Zone = nullptr;

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Sets default values for this actor's properties
	AEventZone();

	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = "EventZone")
	EEventType EventType;

	//If this is checked then the event zone will delete itself after firing.
	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = "EventZone")
	bool FireOnce;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
