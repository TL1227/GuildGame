#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

class UCombatSystem;

UCLASS()
class GUILD_API AMyAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = "Turn")
	void TakeTurn();

	UCombatSystem* CombatSystem;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	//rotation
	bool NeedsToRotate = false;
	FRotator TargetRotation;
	UPROPERTY(EditAnywhere, Category = "Rotation")
	float RotationInterpolationSpeed = 3.0f;

	void Tick(float delta) override;
	
	//delegate binding
	FDelegateHandle OnTurnChangedBind;
	void CheckTurnChanged(AActor* player);

	FDelegateHandle OnHealthToZeroBind;
	void OnDeath(AActor* actor);

	void BindDelegates();
	void RemoveDelegateBinding();

public:	
	void BeginPlay() override;

};
