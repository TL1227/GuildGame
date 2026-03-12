// Copyright Epic Games, Inc. All Rights Reserved.

#include "GuildCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/ArrowComponent.h"
#include "MovementCircle.h"
#include "Combatant.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AGuildCharacter

AGuildCharacter::AGuildCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(ExploreCapsuleRadius, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CombatantComponent = CreateDefaultSubobject<UCombatant>(TEXT("CombatantComponent"));
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AGuildCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AGuildCharacter::Tick(float Delta)
{
	// Call the base class  
	Super::Tick(Delta);

	if (MovementCircle)
	{
		//TODO: Get Radius ahead of time
		const float CapRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();
		SetActorLocation(MovementCircle->ClampToBounds(GetActorLocation(), CapRadius));
		
	}
	
	if (InCombat)
	{
		DrawDebugCircle(
			GetWorld(),
			GetActorLocation() - FVector(0.0f, 0.0f, 95.0f), //TODO: get rid of this magic number
			GetCapsuleComponent()->GetUnscaledCapsuleRadius(),
			64,
			CircleColour,
			false,
			-1,
			0,
			3,
			FVector(1, 0, 0),
			FVector(0, 1, 0),
			false
		);
	}
}

void AGuildCharacter::SetExploreMode()
{
	GetCapsuleComponent()->SetCapsuleRadius(ExploreCapsuleRadius);
}

//TODO: this should probably subscribe to some OnCombatStart function from our BattleSystem
void AGuildCharacter::SetCombatMode()
{
	GetCapsuleComponent()->SetCapsuleRadius(CombatCapsuleRadius);

	//TODO: Draw on player turn
	MovementCircle = GetWorld()->SpawnActor<AMovementCircle>(
		AMovementCircle::StaticClass(),
		GetActorLocation(),
		FRotator::ZeroRotator
	);

	MovementCircle->Radius = MoveCircleRadius;


	InCombat = true;
}

void AGuildCharacter::SetInheritPitch(bool b)
{
	CameraBoom->bInheritPitch = b;
}

UCombatant* AGuildCharacter::GetCombatantComponent()
{
	return CombatantComponent;
}
