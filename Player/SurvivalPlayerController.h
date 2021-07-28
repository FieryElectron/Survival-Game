// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SurvivalPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API ASurvivalPlayerController : public APlayerController
{
	GENERATED_BODY()

	ASurvivalPlayerController();

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void Turn(float Value);
	void LookUp(float Value);

	void ToggleInventoryWidget();

public:
	UFUNCTION(Client, Reliable)
	void ClientShotHitConfirmed();


	
};
