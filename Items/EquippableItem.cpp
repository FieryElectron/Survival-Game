#include "EquippableItem.h"
#include "Net/UnrealNetwork.h"
#include "Player/SurvivalCharacter.h"
#include "Components/InventoryComponent.h"
#include "Components/EquipmentComponent.h"
#include "World/Pickup.h"

#define LOCTEXT_NAMESPACE "EquippableItem"

UEquippableItem::UEquippableItem() {
	SetStackable(false);

	//UseActionText = LOCTEXT("EquipText", "Equip");
}

void UEquippableItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEquippableItem, Slot);
}

void UEquippableItem::Use(class ASurvivalCharacter* Character) {
	if (Character && Character->HasAuthority()) {
		Equip(Character);
	}
}

bool UEquippableItem::Equip(class ASurvivalCharacter* Character) {
	if (Character) {
		return Character->EquipItem(this);
	}
	return false;
}

bool UEquippableItem::UnEquip(class ASurvivalCharacter* Character) {
	if (Character) {
		return Character->UnEquipItem(this);
	}
	return false;
}

void UEquippableItem::PreProcessWeaponItem(UEquipmentComponent* EquipmentComponent) {
	if (GetSlot() == EEquippableSlot::EIS_Weapon) {
		if (ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(EquipmentComponent->GetOwner())) {
			for (auto slot : Character->GetWeaponSlots()) {
				UEquippableItem* AlreadyEquippedWeaponItem = EquipmentComponent->FindEquippableItemBySlot(slot);
				if (!AlreadyEquippedWeaponItem) {
					SetSlot(slot);
					return;
				}
			}
		}
	}
}

bool UEquippableItem::Pre_EquipToEquipmentComponent(UEquipmentComponent* EquipmentComponent, EEquippableSlot TargetSlot) {
	EEquippableSlot FinalTargetSlot;

	if (TargetSlot == EEquippableSlot::EIS_None) {
		PreProcessWeaponItem(EquipmentComponent);

		FinalTargetSlot = GetSlot();
	} else {
		if (GetSlot() == EEquippableSlot::EIS_Weapon) {
			ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(EquipmentComponent->GetOwner());

			if (!Character) {
				return false;
			}

			if (!(Character->WeaponSlots.Contains(TargetSlot))) {
				return false;
			}

		} else {
			if (GetSlot() != TargetSlot) {
				return false;
			}
		}
		FinalTargetSlot = TargetSlot;
	}

	if (FinalTargetSlot == EEquippableSlot::EIS_Weapon) {
		return false;
	}

	UEquippableItem* AlreadyEquippedItem = EquipmentComponent->FindEquippableItemBySlot(FinalTargetSlot);

	if (AlreadyEquippedItem) {
		return false;
	}

	SetSlot(FinalTargetSlot);

	return true;
}

bool UEquippableItem::Post_EquipToEquipmentComponent(UEquipmentComponent* EquipmentComponent) {
	return true;
}


bool UEquippableItem::Pre_UnEquipFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) {

	UEquippableItem* AlreadyEquippedItem = EquipmentComponent->FindEquippableItemBySlot(GetSlot());
	if (!AlreadyEquippedItem) {
		return false;
	} 

	ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(EquipmentComponent->GetOwner());
	if (!Character) {
		return false;
	}

	if (!(Character->HaveEnoughWeightFromActiveInventory(GetWeight() * GetQuantity()))) {
		return false;
	}

	return true;
}

bool UEquippableItem::Post_UnEquipFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) {
	return true;
}


bool UEquippableItem::Pre_DropFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) {

	UEquippableItem* AlreadyEquippedItem = EquipmentComponent->FindEquippableItemBySlot(GetSlot());
	if (!AlreadyEquippedItem) {
		return false;
	}

	ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(EquipmentComponent->GetOwner());
	if (!Character) {
		return false;
	}

	return true;
}

bool UEquippableItem::Post_DropFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) {
	return true;
}



bool UEquippableItem::Pre_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity) {
	bool bSuccessful = Super::Pre_AddItemQuantityToInventory(DestinationInventory, ExpectAddQuantity);

	if (bSuccessful && DestinationInventory && ExpectAddQuantity > 0) {
		UE_LOG(LogTemp, Warning, TEXT("UEquippableItem::TryAddItemQuantityToInventory"));
		//if (DestinationInventory == Character->GetEquipmentsInventory()) {
		//	if (UEquippableItem* AlreadyEquippedItem = DestinationInventory->Equipment_FindEquippableItemBySlot(GetSlot())) {
		//		return false;
		//	}
		//}
	}

	return bSuccessful;
}

bool UEquippableItem::Post_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity) {
	bool bSuccessful = Super::Post_AddItemQuantityToInventory(DestinationInventory, ExpectAddQuantity);

	if (bSuccessful && DestinationInventory && ExpectAddQuantity > 0) {
		UE_LOG(LogTemp, Warning, TEXT("UEquippableItem::Post_AddItemQuantityToInventory"));
		//if (DestinationInventory == Character->GetEquipmentsInventory()) {
		//	if (UEquippableItem* AlreadyEquippedItem = DestinationInventory->Equipment_FindEquippableItemBySlot(GetSlot())) {
		//		return false;
		//	}
		//}
	}

	return bSuccessful;
}

#undef LOCTEXT_NAMESPACE 