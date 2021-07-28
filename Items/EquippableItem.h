#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "EquippableItem.generated.h"

UENUM(BlueprintType)
enum class EEquippableSlot : uint8 {
	EIS_None			UMETA(DisplayName = "None"),

	EIS_Face			UMETA(DisplayName = "Face"),
	EIS_Eyes			UMETA(DisplayName = "Eyes"),
	EIS_Head			UMETA(DisplayName = "Head"),
	EIS_ChestArmor		UMETA(DisplayName = "ChestArmor"),
	EIS_Chest			UMETA(DisplayName = "Chest"),
	EIS_Back			UMETA(DisplayName = "Back"),
	EIS_Belt			UMETA(DisplayName = "Belt"),
	EIS_Hands			UMETA(DisplayName = "Hands"),
	EIS_Legs			UMETA(DisplayName = "Legs"),
	EIS_Feet			UMETA(DisplayName = "Feet"),

	EIS_Weapon			UMETA(DisplayName = "Weapon"),

	EIS_PrimaryWeapon	UMETA(DisplayName = "PrimaryWeapon"),
	EIS_SecondaryWeapon	UMETA(DisplayName = "SecondaryWeapon"),
	EIS_TertiaryWeapon	UMETA(DisplayName = "TertiaryWeapon"),
	EIS_Item1			UMETA(DisplayName = "Item1"),
	EIS_Item2			UMETA(DisplayName = "Item2"),

	EIS_Throwable		UMETA(DisplayName = "Throwable"),

	EIS_Equipments		UMETA(DisplayName = "Equipments")

};

UCLASS(Abstract, NotBlueprintable)
class SURVIVALGAME_API UEquippableItem : public UItem
{
	GENERATED_BODY()

private:
	UPROPERTY(Replicated, EditDefaultsOnly)
	EEquippableSlot Slot;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UEquippableItem();

	//-- Gets --
	UFUNCTION(BlueprintCallable, Category = "EquippableItem")
	FORCEINLINE EEquippableSlot GetSlot() const { return Slot; }


	//-- Sets --
	UFUNCTION(BlueprintCallable, Category = "EquippableItem")
	void SetSlot(EEquippableSlot NewSlot) { Slot = NewSlot; }


	virtual void Use(class ASurvivalCharacter* Character) override;

	UFUNCTION(BlueprintCallable, Category = "EquippableItem")
	virtual bool Equip(class ASurvivalCharacter* Character);

	UFUNCTION(BlueprintCallable, Category = "EquippableItem")
	virtual bool UnEquip(class ASurvivalCharacter* Character);


	virtual bool Pre_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity) override;
	virtual bool Post_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity) override;


	virtual bool Pre_EquipToEquipmentComponent(UEquipmentComponent* EquipmentComponent, EEquippableSlot TargetSlot);
	virtual bool Post_EquipToEquipmentComponent(UEquipmentComponent* EquipmentComponent);
	
	virtual bool Pre_UnEquipFromEquipmentComponent(UEquipmentComponent* EquipmentComponent);
	virtual bool Post_UnEquipFromEquipmentComponent(UEquipmentComponent* EquipmentComponent);


	virtual bool Pre_DropFromEquipmentComponent(UEquipmentComponent* EquipmentComponent);
	virtual bool Post_DropFromEquipmentComponent(UEquipmentComponent* EquipmentComponent);

	void PreProcessWeaponItem(UEquipmentComponent* EquipmentComponent);
};
