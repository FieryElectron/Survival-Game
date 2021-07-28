// SurvivalGame Project - The Unreal C++ Survival Game Course - Copyright Reuben Ward 2020


#include "SurvivalHUD.h"
#include "Widgets/InventoryWidget.h"
//#include "Widgets/NotificationWidget.h"
//#include "Widgets/MapWidget.h"
//#include "Widgets/LootWidget.h"
//#include "Widgets/PauseWidget.h"
#include "Widgets/GameplayWidget.h"
//#include "Widgets/DeathWidget.h"
//#include "InventoryWidget.h"
#include "Player/SurvivalPlayerController.h"


ASurvivalHUD::ASurvivalHUD() {
	bPersonalInfoWidgetIsVisible = false;


}

void ASurvivalHUD::BeginPlay() {
	Super::BeginPlay();

	SurvivalPlayerController = Cast<ASurvivalPlayerController>(GetOwningPlayerController());

	CreateGameplayWidget();
	//CreateInventoryWidget();
}

void ASurvivalHUD::CreateGameplayWidget() {
	if (!GamePlayWidget && GamePlayWidgetClass && PlayerOwner) {
		GamePlayWidget = CreateWidget<UGamePlayWidget>(PlayerOwner, GamePlayWidgetClass);

		if (GamePlayWidget && !GamePlayWidget->IsInViewport()) {
			GamePlayWidget->AddToViewport();
			GamePlayWidget->SurvivalHUD = this;
		}
	}
}

//void ASurvivalHUD::CreateInventoryWidget() {
//	if (!InventoryWidget && InventoryWidgetClass && PlayerOwner) {
//		InventoryWidget = CreateWidget<UInventoryWidget>(PlayerOwner, InventoryWidgetClass);
//
//		if (InventoryWidget && !InventoryWidget->IsInViewport()) {
//			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
//			InventoryWidget->AddToViewport();
//		}
//	}
//}


void ASurvivalHUD::ToggleInventoryWidget() {
	bPersonalInfoWidgetIsVisible = !bPersonalInfoWidgetIsVisible;
	
	if (bPersonalInfoWidgetIsVisible) {
		FInputModeGameAndUI GameAndUI;

		GameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		GameAndUI.SetHideCursorDuringCapture(false);
		PlayerOwner->bShowMouseCursor = true;

		PlayerOwner->SetInputMode(GameAndUI);
		PlayerOwner->GetPawn()->DisableInput(PlayerOwner);
		PlayerOwner->SetIgnoreLookInput(true);

	} else {
		FInputModeGameOnly GameOnly;
		PlayerOwner->bShowMouseCursor = false;

		PlayerOwner->SetInputMode(GameOnly);
		PlayerOwner->GetPawn()->EnableInput(PlayerOwner);
		PlayerOwner->SetIgnoreLookInput(false);
	}
}

void ASurvivalHUD::OpenPauseWidget() {
	//Create Pause widget if it doesn't exist
	//if (!PauseWidget && PauseWidgetClass && PlayerOwner) {
	//	PauseWidget = CreateWidget<UPauseWidget>(PlayerOwner, PauseWidgetClass);
	//}

	////Add to viewport
	//if (PauseWidget && !PauseWidget->IsInViewport()) {
	//	PauseWidget->AddToViewport(2);

	//	FInputModeUIOnly UIInput;
	//	UIInput.SetWidgetToFocus(PauseWidget->TakeWidget());

	//	PlayerOwner->bShowMouseCursor = true;
	//	PlayerOwner->SetInputMode(UIInput);
	//}
}

void ASurvivalHUD::ClosePauseWidget() {
	//if (PauseWidget && PlayerOwner) {
	//	PauseWidget->RemoveFromViewport();
	//	PlayerOwner->bShowMouseCursor = false;
	//	PlayerOwner->SetInputMode(FInputModeGameOnly());
	//}
}

void ASurvivalHUD::OpenLootWidget() {
	////Create Loot widget if it doesn't exist
	//if (!LootWidget && LootWidgetClass && PlayerOwner) {
	//	LootWidget = CreateWidget<ULootWidget>(PlayerOwner, LootWidgetClass);
	//}

	////Add to viewport
	//if (LootWidget) {
	//	LootWidget->AddToViewport();

	//	FInputModeUIOnly UIInput;
	//	UIInput.SetWidgetToFocus(LootWidget->TakeWidget());

	//	PlayerOwner->bShowMouseCursor = true;
	//	PlayerOwner->SetInputMode(UIInput);
	//}
}

void ASurvivalHUD::CloseLootWidget() {
	//if (LootWidget && LootWidget->IsInViewport()) {
	//	LootWidget->RemoveFromViewport();

	//	PlayerOwner->bShowMouseCursor = true;
	//	PlayerOwner->SetInputMode(FInputModeGameOnly());
	//}
}



void ASurvivalHUD::CreateNotificationWidget() {
	//Create Notification widget if it doesn't exist
	//if (!NotificationWidget && NotificationWidgetClass && PlayerOwner) {
	//	NotificationWidget = CreateWidget<UNotificationWidget>(PlayerOwner, NotificationWidgetClass);
	//}

	////Add to viewport
	//if (NotificationWidget) {
	//	//Keep notification widget on top by using high Z order
	//	NotificationWidget->AddToViewport(1);
	//}
}

void ASurvivalHUD::ShowNotification(const FText& NotificationText) {
	//if (NotificationWidget) {
	//	NotificationWidget->ShowNotification(NotificationText);
	//}
}

void ASurvivalHUD::ShowDeathWidget(class ASurvivalCharacter* Killer) {
	//If death widget was on screen remove it
	//if (GameplayWidget) {
	//	GameplayWidget->RemoveFromViewport();
	//}

	////Create Death widget if it doesn't exist
	//if (!DeathWidget && DeathWidgetClass && PlayerOwner) {
	//	DeathWidget = CreateWidget<UDeathWidget>(PlayerOwner, DeathWidgetClass);
	//}

	////Add to viewport
	//if (DeathWidget) {
	//	DeathWidget->Killer = Killer;
	//	//Keep Death widget on top by using high Z order
	//	DeathWidget->AddToViewport();
	//}
}

void ASurvivalHUD::OpenMap() {
	//Create Map widget if it doesn't exist
	//if (!MapWidget && MapWidgetClass && PlayerOwner) {
	//	MapWidget = CreateWidget<UMapWidget>(PlayerOwner, MapWidgetClass);
	//}

	////Add to viewport
	//if (MapWidget) {
	//	//Keep Map widget on top by using high Z order
	//	MapWidget->AddToViewport(1);
	//}
}

void ASurvivalHUD::CloseMap() {
	//if (MapWidget) {
	//	MapWidget->RemoveFromViewport();
	//}
}

void ASurvivalHUD::ShowHitmarker() {
	if (GamePlayWidget) {
		GamePlayWidget->ShowHitmarker();
	}
}

bool ASurvivalHUD::IsInventoryOpen() const {
	//return InventoryWidget ? InventoryWidget->IsInViewport() : false;
	return false;
}
