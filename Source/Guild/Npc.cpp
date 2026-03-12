// Fill out your copyright notice in the Description page of Project Settings.

#include "Npc.h"
#include "GuildCharacter.h"
#include "Engine/LocalPlayer.h"
#include "MyPlayerController.h"

ANpc::ANpc()
{
	PrimaryActorTick.bCanEverTick = false;

	Pivot = CreateDefaultSubobject<USceneComponent>(TEXT("Pivot"));
	RootComponent = Pivot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Pivot);

	InteractZone = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));
	InteractZone->SetupAttachment(Pivot);
	InteractZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractZone->SetCollisionObjectType(ECC_WorldDynamic);
	InteractZone->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InteractZone->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	InteractZone->OnComponentBeginOverlap.AddDynamic(this, &ANpc::OverlapBegin);
	InteractZone->OnComponentEndOverlap.AddDynamic(this, &ANpc::OverlapEnd);
}

void ANpc::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	//UE_LOG(LogTemp, Display, TEXT("InZone"));

	//TODO: this should check the the OtherActor is the GuildCharacter? Otherwise anything could trigger this?
	if (!OtherActor) 
		return;

	HighlightNpc(true);

	if (AMyPlayerController* pc = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		pc->InteractableActor = this;
	}
}

void ANpc::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Display, TEXT("OutZone"));

	HighlightNpc(false);

	if (AMyPlayerController* pc = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		pc->InteractableActor = nullptr;
	}
}

FName ANpc::GetDialogueStartId()
{
	return DialogueStartId;
}

// Called when the game starts or when spawned
void ANpc::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANpc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

