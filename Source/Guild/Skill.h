#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h"
#include "Skill.generated.h"

USTRUCT(BlueprintType)
struct FSkill : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* AnimationMontage;

};

