#include "EquipmentComponent.h"
#include "Player/SurvivalCharacter.h"
#include "Items/InventoryItem.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"


UEquipmentComponent::UEquipmentComponent() {
	SetIsReplicated(true);
}

void UEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UEquipmentComponent, Items, COND_OwnerOnly);
}


UEquippableItem* UEquipmentComponent::FindEquippableItemBySlot(EEquippableSlot Slot) const {
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

void UEquipmentComponent::PreProcessWeaponItem(UEquippableItem* EquippableItem) {
	if (EquippableItem->GetSlot() == EEquippableSlot::EIS_Weapon) {
		ASurvivalCharacter* character = Cast<ASurvivalCharacter>(GetOwner());
		for (auto slot : character->GetWeaponSlots()) {
			UEquippableItem* AlreadyEquippedWeaponItem = FindEquippableItemBySlot(slot);
			if (!AlreadyEquippedWeaponItem) {
				EquippableItem->SetSlot(slot);
				return;
			}
		}
	}
}

UEquippableItem* UEquipmentComponent::AddNewSingleEquippableItem(UEquippableItem* EquippableItem) {
	UEquippableItem* NewEquippableItem = NewObject<UEquippableItem>(GetOwner(), EquippableItem->GetClass());
	NewEquippableItem->SetQuantity(1);
	NewEquippableItem->SetSlot(EquippableItem->GetSlot());
	NewEquippableItem->SetOwningEquipment(this);

	Items.Add(NewEquippableItem);

	return NewEquippableItem;
}

bool UEquipmentComponent::EquipEquippableItem(UEquippableItem* EquippableItem, EEquippableSlot TargetSlot) {
	if (ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(GetOwner())) {

		bool result = EquippableItem->Pre_EquipToEquipmentComponent(this, TargetSlot);

		if (result) {
			UE_LOG(LogTemp, Warning, TEXT("Yes E"));
			UEquippableItem* NewEquippableItem = AddNewSingleEquippableItem(EquippableItem);
			NewEquippableItem->MarkDirtyForReplication();

			EquippableItem->SetQuantity(EquippableItem->GetQuantity() - 1);
			EquippableItem->MarkDirtyForReplication();

			NewEquippableItem->Post_EquipToEquipmentComponent(this);

			return true;
		} else {
			UE_LOG(LogTemp, Warning, TEXT("No E"));
		}
	}
	return false;
}

bool UEquipmentComponent::UnEquipEquippableItem(UEquippableItem* EquippableItem) {
	if (ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(GetOwner())) {

		bool result = EquippableItem->Pre_UnEquipFromEquipmentComponent(this);

		if (result) {
			UE_LOG(LogTemp, Warning, TEXT("Yes U"));
			
			Character->TryAddToActiveInventory(EquippableItem, Character->FindInventoryComponentBySlot(EquippableItem->GetSlot()));
			

			EquippableItem->Post_UnEquipFromEquipmentComponent(this);
			RemoveItem(EquippableItem);
			return true;
		} else {
			UE_LOG(LogTemp, Warning, TEXT("No U"));
		}
	}
	return false;
}

bool UEquipmentComponent::DropEquippableItem(UEquippableItem* EquippableItem) {
	if (ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(GetOwner())) {

		bool result = EquippableItem->Pre_DropFromEquipmentComponent(this);

		if (result) {
			UE_LOG(LogTemp, Warning, TEXT("Yes D"));
			EquippableItem->Post_DropFromEquipmentComponent(this);
			Character->SpawnPickup(EquippableItem, EquippableItem->GetQuantity());
			RemoveItem(EquippableItem);
		} else { 
			UE_LOG(LogTemp, Warning, TEXT("No D"));
		}
	}
	return false;
}


bool UEquipmentComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) {
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	if (Channel->KeyNeedsToReplicate(GetUniqueID(), GetEquippableItemsRepKey())) {
		for (auto& Item : Items) {
			if (Channel->KeyNeedsToReplicate(Item->GetUniqueID(), Item->GetRepKey())) {
				bWroteSomething |= Channel->ReplicateSubobject(Item, *Bunch, *RepFlags);
			}
		}
	}

	return bWroteSomething;
}

void UEquipmentComponent::OnRep_EquippableItems() {
	OnEquipmentUpdated.Broadcast();

	UE_LOG(LogTemp, Warning, TEXT("OnRep_EquippableItems"));
}

bool UEquipmentComponent::RemoveItem(UItem* Item) {
	if (GetOwner() && GetOwner()->HasAuthority()) {
		if (Item) {
			Items.RemoveSingle(Item);

			IncrementEquippableItemsRepKey();
			return true;
		}
	}
	return false;
}