#pragma once

#include "CoreMinimal.h"
#include "Items/EquippableItem.h"
#include "Items/GearItem.h"
#include "InventoryItem.generated.h"

class UInteractionComponent;


UCLASS()
class SURVIVALGAME_API UInventoryItem : public UGearItem
{
	GENERATED_BODY()
private:
	UInventoryComponent* InventoryComponent;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1, ClampMax = 2000))
	int32 InventoryWeight;

public:
	UInventoryItem();

	UFUNCTION(BlueprintCallable, Category = "InventoryItem")
	FORCEINLINE UInventoryComponent* GetInventoryComponent() const {return InventoryComponent;}

	UFUNCTION(BlueprintCallable, Category = "InventoryItem")
	void SetInventoryComponent(UInventoryComponent* NewInventoryComponent) { InventoryComponent = NewInventoryComponent; }

	UFUNCTION(BlueprintPure, Category = "GearItem")
	FORCEINLINE	int32 GetInventoryWeight() const { return InventoryWeight; };


	virtual bool Pre_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity) override;
	virtual bool Post_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity) override;

	virtual bool Pre_EquipToEquipmentComponent(UEquipmentComponent* EquipmentComponent, EEquippableSlot TargetSlot) override;
	virtual bool Post_EquipToEquipmentComponent(UEquipmentComponent* EquipmentComponent) override;

	virtual bool Pre_UnEquipFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) override;
	virtual bool Post_UnEquipFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) override;

	virtual bool Pre_DropFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) override;
	virtual bool Post_DropFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) override;


	
	
};
