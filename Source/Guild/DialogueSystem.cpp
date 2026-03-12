// Fill out your copyright notice in the Description page of Project Settings.
#include "DialogueSystem.h"

UDialogueSystem::UDialogueSystem()
{
	//TODO: look at removing this hardcoded path
	static ConstructorHelpers::FObjectFinder<UDataTable> DialogueTableObj(TEXT("/Game/ThirdPerson/DataTables/DialogueTable"));
	if (DialogueTableObj.Succeeded())
	{
		DialogueTable = DialogueTableObj.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load DialogueTable!!"));
	}
}

void UDialogueSystem::StartDialogue(ANpc* npc)
{
	if (DialogueTable)
	{
		if (FDialogueRow* TempRow = DialogueTable->FindRow<FDialogueRow>(npc->GetDialogueStartId(), TEXT("GetDialogueRow")))
		{
			CurrentRow = *TempRow;
			DialogueStarted.Broadcast(CurrentRow);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to get row!"));
		}
	}
}

void UDialogueSystem::AdvanceDialogue()
{
	if (CurrentRow.NextRowId == NAME_None)
	{
		EndDialogue();
	}
	else
	{
		if (FDialogueRow* TempRow = DialogueTable->FindRow<FDialogueRow>(CurrentRow.NextRowId, TEXT("GetDialogueRow")))
		{
			CurrentRow = *TempRow;
		}

		DialogueAdvanced.Broadcast(CurrentRow);
	}
}

void UDialogueSystem::EndDialogue()
{
	DialogueEnded.Broadcast();
}
