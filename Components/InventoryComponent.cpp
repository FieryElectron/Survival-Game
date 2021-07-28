#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Player/SurvivalCharacter.h"
#include "Items/GearItem.h"
#include "Items/InventoryItem.h"
#include "World/Pickup.h"


#define LOCTEXT_NAMESPACE "Inventory"

UInventoryComponent::UInventoryComponent(){
	SetIsReplicated(true);

	MaxWeight = 50;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UInventoryComponent, Items, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UInventoryComponent, InventoryItem, COND_OwnerOnly);
}

UItem* UInventoryComponent::AddItemQuantity(class UItem* Item, const uint32 Quantity) {
	if (GetOwner() && GetOwner()->HasAuthority()) {
		UItem* NewItem = NewObject<UItem>(GetOwner(), Item->GetClass());
		NewItem->SetQuantity(Quantity);
		NewItem->SetOwningInventory(this);
		Items.Add(NewItem);

		NewItem->IncrementRepKey();
		IncrementItemsRepKey();

		return NewItem;
	}
	return NULL;
}

UItem* UInventoryComponent::FindItemByUniqueID(class UItem* Item) const {
	if (Item) {
		for (auto& InvItem : Items) {
			if ( InvItem->GetUniqueID() == Item->GetUniqueID()) {
				return InvItem;
			}
		}
	}
	return nullptr;
}

UItem* UInventoryComponent::FindNoneStackFullItem(class UItem* Item) const {
	if (Item) {
		for (auto& InvItem : Items) {
			if (InvItem && InvItem->GetClass() == Item->GetClass() && !InvItem->IsStackFull()) {
				return InvItem;
			}
		}
	}
	return nullptr;
}

void UInventoryComponent::TryAddItemQuantity(UItem* Item, int32 ExpectAddQuantity) {
	if (GetOwner()) {
		if (GetOwner()->HasAuthority()) {

				int32 MaxAddQuantity = (GetRemainWeight()) / (Item->GetWeight());
				int32 PendingAddQuantity = FMath::Min(ExpectAddQuantity, MaxAddQuantity);
				int32 TotalAddedQuantity = 0;

				while (PendingAddQuantity > 0) {
					if (UItem* ExistingItem = FindNoneStackFullItem(Item)) {
						FItemAddResult ItemAddResult = ExistingItem->TryAddQuantityTillStackFull(PendingAddQuantity);
						PendingAddQuantity = ItemAddResult.RemainQuantity;
						TotalAddedQuantity += ItemAddResult.AddedQuantity;
						ExistingItem->IncrementRepKey();
						IncrementItemsRepKey();
					} else {
						AddItemQuantity(Item, 1);
						--PendingAddQuantity;
						++TotalAddedQuantity;
					}
				}
				Item->SetQuantity(Item->GetQuantity() - TotalAddedQuantity);

				Item->MarkDirtyForReplication();
			//}
		} else {
			ServerTryAddItemQuantity(Item, ExpectAddQuantity);
		}
	}
}

void UInventoryComponent::ServerTryAddItemQuantity_Implementation(UItem* Item, int32 ExpectAddQuantity) {
	TryAddItemQuantity(Item, ExpectAddQuantity);
}

int32 UInventoryComponent::ConsumeItem(class UItem* Item, const int32 Quantity){
	if (GetOwner() && GetOwner()->HasAuthority() && Item){
		const int32 RemoveQuantity = FMath::Min(Quantity, Item->GetQuantity());

		Item->SetQuantity(Item->GetQuantity() - RemoveQuantity);

		if (Item->GetQuantity() <= 0){
			RemoveItem(Item);
		}

		Item->IncrementRepKey();
		IncrementItemsRepKey();
		
		return RemoveQuantity;
	}
	return 0;
}

bool UInventoryComponent::RemoveItem(class UItem* Item){
	if (GetOwner() && GetOwner()->HasAuthority()){
		if (Item){
			Items.RemoveSingle(Item);

			IncrementItemsRepKey();
			return true;
		}
	}
	return false;
}

void UInventoryComponent::OnItemModified() {
	OnInventoryUpdated.Broadcast(); 
}

void UInventoryComponent::OnRep_Items() {
	OnInventoryUpdated.Broadcast();

	for (auto& Item : Items) {
		if (!Item->HasBindOnModified()) {
			Item->OnItemModified.AddDynamic(this, &UInventoryComponent::OnItemModified);
			Item->SetHasBindOnModified(true);
		}
	}
}

void UInventoryComponent::OnRep_InventoryItem() {
	OnInventoryUpdated.Broadcast();
}

//-- Equipment
void UInventoryComponent::Equipment_RemoveSingleEquippableItem(UEquippableItem* EquippableItem) {
	Items.RemoveSingle(EquippableItem);
	IncrementItemsRepKey();
}


UEquippableItem* UInventoryComponent::Equipment_AddNewSingleEquippableItem(UEquippableItem* EquippableItem) {
	UEquippableItem* NewEquippableItem = NewObject<UEquippableItem>(GetOwner(), EquippableItem->GetClass());
	NewEquippableItem->SetQuantity(1);
	NewEquippableItem->SetSlot(EquippableItem->GetSlot());
	NewEquippableItem->SetOwningInventory(this);

	Items.Add(NewEquippableItem);
	NewEquippableItem->IncrementRepKey();
	IncrementItemsRepKey();
	return NewEquippableItem;
}

