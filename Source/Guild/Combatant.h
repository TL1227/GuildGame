#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combatant.generated.h"

DECLARE_DELEGATE(FTurnDelegate)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUILD_API UCombatant : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatant();
	void ChangeHealthPoints(int);

	UFUNCTION(BluePrintCallable)
	float GetHealthAsPercentage();

	UFUNCTION(BluePrintCallable)
	int GetHealthPoints();

	FTurnDelegate OnTurnStart;
	void StartTurn() const;

	void SetTeamId(int id);
	int GetTeamId();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
	int HealthPoints = 100;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
	int MaxHealth = 100;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
	int BaseAttackDamage;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
	int TeamId = 0;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	int GetBaseAttackDamage();
};
