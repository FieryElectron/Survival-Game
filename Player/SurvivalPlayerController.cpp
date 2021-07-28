// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalPlayerController.h"
#include "Widgets/SurvivalHUD.h"

ASurvivalPlayerController::ASurvivalPlayerController() {

}


void ASurvivalPlayerController::BeginPlay(){
	Super::BeginPlay();
}

void ASurvivalPlayerController::SetupInputComponent(){
	Super::SetupInputComponent();

	InputComponent->BindAxis("Turn", this, &ASurvivalPlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &ASurvivalPlayerController::LookUp);

	InputComponent->BindAction("ToggleInventoryWidget", IE_Pressed, this, &ASurvivalPlayerController::ToggleInventoryWidget);


}


void ASurvivalPlayerController::Turn(float Value){
	if (Value != 0.0f) {
		AddYawInput(Value);
	}
}

void ASurvivalPlayerController::LookUp(float Value){
	if (Value != 0.0f) {
		AddPitchInput(Value);
	}
}

void ASurvivalPlayerController::ToggleInventoryWidget() {
	if (ASurvivalHUD* HUD = Cast<ASurvivalHUD>(GetHUD())) {
		HUD->ToggleInventoryWidget();
	}
}

void ASurvivalPlayerController::ClientShotHitConfirmed_Implementation() {
	if (ASurvivalHUD* HUD = Cast<ASurvivalHUD>(GetHUD())) {
		HUD->ShowHitmarker();
	}
}