// Fill out your copyright notice in the Description page of Project Settings.

#include "Team.h"
#include "GameFramework/Actor.h"

DECLARE_LOG_CATEGORY_EXTERN(Team, Display, All);
DEFINE_LOG_CATEGORY(Team);

void UTeam::SetId(int id) { Id = id; }
int UTeam::GetId() { return Id; }

void UTeam::SetMembers(TArray<AActor*> members) { Members = members; }
void UTeam::AddMember(AActor* member) { Members.Add(member); }

AActor* UTeam::GetCurrentParticipant()
{
	if (Members.IsValidIndex(CurrentMemberIndex))
	{
		UE_LOG(Team, Display, TEXT("Team: %i CurrentMemberIndex: %i"), Id, CurrentMemberIndex);
		return Members[CurrentMemberIndex];
	}
	else
	{
		UE_LOG(Team, Display, TEXT("Index is too big! %i"), CurrentMemberIndex);
		return nullptr;
	}
}

//This will return nullptr if the last team member has taken their turn
AActor* UTeam::GetNextParticipant()
{
	return (Members.IsValidIndex(++CurrentMemberIndex))
		? Members[CurrentMemberIndex] : nullptr;
}

void UTeam::SetCurrentParticipantIndex(int index)
{
	CurrentMemberIndex = index;
}

int UTeam::GetCurrentParticipantIndex()
{
	return CurrentMemberIndex;
}

void UTeam::RemoveParticipant(AActor* member)
{
	if (!IsValid(member))
	{
		UE_LOG(Team, Warning, TEXT("Tried to remove invalid member"));
		return;
	}
	
	Members.Remove(member);
	
	UE_LOG(Team, Display, TEXT("Removed from Team %i Member %s"), Id, *member->GetName());
	UE_LOG(Team, Display, TEXT("Team %i CurrentIndex %i Count %i"), Id, CurrentMemberIndex, Members.Num());
}
