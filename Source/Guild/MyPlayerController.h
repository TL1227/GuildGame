// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "DialogueWidget.h"
#include "DialogueSystem.h"
#include "EventZone.h"
#include "Components/TimelineComponent.h"
#include "MyPlayerController.generated.h"

class UCombatSystem;

UCLASS()
class GUILD_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

	//Default Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	//Combat Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* CombatMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UInputAction* ComboAttackAction;

	//Dialogue
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DialogueMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue", meta = (AllowPrivateAccess = "true"))
	UInputAction* AdvanceDialogueAction;

	UPROPERTY(EditDefaultsOnly, Category = "Dialogue")
	TSubclassOf<UDialogueWidget> DialogueWidgetClass;

	UPROPERTY()
	UDialogueSystem* DialogueSystem;

	UPROPERTY()
	UDialogueWidget* DialogueWidget = nullptr;

	UFUNCTION()
	void OnDialogueStarted(const FDialogueRow& page);

	UFUNCTION()
	void OnDialogueAdvanced(const FDialogueRow& page);

	UFUNCTION()
	void OnDialogueEnded();

	UFUNCTION(BlueprintCallable)
	void StopInput();

	//camera
	UPROPERTY(EditAnywhere, Category="Camera")
	UCurveFloat* CameraBoomArmLengthCurve;

	UPROPERTY(EditAnywhere, Category="Camera")
	UCurveFloat* CameraBoomPitchCurve;

	UPROPERTY(VisibleAnywhere, Category="Camera")
	UTimelineComponent* CombatCameraTransition;

	FOnTimelineFloat UpdateBoomArmLengthTrack;
	FOnTimelineFloat UpdateBoomPitchTrack;
	FOnTimelineEvent CombatTransitionEnded;

	int ComboAttackCount;

	UCombatSystem* CombatSystem;
public:
	AMyPlayerController();

	void CombatEventTriggered();

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	AActor* InteractableActor;


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float) override;
	virtual void SetupInputComponent() override;

	//input
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact();
	void AdvanceDialogue();
	void SwitchInputMappingContext(UInputMappingContext* context);
	void ComboAttackExecute();

	UFUNCTION(BlueprintCallable)
	void EndTurn();

	void SetCombatInput();

	void CheckTurnChanged(AActor* player);

	UFUNCTION(BlueprintImplementableEvent, Category = "Enemies")
	void BPSetEnemiesToInCombat();

	//combat
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Combat")
	AActor* TargetedActor;

	//camera
	float CombatCameraPitchToMove;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Camera")
	float CombatCameraPitch = -35;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Camera")
	float CurrentCombatCameraPitch;

	float CombatCameraArmLengthToMove;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Camera")
	float CombatCameraArmLength = 600;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Camera")
	float CurrentCameraArmLength;

	UFUNCTION()
	void UpdateCameraBoomPitch(float pitch);
	UFUNCTION()
	void UpdateCameraArmLength(float pitch);

	UFUNCTION()
	void StartCombat();
};
