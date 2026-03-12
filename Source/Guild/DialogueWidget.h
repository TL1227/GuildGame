// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

UCLASS()
class GUILD_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintImplementableEvent)
    void SetDialogueText(const FText& Speaker, const FText& Line);

    UFUNCTION(BlueprintImplementableEvent)
    void Show();

    UFUNCTION(BlueprintImplementableEvent)
    void Hide();

};
