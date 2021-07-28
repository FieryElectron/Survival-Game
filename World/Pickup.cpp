#include "Pickup.h"
#include "Items/Item.h"
#include "Net/UnrealNetwork.h"
#include "Player/SurvivalCharacter.h"
#include "Player/SurvivalPlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InteractionComponent.h"
#include "Components/InventoryComponent.h"
#include "Engine/ActorChannel.h"


APickup::APickup(){
	bNetLoadOnClient = false;
	SetReplicates(true);

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	SetRootComponent(PickupMesh);

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>("PickupInteractionComponent");
	InteractionComponent->SetInteractionTime(0.0f);
	InteractionComponent->SetInteractionDistance(200.0f);
	InteractionComponent->SetInteractableNameText(FText::FromString("Pickup"));
	InteractionComponent->SetInteractableActionText(FText::FromString("Take"));

	InteractionComponent->SetupAttachment(PickupMesh);

	if (HasAuthority()) {
		InteractionComponent->OnInteract.AddDynamic(this, &APickup::OnTakePickup);
	}
}

void APickup::BeginPlay() {
	Super::BeginPlay();
}

void APickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickup, Item);
}

bool APickup::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) {
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	if (Item && Channel->KeyNeedsToReplicate(Item->GetUniqueID(), Item->GetRepKey())) {
		bWroteSomething |= Channel->ReplicateSubobject(Item, *Bunch, *RepFlags);
	}
	return bWroteSomething;
}

void APickup::InitializePickup(const TSubclassOf<class UItem> ItemClass, const int32 Quantity){
	if (HasAuthority() && ItemClass && Quantity > 0){
		Item = NewObject<UItem>(this, ItemClass);
		Item->SetQuantity(Quantity);
		Item->SetOwningPickup(this);
		
		if (GetNetMode() != NM_DedicatedServer) {
			OnRep_Item();
		}

		Item->IncrementRepKey();
	}
}

void APickup::UpdateStaticMesh() {
	PickupMesh->SetMobility(EComponentMobility::Movable);
	PickupMesh->SetStaticMesh(Item->GetPickupMesh());
}

void APickup::UpdateInteractableInfo() {
	InteractionComponent->SetInteractableNameText(FText::FromString(FString::FromInt(Item->GetQuantity())));
	InteractionComponent->SetInteractableActionText(Item->GetUseActionText());
}

void APickup::OnItemModified() {
	UpdateStaticMesh();
	UpdateInteractableInfo();

	if (InteractionComponent) {
		InteractionComponent->UpdateUserWidget();
	}
}

void APickup::OnRep_Item(){
	if (Item){
		OnItemModified();

		//Clients bind to this delegate in order to refresh the interaction widget if item quantity changes
		if (!Item->HasBindOnModified()) {
			Item->OnItemModified.AddDynamic(this, &APickup::OnItemModified);
			Item->SetHasBindOnModified(true);
		}
	}
}



void APickup::OnTakePickup(class ASurvivalCharacter* Taker){
	
	if (!Taker){
		UE_LOG(LogTemp, Warning, TEXT("Pickup was taken but player was not valid. "));
		return;
	}

	if (HasAuthority() && !IsPendingKillPending() && Item){
		Taker->Player_TakeItem(Item);

	

		//if (GetNetMode() != NM_DedicatedServer) {
		//	OnRep_Item();
		//	EquipmentsInventory->OnInventoryUpdated.Broadcast();
		//}
		//Item->IncrementRepKey();


		/*InteractionComponent->SetIsCustomActive(false);*/
		if (Item->GetQuantity() <= 0) {
			Destroy();
		}
		//----------------------------------------
		//if (UInventoryComponent* EquipmentsInventory = Taker->GetEquipmentsInventory()){
		//	EquipmentsInventory->TryAddItemQuantity(Item, Item->GetQuantity());

		//	//if (GetNetMode() != NM_DedicatedServer) {
		//	//	OnRep_Item();
		//	//	EquipmentsInventory->OnInventoryUpdated.Broadcast();
		//	//}
		//	Item->IncrementRepKey();
		//
		//	/*InteractionComponent->SetIsCustomActive(false);*/
		//	if (Item->GetQuantity() <= 0) {
		//		Destroy();
		//	}
		//}
	}
}

#if WITH_EDITOR
void APickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	//If a new pickup is selected in the property editor, change the mesh to reflect the new item being selected
	if (PropertyName == GET_MEMBER_NAME_CHECKED(APickup, ItemTemplate)) {
		if (ItemTemplate) {
			PickupMesh->SetStaticMesh(ItemTemplate->GetPickupMesh());
		}
	}
}
#endif


