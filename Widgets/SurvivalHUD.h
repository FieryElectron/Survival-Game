// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SurvivalHUD.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API ASurvivalHUD : public AHUD
{
	GENERATED_BODY()
public:

	ASurvivalHUD();

	class ASurvivalPlayerController* SurvivalPlayerController;

	virtual void BeginPlay() override;

	void ToggleInventoryWidget();

	void OpenPauseWidget();
	void ClosePauseWidget();

	void CreateGameplayWidget();
	//void CreateInventoryWidget();

	void CreateNotificationWidget();
	void ShowNotification(const FText& NotificationText);

	void ShowDeathWidget(class ASurvivalCharacter* Killer);

	void OpenMap();
	void CloseMap();

	void OpenLootWidget();
	void CloseLootWidget();

	void ShowHitmarker();

	bool IsInventoryOpen() const;

protected:

	/**Widget classes and refs*/
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UInventoryWidget> InventoryWidgetClass;

	//UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	//	TSubclassOf<class UNotificationWidget> NotificationWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UGamePlayWidget> GamePlayWidgetClass;

	//UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	//	TSubclassOf<class UMapWidget> MapWidgetClass;

	//UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	//	TSubclassOf<class UDeathWidget> DeathWidgetClass;

	//UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	//	TSubclassOf<class ULootWidget> LootWidgetClass;

	//UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	//	TSubclassOf<class UPauseWidget> PauseWidgetClass;

public:

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	class UInventoryWidget* InventoryWidget;

	//UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	//	class UNotificationWidget* NotificationWidget;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	class UGamePlayWidget* GamePlayWidget;

	//UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	//	class UMapWidget* MapWidget;

	//UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	//	class UDeathWidget* DeathWidget;

	//UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	//	class ULootWidget* LootWidget;

	//UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	//	class UPauseWidget* PauseWidget;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	bool bPersonalInfoWidgetIsVisible;
	
};
