#include "MyAIController.h"
#include "Combatant.h"
#include "Kismet/GameplayStatics.h"
#include "GuildCharacter.h"
#include "CombatSystem.h"
#include "EnemyCharacter.h"
#include "Templates/Casts.h"
#include <Kismet/KismetMathLibrary.h>

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();

	CombatSystem = GetGameInstance()->GetSubsystem<UCombatSystem>();
	BindDelegates();
}

void AMyAIController::Tick(float delta)
{
	Super::Tick(delta);

	DrawDebugSphere(GetWorld(), MoveTarget, 10.0f, 8, FColor::Magenta);

	if (NeedsToRotate)
	{
		FRotator newRot = FMath::RInterpTo(
			GetControlRotation(), 
			TargetRotation, 
			delta, 
			RotationInterpolationSpeed
		);

		SetControlRotation(newRot);

		UE_LOG(LogTemp, Display, TEXT("Target: %f\t, New: %f"), TargetRotation.Yaw, newRot.Yaw);

		if (newRot.Equals(TargetRotation, 2.0f))
		{
			NeedsToRotate = false;

			APawn* player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

			if (AEnemyCharacter* ch = Cast<AEnemyCharacter>(GetPawn()))
			{
				CombatSystem->ApplyDamage(ch->GetCombatantComponent()->GetBaseAttackDamage(), player);
				CombatSystem->EndTurn();
			}
		}
	}
}

void AMyAIController::CheckTurnChanged(AActor* player)
{
	UE_LOG(LogTemp, Display, TEXT("%s listening to turn change"), *GetPawn()->GetName());
	if (IsValid(player) && player == GetPawn())
	{
		UE_LOG(LogTemp, Display, TEXT("it's %s turn"), *player->GetName());
		TakeTurn();
	}
}

void AMyAIController::TakeTurn()
{
	//Get our controlled pawn
	APawn* pawn = GetPawn();

	//Check it's got a combatant component
	if (pawn->GetComponentByClass<UCombatant>())
	{
		APawn* player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		//TODO: Just testing out if the offest makes pathfinding any less shit
		UE_LOG(LogTemp, Display, TEXT("Moving to %s"), *player->GetName());
		AEnemyCharacter* ch = Cast<AEnemyCharacter>(pawn);
		auto target = player->GetActorLocation() + ch->PersonalOffset;
		MoveTarget = target;
		MoveToLocation(target, ch->AcceptanceRadius);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Can't find the UCombatant on pawn!"));
	}
}

void AMyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (CombatSystem)
	{
		//TODO: find targets dynamically
		APawn* player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), player->GetActorLocation());
		if (!GetControlRotation().Equals(TargetRotation, 2.0f))
		{
			UE_LOG(LogTemp, Display, TEXT("Rotating!!!"));
			NeedsToRotate = true;
		}
		else
		{
			if (AEnemyCharacter* ch = Cast<AEnemyCharacter>(GetPawn()))
			{
				CombatSystem->ApplyDamage(ch->GetCombatantComponent()->GetBaseAttackDamage(), player);
				CombatSystem->EndTurn();
			}
		}
	}
}

void AMyAIController::OnDeath(AActor* actor)
{
	if (GetPawn() == actor)
	{
		GetPawn()->Destroy();
		RemoveDelegateBinding();
		Destroy();
	}
}

void AMyAIController::BindDelegates()
{
	OnTurnChangedBind = CombatSystem->OnTurnChanged.AddUObject(this, &AMyAIController::CheckTurnChanged);
	CombatSystem->OnHealthToZero.AddDynamic(this, &AMyAIController::OnDeath);
}

void AMyAIController::RemoveDelegateBinding()
{
	CombatSystem->OnHealthToZero.RemoveDynamic(this, &AMyAIController::OnDeath);
	CombatSystem->OnTurnChanged.Remove(OnTurnChangedBind);
}
