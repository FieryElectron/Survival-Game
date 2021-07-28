#pragma once

#include "CoreMinimal.h"
#include "Items/EquippableItem.h"
#include "WeaponItem.generated.h"

class AWeapon;
class UEquipmentComponent;

UCLASS(Blueprintable)
class SURVIVALGAME_API UWeaponItem : public UEquippableItem {
	GENERATED_BODY()
public:
	UWeaponItem();

	//virtual bool Equip(class ASurvivalCharacter* Character) override;
	//virtual bool UnEquip(class ASurvivalCharacter* Character) override;

	AWeapon* Spawn(APawn* Instigator);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass;

	virtual bool Pre_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity) override;
	virtual bool Post_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity) override;

	virtual bool Pre_EquipToEquipmentComponent(UEquipmentComponent* EquipmentComponent, EEquippableSlot TargetSlot) override;
	virtual bool Post_EquipToEquipmentComponent(UEquipmentComponent* EquipmentComponent) override;

	virtual bool Pre_UnEquipFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) override;
	virtual bool Post_UnEquipFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) override;

	virtual bool Pre_DropFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) override;
	virtual bool Post_DropFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) override;

	
	
};
