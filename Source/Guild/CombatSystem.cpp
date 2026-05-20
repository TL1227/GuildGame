// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatSystem.h"
#include "EnemyCharacter.h"
#include "Combatant.h"
#include "Team.h"

DECLARE_LOG_CATEGORY_EXTERN(Combat, Display, All);
DEFINE_LOG_CATEGORY(Combat);
//FLogCategory Combat;

void UCombatSystem::ApplyDamage(int damage, AActor* target)
{
	if (target)
	{
		if (UCombatant* combatant = target->GetComponentByClass<UCombatant>())
		{
			combatant->ChangeHealthPoints(damage);
			OnHealthChanged.Broadcast(damage, target);

			if (combatant->GetHealthPoints() <= 0)
			{
				if (combatant->GetTeamId() != 0) //NOTE: This is a little hack that should stop the player health hitting 0 mattering until we decide how to handle it.
				{
					RemoveParticipant(target);
					OnHealthToZero.Broadcast(target);
				}
			}
		}
		else
		{
			UE_LOG(Combat, Display, TEXT("Doesn't have a CombatantComponant: %s"), *combatant->GetName());;
		}
	}
}

void UCombatSystem::EndTurn() //TODO: maybe NextTurn() ?
{
	SetParticipant(GetNextParticipant());
	OnTurnChanged.Broadcast(CurrentParticipant);
}

AActor* UCombatSystem::GetNextParticipant()
{
	AActor* nextParticipant = Teams[TeamIndex]->GetNextParticipant();

	if (IsValid(nextParticipant))
	{
		return nextParticipant;
	}
	else
	{
		//reset the index to the first team member
		Teams[TeamIndex]->SetCurrentParticipantIndex(0);

		//switch teams
		TeamIndex++;
		UE_LOG(Combat, Display, TEXT("inreasing TeamIdex to %i"), TeamIndex);
		if (TeamIndex >= TeamCount)
		{
			TeamIndex = 0;
		}

		UE_LOG(Combat, Display, TEXT("TeamIndex is now %i"), TeamIndex);
		return Teams[TeamIndex]->GetCurrentParticipant();
	}
}

void UCombatSystem::SetParticipant(AActor* participant)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 5.0f, 
			FColor::Yellow, 
			FString::Printf(TEXT("TURN Team %i Member %i"), TeamIndex, Teams[TeamIndex]->GetCurrentParticipantIndex())
		);
	}

	CurrentParticipant = participant;
	UE_LOG(Combat, Display, TEXT("TURN Team %i Member %i"), TeamIndex, Teams[TeamIndex]->GetCurrentParticipantIndex());
}

void UCombatSystem::StartBattle(TArray<AActor*> participants)
{
	//TODO: write some code to work out number of unique teams in the combatants (prolly use a map)
	Teams.SetNum(2);

	for (auto* participant : participants)
	{
		if (UCombatant* combatant = participant->GetComponentByClass<UCombatant>())
		{
			int id = combatant->GetTeamId();
			if (!IsValid(Teams[id]))
			{
				Teams[id] = NewObject<UTeam>(this);
			}
			Teams[id]->AddMember(participant);
		}
	}

	TeamCount = Teams.Num();
	Teams[TeamIndex]->SetCurrentParticipantIndex(0);
	SetParticipant(Teams[TeamIndex]->GetCurrentParticipant());
}

void UCombatSystem::RemoveParticipant(AActor* participant)
{
	if (UCombatant* combatant = participant->GetComponentByClass<UCombatant>())
	{
		Teams[combatant->GetTeamId()]->RemoveParticipant(participant);
	}
}

void UCombatSystem::FillAllCurrentParticipants(TArray<AActor*>& participants)
{
	participants.Empty();

	for (auto* team : Teams)
	{
		if (IsValid(team))
		{
			for (auto* member : team->GetAllMembers())
			{
				if (IsValid(member))
				{
					participants.Add(member);
				}
				else
				{
					UE_LOG(Combat, Display, TEXT("FillAllCurrentPart() member not valid"));
				}
			}
		}
		else
		{
			UE_LOG(Combat, Display, TEXT("FillAllCurrentPart() team not valid"));
		}
	}
}
