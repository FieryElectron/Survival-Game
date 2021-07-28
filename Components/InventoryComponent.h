#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Item.h"
#include "Items/EquippableItem.h"
#include "InventoryComponent.generated.h"

class UItem;
class UGearItem;
class UEquippableItem;
class ASurvivalCharacter;
class APickup;
class UInventoryItem;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVALGAME_API UInventoryComponent : public UActorComponent{
	GENERATED_BODY()

private:
	UPROPERTY(ReplicatedUsing = OnRep_Items, VisibleAnywhere, Category = "Inventory")
	TArray<UItem*> Items;

	UPROPERTY(ReplicatedUsing = OnRep_InventoryItem, VisibleAnywhere, Category = "Inventory")
	UInventoryItem* InventoryItem;

	UPROPERTY()
	int32 MaxWeight;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	EEquippableSlot ESlot;

	UItem* AddItemQuantity(UItem* Item, const uint32 Quantity);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

public:	
	UInventoryComponent();

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	//-- Gets --
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE bool IsEmpty() const { return !GetItems().Num(); }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE TArray<UItem*> GetItems() const { return Items; }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE int32 GetMaxWeight() const { return MaxWeight; };

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE EEquippableSlot GetSlot() const { return ESlot; };

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetCurrentWeight() const {
		int32 CurrentWeight = 0;
		for (auto& InvItem : Items) {
			if (InvItem) {
				CurrentWeight += (InvItem->GetWeight() * InvItem->GetQuantity());
			}
		}
		return CurrentWeight;
	}

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetRemainWeight() const {
		return (GetMaxWeight() - GetCurrentWeight());
	}

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE UInventoryItem* GetInventoryItem() const { return InventoryItem; }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE ASurvivalCharacter* GetSurvivalCharacter() const { return SurvivalCharacter; }

	FORCEINLINE int32 GetItemsRepKey() const { return ItemsRepKey; }

	//-- Sets --
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetSlot(EEquippableSlot NewSlot) { ESlot = NewSlot; };

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetMaxWeight(int32 NewWeight) { MaxWeight = NewWeight; }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetInventoryItem(UInventoryItem* NewInventoryItem) { InventoryItem = NewInventoryItem; }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetSurvivalCharacter(ASurvivalCharacter* NewSurvivalCharacter) { SurvivalCharacter = NewSurvivalCharacter; }

	//--

	UFUNCTION(BlueprintCallable, Category = "Item")
	void IncrementItemsRepKey() { ++ItemsRepKey; }

	UFUNCTION(BlueprintPure, Category = "Inventory")
	UItem* FindItemByUniqueID(UItem* Item) const;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	UItem* FindNoneStackFullItem(UItem* Item) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void TryAddItemQuantity(UItem* Item, int32 ExpectAddQuantity);

	UFUNCTION(Server, Reliable)
	void ServerTryAddItemQuantity(UItem* Item, int32 ExpectAddQuantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 ConsumeItem(UItem* Item, const int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(UItem* Item);

	UPROPERTY()
	int32 ItemsRepKey;

	UFUNCTION()
	void OnItemModified();

	UFUNCTION()
	void OnRep_Items();

	UFUNCTION()
	void OnRep_InventoryItem();

	

	//-- Equipment

	ASurvivalCharacter* SurvivalCharacter;

	//--

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	UEquippableItem* Equipment_AddNewSingleEquippableItem(UEquippableItem* EquippableItem);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void Equipment_RemoveSingleEquippableItem(UEquippableItem* EquippableItem);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	UEquippableItem* Equipment_FindEquippableItemBySlot(EEquippableSlot Slot) const;

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void Equipment_PreProcessWeaponItem(UEquippableItem* EquippableItem);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool Equipment_TryAddEquippableItem(UEquippableItem* EquippableItem);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool Equipment_TryUnEquipeEquippableItem(UEquippableItem* EquippableItem);

	//----

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool DropItemQuantity(UItem* Item, int32 DropQuantity);

};
