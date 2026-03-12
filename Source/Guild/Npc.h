// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Npc.generated.h"

UCLASS()
class GUILD_API ANpc : public AActor
{
	GENERATED_BODY()
	
	// Pivot root
	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* Pivot;

	UPROPERTY(EditAnywhere, Category = "NPC", meta = (AllowPrivateAccess = "true"))
	FString Name;

	UPROPERTY(EditAnywhere, Category = "NPC", meta = (AllowPrivateAccess = "true"))
	FName DialogueStartId;

	UPROPERTY(EditAnywhere, Category = "NPC", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* InteractZone = nullptr;


	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:	
	ANpc();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void HighlightNpc(bool bHighlight);

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Dialogue", meta = (AllowPrivateAccess = "true"))
	FString DialoguePlaceHolder;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FName GetDialogueStartId();

};
