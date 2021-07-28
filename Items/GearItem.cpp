#include "GearItem.h"
#include "Player/SurvivalCharacter.h"
#include "Components/EquipmentComponent.h"

UGearItem::UGearItem() {
	DamageDefenceMultiplier = 0.1f;
}

//bool UGearItem::Equip(class ASurvivalCharacter* Character) {
//	bool bEquipSuccessful = Super::Equip(Character);
//	if (bEquipSuccessful && Character) {
//		Character->EquipGear(this);
//	}
//	return bEquipSuccessful;
//}
//
//bool UGearItem::UnEquip(class ASurvivalCharacter* Character) {
//	bool bUnEquipSuccessful = Super::UnEquip(Character);
//	if (bUnEquipSuccessful && Character) {
//		Character->UnEquipGear(GetSlot());
//	}
//	return bUnEquipSuccessful;
//}

bool UGearItem::Pre_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity) {
	bool bSuccessful = Super::Pre_AddItemQuantityToInventory(DestinationInventory, ExpectAddQuantity);

	if (bSuccessful && DestinationInventory && ExpectAddQuantity > 0) {
		UE_LOG(LogTemp, Warning, TEXT("UGearItem::TryAddItemQuantityToInventory"));
	}
	return bSuccessful;
}

bool UGearItem::Post_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity) {
	bool bSuccessful = Super::Post_AddItemQuantityToInventory(DestinationInventory, ExpectAddQuantity);

	if (bSuccessful && DestinationInventory && ExpectAddQuantity > 0) {
		UE_LOG(LogTemp, Warning, TEXT("UGearItem::Post_AddItemQuantityToInventory"));
		if (ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(DestinationInventory->GetOwner())) {
			Character->UnEquipGear(GetSlot());
		}
	}
	return bSuccessful;
}

bool UGearItem::Pre_EquipToEquipmentComponent(UEquipmentComponent* EquipmentComponent, EEquippableSlot TargetSlot) {
	bool bSuccessful = Super::Pre_EquipToEquipmentComponent(EquipmentComponent, TargetSlot);

	if (bSuccessful && EquipmentComponent) {
		UE_LOG(LogTemp, Warning, TEXT("UGearItem::EquipToEquipmentComponent"));
	}
	return bSuccessful;
}

bool UGearItem::Post_EquipToEquipmentComponent(UEquipmentComponent* EquipmentComponent) {
	bool bSuccessful = Super::Post_EquipToEquipmentComponent(EquipmentComponent);

	if (bSuccessful && EquipmentComponent) {
		UE_LOG(LogTemp, Warning, TEXT("UGearItem::Post_EquipToEquipmentComponent"));
		if (ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(EquipmentComponent->GetOwner())) {
			Character->EquipGear(this);
		}
	}
	return bSuccessful;
}

bool UGearItem::Pre_UnEquipFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) {
	bool bSuccessful = Super::Pre_UnEquipFromEquipmentComponent(EquipmentComponent);

	if (bSuccessful && EquipmentComponent) {
		UE_LOG(LogTemp, Warning, TEXT("UGearItem::UnEquipFromEquipmentComponent"));
	}
	return bSuccessful;
}

bool UGearItem::Post_UnEquipFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) {
	bool bSuccessful = Super::Post_UnEquipFromEquipmentComponent(EquipmentComponent);

	if (bSuccessful && EquipmentComponent) {
		UE_LOG(LogTemp, Warning, TEXT("UGearItem::Post_UnEquipFromEquipmentComponent"));
		if (ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(EquipmentComponent->GetOwner())) {
			Character->UnEquipGear(GetSlot());
		}
	}
	return bSuccessful;
}


bool UGearItem::Pre_DropFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) {
	bool bSuccessful = Super::Pre_DropFromEquipmentComponent(EquipmentComponent);

	if (bSuccessful && EquipmentComponent) {
		UE_LOG(LogTemp, Warning, TEXT("UGearItem::Pre_DropFromEquipmentComponent"));
	}
	return bSuccessful;
}

bool UGearItem::Post_DropFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) {
	bool bSuccessful = Super::Post_DropFromEquipmentComponent(EquipmentComponent);

	if (bSuccessful && EquipmentComponent) {
		UE_LOG(LogTemp, Warning, TEXT("UGearItem::Post_DropFromEquipmentComponent"));
		if (ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(EquipmentComponent->GetOwner())) {
			Character->UnEquipGear(GetSlot());
		}
	}
	return bSuccessful;
}


