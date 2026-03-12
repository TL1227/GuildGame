// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CombatSystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FTurnChange, AActor*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeHealth, int, Damage, AActor*, Target);
DECLARE_MULTICAST_DELEGATE_OneParam(FZeroHealth, AActor*);

UCLASS()
class GUILD_API UCombatSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	TArray<AActor*> Participants;
	size_t ParticipantIndex = 0;
	AActor* CurrentParticipant;

public:
	void ApplyDamage(int damage, AActor* actor);

	void EndTurn();

	FTurnChange OnTurnChanged;

	UPROPERTY(BlueprintAssignable)
	FChangeHealth OnHealthChanged;

	FZeroHealth OnHealthToZero;

	UFUNCTION(BlueprintCallable, Category="Combat")
	void StartBattle(TArray<AActor*> particpants);
};

/*
	We probably want some kind of FTeam UStruct and the CS can hold a reference to the multiple teams
	So something like

	FTeam playerTeam { Mike };
	FTeam enemyTeam { Soldier_1, Soldier_2 }

	And they'd live in a list like so

	vector<FTeam> Teams;
	Teams.push_back(playerTeam);
	Teams.push_back(enemyTeam);

	Then the turn cycle can just be like
	FTeam* CurrentTurnTeam = Teams[0];

	OnTurnEnd()
	{
		CurrentTurnTeam = GetNextTeam();
		OnTurnStart.Broadcast(CurrentTurnTeam);
	}

	Then some kind of map that states the enemies/allies maybe?

	map<FTeam, vector<FTeam>> Enemies;
	Enemies[playerTeam].push_back(enemyTeam);
	Enemies[enemyTeam].push_back(playerTeam);
*/ 
