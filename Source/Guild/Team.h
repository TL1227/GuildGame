// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Team.generated.h"

class AActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUILD_API UTeam : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AActor*> Members;

	UPROPERTY()
	int CurrentMemberIndex = 0;

	UPROPERTY()
	int Id;

public:
	void SetId(int id);
	int GetId();
	void SetMembers(TArray<AActor*> members);
	void AddMember(AActor* member);
	TArray<AActor*> GetAllMembers();
	AActor* GetCurrentParticipant();
	AActor* GetNextParticipant();
	void SetCurrentParticipantIndex(int index);
	int GetCurrentParticipantIndex();
	void RemoveParticipant(AActor* member);
};
