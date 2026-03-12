// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatSystem.h"
#include "EnemyCharacter.h"
#include "Combatant.h"

void UCombatSystem::ApplyDamage(int damage, AActor* target)
{
	if (target)
	{
		if (UCombatant* enemy = target->GetComponentByClass<UCombatant>())
		{
			enemy->ChangeHealthPoints(damage);
			OnHealthChanged.Broadcast(damage, target);

			if (enemy->GetHealthPoints() <= 0)
			{
				OnHealthToZero.Broadcast(target);
				Participants.Remove(target);
			}
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Doesn't have a CombatantComponant: %s"), *enemy->GetName());;
		}
	}
}

void UCombatSystem::EndTurn()
{
	if (++ParticipantIndex >= Participants.Num())
		ParticipantIndex = 0;
	
	UE_LOG(LogTemp, Display, TEXT("Participant: %i/%i"), ParticipantIndex, Participants.Num());

	CurrentParticipant = Participants[ParticipantIndex];
	OnTurnChanged.Broadcast(CurrentParticipant);

	UE_LOG(LogTemp, Display, TEXT("Turn changed to %s"), *CurrentParticipant->GetName());
}

void UCombatSystem::StartBattle(TArray<AActor*> particpants)
{
	UE_LOG(LogTemp, Display, TEXT("BattleStarted!"));
	Participants = particpants;
	CurrentParticipant = Participants[ParticipantIndex];

	UE_LOG(LogTemp, Display, TEXT("First turn is %s"), *CurrentParticipant->GetName());
}
