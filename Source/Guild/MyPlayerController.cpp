// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"

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
#include "Npc.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyCharacter.h"
#include "MovementCircle.h"
#include "CombatSystem.h"
#include "Combatant.h"
#include "Team.h"

AMyPlayerController::AMyPlayerController()
{
	CombatCameraTransition = CreateDefaultSubobject<UTimelineComponent>(TEXT("CombatCameraTransition"));
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//set up dialogue system
	DialogueSystem = GetGameInstance()->GetSubsystem<UDialogueSystem>();
	DialogueSystem->DialogueStarted.AddDynamic(this, &AMyPlayerController::OnDialogueStarted);
	DialogueSystem->DialogueAdvanced.AddDynamic(this, &AMyPlayerController::OnDialogueAdvanced);
	DialogueSystem->DialogueEnded.AddDynamic(this, &AMyPlayerController::OnDialogueEnded);

	//set up dialogue widget
	if (DialogueWidgetClass)
	{
		DialogueWidget = CreateWidget<UDialogueWidget>(this, DialogueWidgetClass);

		if (DialogueWidget)
		{
			DialogueWidget->AddToViewport();
			DialogueWidget->Hide();
		}
	}

	//set up combat menu widget
	if (CombatMenuClass)
	{
		CombatMenuWidget = CreateWidget<UMenuWidget>(this, CombatMenuClass);

		if (CombatMenuWidget)
		{
			CombatMenuWidget->AddToViewport();
			CombatMenuWidget->Hide();
		}
	}

	//camera
	UpdateBoomArmLengthTrack.BindDynamic(this, &AMyPlayerController::UpdateCameraArmLength);
	UpdateBoomPitchTrack.BindDynamic(this, &AMyPlayerController::UpdateCameraBoomPitch);
	CombatTransitionEnded.BindDynamic(this, &AMyPlayerController::StartCombat);

	CombatCameraTransition->AddInterpFloat(CameraBoomArmLengthCurve, UpdateBoomArmLengthTrack);
	CombatCameraTransition->AddInterpFloat(CameraBoomPitchCurve, UpdateBoomPitchTrack);
	CombatCameraTransition->SetTimelineFinishedFunc(CombatTransitionEnded);

	CombatSystem = GetGameInstance()->GetSubsystem<UCombatSystem>();
	CombatSystem->OnTurnChanged.AddUObject(this, &AMyPlayerController::CheckTurnChanged);
}

void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyPlayerController::SwitchInputMappingContext(UInputMappingContext* context)
{
	//NOTE: we may want to leave some lower level mapping contexts for global stuff like a pause menu
	//so this could all be changed. It works fine for now though.
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(context, 0);
	}
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) 
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Interact);

		EnhancedInputComponent->BindAction(AdvanceDialogueAction, ETriggerEvent::Triggered, this, &AMyPlayerController::AdvanceDialogue);
		EnhancedInputComponent->BindAction(ComboAttackAction, ETriggerEvent::Triggered, this, &AMyPlayerController::ComboAttackExecute);

		EnhancedInputComponent->BindAction(ConfirmMenuAction, ETriggerEvent::Triggered, this, &AMyPlayerController::ConfirmMenu);
		EnhancedInputComponent->BindAction(CancelMenuAction, ETriggerEvent::Triggered, this, &AMyPlayerController::CancelMenu);
		EnhancedInputComponent->BindAction(MenuNavigateAction, ETriggerEvent::Triggered, this, &AMyPlayerController::NavigateMenu);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMyPlayerController::ConfirmMenu()
{
	if (MenuStack.IsEmpty()) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Menu stack is empty"));
		return;
	}
	else
	{
		MenuStack.Top()->Confirm();
	}
}

void AMyPlayerController::CancelMenu()
{
	if (MenuStack.IsEmpty()) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Menu stack is empty"));
		return;
	}
	else
	{
		MenuStack.Top()->Cancel();
	}
}

void AMyPlayerController::NavigateMenu(const FInputActionValue& Value)
{
	if (MenuStack.IsEmpty()) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Menu stack is empty"));
		return;
	}
	else
	{
		MenuStack.Top()->Navigate(Value.Get<FVector2D>());
	}
}

void AMyPlayerController::Interact()
{
	if (InteractableActor == nullptr)
		return;

	if (ANpc* npc = Cast<ANpc>(InteractableActor))
	{
		SwitchInputMappingContext(DialogueMappingContext); //TODO: should this go in OnDialogueStarted?
		DialogueSystem->StartDialogue(npc);
	}
}

