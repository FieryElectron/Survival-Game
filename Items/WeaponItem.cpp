#include "WeaponItem.h"
#include "Player/SurvivalPlayerController.h"
#include "Player/SurvivalCharacter.h"
#include "Weapons/Weapon.h"
#include "Engine/World.h"
#include "Components/InventoryComponent.h"
#include "Components/EquipmentComponent.h"

UWeaponItem::UWeaponItem() {

}

//bool UWeaponItem::Equip(class ASurvivalCharacter* Character) {
//	bool bEquipSuccessful = Super::Equip(Character);
//
//	if (bEquipSuccessful && Character) {
//		Character->EquipWeapon(this);
//	}
//
//	return bEquipSuccessful;
//}
//
//bool UWeaponItem::UnEquip(class ASurvivalCharacter* Character) {
//	bool bUnEquipSuccessful = Super::UnEquip(Character);
//
//	if (bUnEquipSuccessful && Character) {
//		Character->UnEquipWeapon(this);
//	}
//
//	return bUnEquipSuccessful;
//}

AWeapon* UWeaponItem::Spawn(APawn* Instigator) {
	if (WeaponClass && Instigator) {

		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParams.Owner = SpawnParams.Instigator = Instigator;

		return GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnParams);
	}
	return NULL;
}


bool UWeaponItem::Pre_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity) {
	bool bSuccessful = Super::Pre_AddItemQuantityToInventory(DestinationInventory, ExpectAddQuantity);

	if (bSuccessful && DestinationInventory && ExpectAddQuantity > 0) {
		UE_LOG(LogTemp, Warning, TEXT("UWeaponItem::TryAddItemQuantityToInventory"));
	}
	return bSuccessful;
}

bool UWeaponItem::Post_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity) {
	bool bSuccessful = Super::Post_AddItemQuantityToInventory(DestinationInventory, ExpectAddQuantity);

	if (bSuccessful && DestinationInventory && ExpectAddQuantity > 0) {
		UE_LOG(LogTemp, Warning, TEXT("UWeaponItem::Post_AddItemQuantityToInventory"));

		if (ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(DestinationInventory->GetOwner())) {
			if (UEquippableItem* equippedInventoryItem = Character->GetEquipmentComponent()->FindEquippableItemBySlot(GetSlot())) {
				if (GetUniqueID() == equippedInventoryItem->GetUniqueID()) {
					Character->UnEquipWeapon(this);
				}
			}
		}
	}
	return bSuccessful;
}

bool UWeaponItem::Pre_EquipToEquipmentComponent(UEquipmentComponent* EquipmentComponent, EEquippableSlot TargetSlot) {
	bool bSuccessful = Super::Pre_EquipToEquipmentComponent(EquipmentComponent, TargetSlot);

	if (bSuccessful && EquipmentComponent) {
		UE_LOG(LogTemp, Warning, TEXT("UWeaponItem::EquipToEquipmentComponent"));
	}
	return bSuccessful;
}

bool UWeaponItem::Post_EquipToEquipmentComponent(UEquipmentComponent* EquipmentComponent) {
	bool bSuccessful = Super::Post_EquipToEquipmentComponent(EquipmentComponent);

	if (bSuccessful && EquipmentComponent) {
		UE_LOG(LogTemp, Warning, TEXT("UWeaponItem::EquipToEquipmentComponent"));
		if (ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(EquipmentComponent->GetOwner())) {
			Character->EquipWeapon(this);
		}
	}
	return bSuccessful;
}

bool UWeaponItem::Pre_UnEquipFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) {
	bool bSuccessful = Super::Pre_UnEquipFromEquipmentComponent(EquipmentComponent);

	if (bSuccessful && EquipmentComponent) {
		UE_LOG(LogTemp, Warning, TEXT("UWeaponItem::UnEquipFromEquipmentComponent"));
	}
	return bSuccessful;
}

bool UWeaponItem::Post_UnEquipFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) {
	bool bSuccessful = Super::Post_UnEquipFromEquipmentComponent(EquipmentComponent);

	if (bSuccessful && EquipmentComponent) {
		UE_LOG(LogTemp, Warning, TEXT("UWeaponItem::UnEquipFromEquipmentComponent"));
		if (ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(EquipmentComponent->GetOwner())) {
			Character->UnEquipWeapon(this);
		}
	}
	return bSuccessful;
}

bool UWeaponItem::Pre_DropFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) {
	bool bSuccessful = Super::Pre_DropFromEquipmentComponent(EquipmentComponent);

	if (bSuccessful && EquipmentComponent) {
		UE_LOG(LogTemp, Warning, TEXT("UWeaponItem::Pre_DropFromEquipmentComponent"));
	}
	return bSuccessful;
}

bool UWeaponItem::Post_DropFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) {
	bool bSuccessful = Super::Post_DropFromEquipmentComponent(EquipmentComponent);

	if (bSuccessful && EquipmentComponent) {
		UE_LOG(LogTemp, Warning, TEXT("UWeaponItem::Post_DropFromEquipmentComponent"));
		if (ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(EquipmentComponent->GetOwner())) {
			Character->UnEquipWeapon(this);
		}
	}
	return bSuccessful;
}