UEquippableItem* UInventoryComponent::Equipment_FindEquippableItemBySlot(EEquippableSlot Slot) const {
	for (auto& InvItem : Items) {
		UEquippableItem* EquippableItem = Cast<UEquippableItem>(InvItem);
		if (EquippableItem) {
			if (EquippableItem->GetSlot() == Slot) {
				return EquippableItem;
			}
		}
	}
	return NULL;
}

void UInventoryComponent::Equipment_PreProcessWeaponItem(UEquippableItem* EquippableItem) {
	if (EquippableItem->GetSlot() == EEquippableSlot::EIS_Weapon) {
		for (auto slot : GetSurvivalCharacter()->GetWeaponSlots()) {
			UEquippableItem* AlreadyEquippedWeaponItem = Equipment_FindEquippableItemBySlot(slot);
			if (!AlreadyEquippedWeaponItem) {
				EquippableItem->SetSlot(slot);
				return;
			}
		}
	}
}
bool UInventoryComponent::Equipment_TryUnEquipeEquippableItem(UEquippableItem* EquippableItem) {
	if (GetSurvivalCharacter() && GetSurvivalCharacter()->GetEquipmentsInventory() == this) {
		//Equipment_PreProcessWeaponItem(EquippableItem);

		//if (EquippableItem->GetSlot() == EEquippableSlot::EIS_Weapon) {
		//	return false;
		//}

		UEquippableItem* equippedItem = Equipment_FindEquippableItemBySlot(EquippableItem->GetSlot());

		GetSurvivalCharacter()->TryAddToActiveInventory(equippedItem, GetSurvivalCharacter()->FindInventoryComponentBySlot(equippedItem->GetSlot()));
		
		if (equippedItem->GetQuantity() == 0) {
			if (equippedItem) {
				if (UInventoryItem* inventoryItem = Cast<UInventoryItem>(equippedItem)) {
					if (UInventoryComponent* UpdateInventory = GetSurvivalCharacter()->FindInventoryComponentBySlot(equippedItem->GetSlot())) {
						UpdateInventory->SetInventoryItem(NULL);
					}
				}
			}

			return true;
		}
	}

	return false;
}


bool UInventoryComponent::Equipment_TryAddEquippableItem(UEquippableItem* EquippableItem) {
	if (GetSurvivalCharacter() && GetSurvivalCharacter()->GetEquipmentsInventory() == this) {
		Equipment_PreProcessWeaponItem(EquippableItem);
		 
		if (EquippableItem->GetSlot() == EEquippableSlot::EIS_Weapon) {
			return false;
		}

		UEquippableItem* AlreadyEquippedItem = Equipment_FindEquippableItemBySlot(EquippableItem->GetSlot());

		if (!AlreadyEquippedItem) {
			UEquippableItem* NewEquippableItem = Equipment_AddNewSingleEquippableItem(EquippableItem);
			EquippableItem->SetQuantity(EquippableItem->GetQuantity() - 1);
			EquippableItem->MarkDirtyForReplication();


			if (UInventoryItem* inventoryItem = Cast<UInventoryItem>(NewEquippableItem)) {
				if (UInventoryComponent* UpdateInventory = GetSurvivalCharacter()->FindInventoryComponentBySlot(NewEquippableItem->GetSlot())) {
					inventoryItem->SetInventoryComponent(UpdateInventory);

					UpdateInventory->SetMaxWeight(inventoryItem->GetInventoryWeight());
					UpdateInventory->SetInventoryItem(inventoryItem);
				}
			}

			return true;
		}
	}

	return false;
}

bool UInventoryComponent::DropItemQuantity(UItem* Item, int32 DropQuantity) {

	if (ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(GetOwner())) {

		bool result = Item->Pre_DropFromInventoryComponent(this, DropQuantity);

		if (result) {
			UE_LOG(LogTemp, Warning, TEXT("Yes D"));
			Item->Post_DropFromInventoryComponent(this, DropQuantity);

			const int32 DroppedQuantity = ConsumeItem(Item, DropQuantity);

			Character->SpawnPickup(Item, DroppedQuantity);

			return true;
		} else {
			UE_LOG(LogTemp, Warning, TEXT("No U"));
		}
	}




	return false;
}

//--

bool UInventoryComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) {
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	if (Channel->KeyNeedsToReplicate(GetUniqueID(), GetItemsRepKey())) {
		for (auto& Item : Items) {
			if (Channel->KeyNeedsToReplicate(Item->GetUniqueID(), Item->GetRepKey())) {
				bWroteSomething |= Channel->ReplicateSubobject(Item, *Bunch, *RepFlags);
			}
		}
	}

	if (InventoryItem && Channel->KeyNeedsToReplicate(InventoryItem->GetUniqueID(), InventoryItem->GetRepKey())) {
		bWroteSomething |= Channel->ReplicateSubobject(InventoryItem, *Bunch, *RepFlags);
	}
	return bWroteSomething;
}


#undef LOCTEXT_NAMESPACE