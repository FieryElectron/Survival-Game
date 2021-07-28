// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GamePlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UGamePlayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void ShowHitmarker();


	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	class ASurvivalHUD* SurvivalHUD;
};
