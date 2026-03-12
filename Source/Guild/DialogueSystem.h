// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Npc.h"
#include "DialogueSystem.generated.h"

USTRUCT(BlueprintType)
struct FDialogueRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Text;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NextRowId;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueStarted, const FDialogueRow&, Page);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueAdvance, const FDialogueRow&, Page);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnded);

UCLASS(BlueprintType)
class GUILD_API UDialogueSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	FDialogueRow CurrentRow;

public:
	UDialogueSystem();
	void StartDialogue(ANpc* npc);
	void AdvanceDialogue();
	void EndDialogue();

	UPROPERTY()
	FOnDialogueStarted DialogueStarted;

	UPROPERTY()
	FOnDialogueAdvance DialogueAdvanced;

	UPROPERTY()
	FOnDialogueEnded DialogueEnded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	UDataTable* DialogueTable;
};
