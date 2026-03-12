// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GuildCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AGuildCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, Category = "Capsule", meta = (AllowPrivateAccess = "true"))
	float CapsuleRadius;

	UPROPERTY(EditAnywhere, Category = "Capsule", meta = (AllowPrivateAccess = "true"))
	float ExploreCapsuleRadius = 42.f;

	UPROPERTY(EditAnywhere, Category = "Capsule", meta = (AllowPrivateAccess = "true"))
	float CombatCapsuleRadius = 80.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	bool InCombat = false;

	
public:
	AGuildCharacter();

	
protected:
	virtual void BeginPlay();
	virtual void Tick(float) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FColor CircleColour = FColor::Green;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UCombatant* CombatantComponent;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MoveCircle")
	float MoveCircleRadius;

	void SetInheritPitch(bool);

	void SetCombatMode();
	void SetExploreMode();

	class AMovementCircle* MovementCircle;

	UCombatant* GetCombatantComponent();
};

