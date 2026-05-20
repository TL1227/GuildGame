// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

UCLASS()
class GUILD_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    UPROPERTY(BlueprintReadWrite)
    int ItemsCount = 0;

    UPROPERTY(BlueprintReadWrite)
    int CurrentItemIndex = 0;


    UFUNCTION(BlueprintCallable)
    void IncreaseIndex();
    
    UFUNCTION(BlueprintCallable)
    void DecreaseIndex();

    UFUNCTION(BlueprintImplementableEvent)
    void Up();

    UFUNCTION(BlueprintImplementableEvent)
    void Down();

    UFUNCTION(BlueprintImplementableEvent)
    void Right();

    UFUNCTION(BlueprintImplementableEvent)
    void Left();
	
public:
    UFUNCTION(BlueprintImplementableEvent)
    void Show();

    UFUNCTION(BlueprintImplementableEvent)
    void Hide();

    UFUNCTION(BlueprintImplementableEvent)
    void Confirm();

    UFUNCTION(BlueprintImplementableEvent)
    void Cancel();

    void Navigate(FVector2D Direction);
};
