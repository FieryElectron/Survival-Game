#include "InventoryItem.h"
#include "Components/InventoryComponent.h"
#include "Components/EquipmentComponent.h"
#include "Player/SurvivalCharacter.h"

UInventoryItem::UInventoryItem() {
	InventoryWeight = 1;
}


bool UInventoryItem::Pre_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity) {
	bool bSuccessful = Super::Pre_AddItemQuantityToInventory(DestinationInventory, ExpectAddQuantity);

	if (bSuccessful && DestinationInventory && ExpectAddQuantity > 0) {
		UE_LOG(LogTemp, Warning, TEXT("UInventoryItem::TryAddItemQuantityToInventory"));

		if (GetInventoryComponent() == DestinationInventory) {
			return false;
		}

		ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(DestinationInventory->GetOwner());

		if (!Character) {
			return false;
		}

		if (UEquippableItem* equippedInventoryItem = Character->GetEquipmentComponent()->FindEquippableItemBySlot(GetSlot())) {
			if (GetUniqueID() == equippedInventoryItem->GetUniqueID()) {
				UInventoryComponent* UpdateInventory = Character->FindInventoryComponentBySlot(GetSlot());
				if (!UpdateInventory) {
					return false;
				}

				if (!UpdateInventory->IsEmpty()) {
					return false;
				}
			}
		}


	}
	return bSuccessful;
}

bool UInventoryItem::Post_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity) {
	bool bSuccessful = Super::Post_AddItemQuantityToInventory(DestinationInventory, ExpectAddQuantity);

	if (bSuccessful && DestinationInventory && ExpectAddQuantity > 0) {
		UE_LOG(LogTemp, Warning, TEXT("UInventoryItem::Post_AddItemQuantityToInventory"));

		ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(DestinationInventory->GetOwner());

		if (!Character) {
			return false;
		}

		if (Character->FindInventoryComponentBySlot(GetSlot()) == DestinationInventory) {
			return false;
		}

		if (UEquippableItem* equippedInventoryItem = Character->GetEquipmentComponent()->FindEquippableItemBySlot(GetSlot())) {
			if (GetUniqueID() == equippedInventoryItem->GetUniqueID()) {
				UInventoryComponent* UpdateInventory = Character->FindInventoryComponentBySlot(GetSlot());
				if (!UpdateInventory) {
					return false;
				}
				UpdateInventory->SetInventoryItem(NULL);
			}
		}
	}
	return bSuccessful;
}

bool UInventoryItem::Pre_EquipToEquipmentComponent(UEquipmentComponent* EquipmentComponent, EEquippableSlot TargetSlot) {
	bool bSuccessful = Super::Pre_EquipToEquipmentComponent(EquipmentComponent, TargetSlot);

	if (bSuccessful && EquipmentComponent) {
		UE_LOG(LogTemp, Warning, TEXT("UInventoryItem::TryAddToEquipmentComponent"));
	}
	return bSuccessful;
}

bool UInventoryItem::Post_EquipToEquipmentComponent(UEquipmentComponent* EquipmentComponent) {
	bool bSuccessful = Super::Post_EquipToEquipmentComponent(EquipmentComponent);

	if (bSuccessful && EquipmentComponent) {
		UE_LOG(LogTemp, Warning, TEXT("UInventoryItem::TryAddToEquipmentComponent"));

		if (ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(EquipmentComponent->GetOwner())) {
			if (UInventoryComponent* UpdateInventory = Character->FindInventoryComponentBySlot(GetSlot())) {
				SetInventoryComponent(UpdateInventory);

				UpdateInventory->SetMaxWeight(GetInventoryWeight());
				UpdateInventory->SetInventoryItem(this);
			}
		}
	}
	return bSuccessful;
}

bool UInventoryItem::Pre_UnEquipFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) {
	bool bSuccessful = Super::Pre_UnEquipFromEquipmentComponent(EquipmentComponent);

	if (bSuccessful && EquipmentComponent) {
		UE_LOG(LogTemp, Warning, TEXT("UInventoryItem::UnEquipFromEquipmentComponent"));

		ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(EquipmentComponent->GetOwner());
		if (!Character) {
			return false;
		}

		UInventoryComponent* UpdateInventory = Character->FindInventoryComponentBySlot(GetSlot());
		
		if (!UpdateInventory) {
			return false;
		}

		if (!(UpdateInventory->IsEmpty())) {
			return false;
		}

		if (!(Character->HaveEnoughWeightFromActiveInventory(GetWeight() * GetQuantity(), UpdateInventory))) {
			return false;
		}
	}
	return bSuccessful;
}

bool UInventoryItem::Post_UnEquipFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) {
	bool bSuccessful = Super::Post_UnEquipFromEquipmentComponent(EquipmentComponent);

	if (bSuccessful && EquipmentComponent) {
		UE_LOG(LogTemp, Warning, TEXT("UInventoryItem::Post_UnEquipFromEquipmentComponent"));

		ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(EquipmentComponent->GetOwner());
		if (!Character) {
			return false;
		}
		
		UInventoryComponent* UpdateInventory = Character->FindInventoryComponentBySlot(GetSlot());

		if (!UpdateInventory) {
			return false;
		}

		UpdateInventory->SetInventoryItem(NULL);
	}
	return bSuccessful;
}

bool UInventoryItem::Pre_DropFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) {
	bool bSuccessful = Super::Pre_DropFromEquipmentComponent(EquipmentComponent);

	if (bSuccessful && EquipmentComponent) {
		UE_LOG(LogTemp, Warning, TEXT("UInventoryItem::Pre_DropFromEquipmentComponent"));

		ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(EquipmentComponent->GetOwner());
		if (!Character) {
			return false;
		}

		UInventoryComponent* UpdateInventory = Character->FindInventoryComponentBySlot(GetSlot());

		if (!UpdateInventory) {
			return false;
		}

		if (!(UpdateInventory->IsEmpty())) {
			return false;
		}
	}
	return bSuccessful;
}

bool UInventoryItem::Post_DropFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) {
	bool bSuccessful = Super::Post_DropFromEquipmentComponent(EquipmentComponent);

	if (bSuccessful && EquipmentComponent) {
		UE_LOG(LogTemp, Warning, TEXT("UInventoryItem::Post_DropFromEquipmentComponent"));

		ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(EquipmentComponent->GetOwner());
		if (!Character) {
			return false;
		}

		UInventoryComponent* UpdateInventory = Character->FindInventoryComponentBySlot(GetSlot());

		if (!UpdateInventory) {
			return false;
		}

		UpdateInventory->SetInventoryItem(NULL);
	}
	return bSuccessful;
}


