// Fill out your copyright notice in the Description page of Project Settings.

#include "EventZone.h"
#include "MyPlayerController.h"
#include "GuildCharacter.h"

// Sets default values
AEventZone::AEventZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Zone = CreateDefaultSubobject<USphereComponent>(FName("Collision Mesh"));
	Zone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Zone->SetCollisionObjectType(ECC_WorldDynamic);
	Zone->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Zone->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Zone->OnComponentBeginOverlap.AddDynamic(this, &AEventZone::OverlapBegin);
}

// Called when the game starts or when spawned
void AEventZone::BeginPlay()
{
	Super::BeginPlay();
}

void AEventZone::OverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Display, TEXT("Overlap"));
	if (AGuildCharacter* ch = Cast<AGuildCharacter>(OtherActor))
	{
		switch (EventType)
		{
		case EEventType::Combat:
			if (AMyPlayerController* pc = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController()))
			{
				//TODO: this should all probably happen on OnCombatBegin in the player controller or something
				pc->CombatEventTriggered();
			}
			break;
		default:
			break;
		}

		if (FireOnce) Destroy();
	}
}

