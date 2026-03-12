#include "Combatant.h"
#include "CombatSystem.h"

UCombatant::UCombatant()
{
	//PrimaryComponentTick.bCanEverTick = true;
}

void UCombatant::BeginPlay()
{
	Super::BeginPlay();

	//UCombatSystem* combatSystem = GetOwner()->GetGameInstance()->GetSubsystem<UCombatSystem>();
}

void UCombatant::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatant::ChangeHealthPoints(int points)
{
	UE_LOG(LogTemp, Display, TEXT("Took %i damage"), points);
	HealthPoints -= points;
	UE_LOG(LogTemp, Display, TEXT("Health: %i"), HealthPoints);
}

float UCombatant::GetHealthAsPercentage()
{
	return (float)HealthPoints / (float)MaxHealth;
}

int UCombatant::GetBaseAttackDamage() { return BaseAttackDamage; }

void UCombatant::StartTurn() const
{
	OnTurnStart.Execute();
}

int UCombatant::GetHealthPoints()
{
	return HealthPoints;
}