void AMyPlayerController::OnDialogueStarted(const FDialogueRow& row)
{
	DialogueWidget->Show();
	DialogueWidget->SetDialogueText(row.Name, row.Text);
}

void AMyPlayerController::AdvanceDialogue()
{
	DialogueSystem->AdvanceDialogue();
}

void AMyPlayerController::OnDialogueAdvanced(const FDialogueRow& row)
{
	DialogueWidget->SetDialogueText(row.Name, row.Text);
}

void AMyPlayerController::OnDialogueEnded()
{
	SwitchInputMappingContext(DefaultMappingContext);
	DialogueWidget->Hide();
}

void AMyPlayerController::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (ACharacter* character = GetCharacter())
	{
		character->AddMovementInput(ForwardDirection, MovementVector.Y);
		character->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMyPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void AMyPlayerController::ComboAttackExecute()
{
	//NOTE: maybe we want to change the number of targets a combo attack can hit later
	if (TargetedActors.Num() == 1)
	{
		if (AGuildCharacter* gc = Cast<AGuildCharacter>(GetPawn()))
		{
			CombatSystem->ApplyDamage(
				gc->GetCombatantComponent()->GetBaseAttackDamage() * (ComboAttackCount + 1), 
				TargetedActors[0]
			);

			if (ComboAttackCount++ >= 2)
				EndTurn();
		}
	}
}

void AMyPlayerController::EndTurn()
{
	ComboAttackCount = 0;
	StopInput();
	CombatSystem->EndTurn();
}

void AMyPlayerController::CombatEventTriggered()
{
	if (AGuildCharacter* ch = Cast<AGuildCharacter>(GetPawn()))
	{
		//TODO: This might need to be made a little more elegant 
		UE_LOG(LogTemp, Display, TEXT("CombatEventtriggered-StopInput()"));
		StopInput();

		CurrentCameraArmLength = ch->GetCameraBoom()->TargetArmLength;
		CombatCameraArmLengthToMove = CombatCameraArmLength - CurrentCameraArmLength;

		//this needs to happen otherwise the camera can't change the pitch for the transition to combat (I think)
		ch->SetInheritPitch(false); 
		CurrentCombatCameraPitch = ch->GetFollowCamera()->GetComponentRotation().Pitch;
		CombatCameraPitchToMove = CombatCameraPitch - CurrentCombatCameraPitch;

		CombatCameraTransition->Play();
	}
}

void AMyPlayerController::StopInput()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
	}
}

void AMyPlayerController::UpdateCameraBoomPitch(float pitch)
{
	if (AGuildCharacter* ch = Cast<AGuildCharacter>(GetPawn()))
	{
		ch->GetCameraBoom()->SetRelativeRotation(FRotator (CurrentCombatCameraPitch + (pitch * CombatCameraPitchToMove), 0.0f, 0.0f));
	}
}

void AMyPlayerController::UpdateCameraArmLength(float length)
{
	if (AGuildCharacter* ch = Cast<AGuildCharacter>(GetPawn()))
	{
		ch->GetCameraBoom()->TargetArmLength = CurrentCameraArmLength + (length * CombatCameraArmLengthToMove);
	}
}

void AMyPlayerController::SetCombatInput()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		Subsystem->AddMappingContext(CombatMappingContext, 1);

		UE_LOG(LogTemp, Display, TEXT("Set CombatInput"));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Set CombatInput Failed"));
	}
}

void AMyPlayerController::StartCombat()
{
	if (AGuildCharacter* ch = Cast<AGuildCharacter>(GetPawn()))
	{
		ch->SetCombatMode();

		SetCombatInput();

		BPSetEnemiesToInCombat(); //Change this in the future

		//Get All actors that are combatants
		TArray<AActor*> combatants;
		UGameplayStatics::GetAllActorsOfClass(
			GetWorld(),
			AActor::StaticClass(),
			combatants
		);

		TArray<AActor*> participants;
		for (auto* combatant : combatants)
			if (combatant->GetComponentByClass<UCombatant>())
			{
				participants.Emplace(combatant);
			}

		if (CombatMenuWidget)
		{
			MenuStack.Push(CombatMenuWidget);
			CombatMenuWidget->Show();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("CombatMenuWidget is NULL"));;
		}

		CombatSystem->StartBattle(participants);
	}
}

void AMyPlayerController::CheckTurnChanged(AActor* player)
{
	UE_LOG(LogTemp, Display, TEXT("%s listening to turn change"), *GetPawn()->GetName());
	if (IsValid(player) && player == GetPawn())
	{
		UE_LOG(LogTemp, Display, TEXT("it's %s turn"), *player->GetName());

		SetCombatInput();
	}
}
