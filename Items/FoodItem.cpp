#include "FoodItem.h"
#include "Player/SurvivalCharacter.h"
#include "Components/InventoryComponent.h"

bool UFoodItem::Pre_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity) {
	bool bSuccessful = Super::Pre_AddItemQuantityToInventory(DestinationInventory, ExpectAddQuantity);

	if (bSuccessful && DestinationInventory && ExpectAddQuantity > 0) {
		UE_LOG(LogTemp, Warning, TEXT("FoodItem::TryAddItemQuantityToInventory"));
	}

	return bSuccessful;
}

bool UFoodItem::Post_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity) {
	bool bSuccessful = Super::Post_AddItemQuantityToInventory(DestinationInventory, ExpectAddQuantity);

	if (bSuccessful && DestinationInventory && ExpectAddQuantity > 0) {
		UE_LOG(LogTemp, Warning, TEXT("FoodItem::Post_AddItemQuantityToInventory"));
	}

	return bSuccessful;
}