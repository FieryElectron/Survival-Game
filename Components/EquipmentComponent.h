#pragma once

#include "CoreMinimal.h"
#include "Components/InventoryComponent.h"
#include "EquipmentComponent.generated.h"

class UEquippableItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentUpdated);

UCLASS()
class SURVIVALGAME_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(ReplicatedUsing = OnRep_EquippableItems, VisibleAnywhere, Category = "Inventory")
	TArray<UItem*> Items;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

public:
	UEquipmentComponent();

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FEquipmentUpdated OnEquipmentUpdated;

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool EquipEquippableItem(UEquippableItem* EquippableItem, EEquippableSlot TargetSlot = EEquippableSlot::EIS_None);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool UnEquipEquippableItem(UEquippableItem* EquippableItem);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool DropEquippableItem(UEquippableItem* EquippableItem);


	
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	UEquippableItem* FindEquippableItemBySlot(EEquippableSlot Slot) const;

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void PreProcessWeaponItem(UEquippableItem* EquippableItem);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	UEquippableItem* AddNewSingleEquippableItem(UEquippableItem* EquippableItem);

	UFUNCTION()
	void OnRep_EquippableItems();

	UPROPERTY()
	int32 EquippableItemsRepKey;


	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void IncrementEquippableItemsRepKey() { ++EquippableItemsRepKey; }

	FORCEINLINE int32 GetEquippableItemsRepKey() const { return EquippableItemsRepKey; }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE TArray<UItem*> GetItems() const { return Items; }

	bool RemoveItem(UItem* Item);

};
