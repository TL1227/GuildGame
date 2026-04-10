// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CombatSystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FTurnChange, AActor*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FZeroHealth, AActor*, DeadActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeHealth, int, Damage, AActor*, Target);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBattleStart);

class UTeam;

UCLASS()
class GUILD_API UCombatSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	TArray<UTeam*> Teams;
	AActor* CurrentParticipant;
	int TeamCount;
	int TeamIndex = 0;
	int MemberCount;
	int MemberIndex = 0;

public:
	void RemoveParticipant(AActor* participant);
	void SetParticipant(AActor* participant);

	UPROPERTY(BlueprintAssignable)
	FChangeHealth OnHealthChanged;

	void ApplyDamage(int damage, AActor* actor);

	FTurnChange OnTurnChanged;
	void EndTurn();

	UPROPERTY(BlueprintAssignable)
	FZeroHealth OnHealthToZero;

	UPROPERTY(BlueprintAssignable)
	FBattleStart OnBattleStarted;

	UFUNCTION(BlueprintCallable, Category="Combat")
	void StartBattle(TArray<AActor*> participants);

	AActor* GetNextParticipant();
};
