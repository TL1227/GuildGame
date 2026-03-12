// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class GUILD_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, Category = "Capsule", meta = (AllowPrivateAccess = "true"))
	float ExploreCapsuleRadius = 42.f;

	UPROPERTY(EditAnywhere, Category = "Capsule", meta = (AllowPrivateAccess = "true"))
	float CombatCapsuleRadius = 80.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	bool InCombat = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FColor CircleColour = FColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UCombatant* CombatantComponent;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UCombatant* GetCombatantComponent();

	UFUNCTION(BlueprintCallable)
	void SetInCombat(bool value);

	UPROPERTY(EditAnywhere, Category = "Move")
	FVector PersonalOffset;
};
