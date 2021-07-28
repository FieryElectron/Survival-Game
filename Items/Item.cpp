#include "Item.h"
#include "Components/InventoryComponent.h"
#include "Components/EquipmentComponent.h"
#include "World/Pickup.h"
#include "Net/UnrealNetwork.h"

#define LOCTEXT_NAMESPACE "Item"

UItem::UItem() {
	DisplayName = LOCTEXT("ItemName", "Item");
	UseActionText = LOCTEXT("ItemUseActionText", "Use");
	Weight = 0.0f;
	SetStackable(true);
	SetQuantity(1);
	Weight = 1;
	MaxStackSize = 1;
	RepKey = 0;
	bHasBindOnModified = false;
}

FItemAddResult UItem::TryAddQuantityTillStackFull(const int32 ExpectAddQuantity) {
	int32 BeforeAddQuantity = GetQuantity();
	
	SetQuantity(FMath::Min(GetQuantity() + ExpectAddQuantity, MaxStackSize));

	int32 AddedQuantity = GetQuantity() - BeforeAddQuantity;
	int32 RemainQuantity = ExpectAddQuantity - AddedQuantity;

	return ItemAddResultConstructor(AddedQuantity, RemainQuantity);
}


void UItem::Use(class ASurvivalCharacter* Character) {

}

bool UItem::Pre_DropFromInventoryComponent(UInventoryComponent* SourceInventory, int32 DropQuantity) {
	if (!SourceInventory || !(DropQuantity > 0)) {
		return false;
	}

	UItem* FoundItem =  SourceInventory->FindItemByUniqueID(this);

	if (!FoundItem) {
		return false;
	}

	if (DropQuantity > GetQuantity()) {
		return false;
	}
	
	return true;
}

bool UItem::Post_DropFromInventoryComponent(UInventoryComponent* SourceInventory, int32 DropQuantity) {
	return true;
}

bool UItem::Pre_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity) {
	UE_LOG(LogTemp, Warning, TEXT("UItem::TryAddItemQuantityToInventory"));

	if (!DestinationInventory || !(ExpectAddQuantity > 0)) {
		return false;
	}

	int TrueNum = 0;

	UEquipmentComponent* SourceEquipment = GetOwningEquipment();
	UInventoryComponent* SourceInventory = GetOwningInventory();
	APickup* SourcePickup = GetOwningPickup();

	if (SourceEquipment) {
		++TrueNum;
	}

	if (SourceInventory) {
		++TrueNum;
	}

	if (SourcePickup) {
		++TrueNum;
	}

	if (TrueNum != 1) {
		return false;
	}

	if (SourceInventory) {
		if (SourceInventory == DestinationInventory) {
			return false;
		}
	} else if (SourcePickup) {
		if (SourcePickup->IsPendingKillPending()) {
			return false;
		}
	} else if (SourceEquipment) {

	} else {
		UE_LOG(LogTemp, Warning, TEXT("Not SourceInventory Not SourcePickup ??????? plz modify!!!"));
		return false;
	}

	//if (DestinationInventory->GetRemainWeight() < (GetWeight() * GetQuantity()) ) {
	//	return false;
	//}

	return true;
}

bool UItem::Post_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity) {
	UE_LOG(LogTemp, Warning, TEXT("UItem::Post_AddItemQuantityToInventory"));
	return true;
}

void UItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME(UItem, Quantity); 
	DOREPLIFETIME_CONDITION(UItem, OwningPickup, COND_OwnerOnly);
	
	//DOREPLIFETIME_CONDITION(UItem, Quantity, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UItem, OwningInventory, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UItem, OwningEquipment, COND_OwnerOnly);
}

void UItem::MarkDirtyForReplication() {
	if (GetOwningEquipment()) {
		if (GetQuantity() <= 0) {
			GetOwningEquipment()->RemoveItem(this);
		} else {
			IncrementRepKey();
			GetOwningEquipment()->IncrementEquippableItemsRepKey();
		}
	}

	if (GetOwningInventory()) {
		if (GetQuantity() <= 0) {
			GetOwningInventory()->RemoveItem(this);
		} else {
			IncrementRepKey();
			GetOwningInventory()->IncrementItemsRepKey();
		}
	}

	if (GetOwningPickup()) {
		if (GetQuantity() <= 0) {
			GetOwningPickup()->Destroy();
		} else {
			IncrementRepKey();
		}
	}

}

void UItem::OnRep_Quantity(){
	OnItemModified.Broadcast();
}

bool UItem::IsSupportedForNetworking() const {
	return true;
}

#if WITH_EDITOR
void UItem::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent){
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	//UPROPERTY clamping doesn't support using a variable to clamp so we do in here instead
	//if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(UItem, Quantity)){
	//	Quantity = FMath::Clamp(Quantity, 1, bStackable ? MaxStackSize : 1);
	//}else if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(UItem, bStackable)){
	//	if (!bStackable){
	//		Quantity = 1;
	//	}
	//}
}
#endif

#undef LOCTEXT_NAMESPACE