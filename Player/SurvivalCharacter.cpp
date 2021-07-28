#include "SurvivalCharacter.h"

#include "Net/UnrealNetwork.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InteractionComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/EquipmentComponent.h"




#include "Camera/CameraComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "World/Pickup.h"

#include "Items/GearItem.h"
#include "Items/InventoryItem.h"
#include "Items/WeaponItem.h"

#include "Weapons/Weapon.h"

#include "TimerManager.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "DrawDebugHelpers.h"


#include "SurvivalGame.h"
#include "SurvivalPlayerController.h"

#include "GameFramework/DamageType.h"
#include "Weapons/SurvivalDamageBase.h"

#include "Sound/SoundCue.h"

 
ASurvivalCharacter::ASurvivalCharacter(){
	SetReplicateMovement(true);
	SetReplicates(true);

	NetCullDistanceSquared = 100000000.0f;

	bAlwaysRelevant = false;
	PrimaryActorTick.bCanEverTick = true;

	ThirdPersonSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("ThirdPersonSpringArmComponent");
	ThirdPersonSpringArmComponent->SetupAttachment(GetCapsuleComponent());
	ThirdPersonSpringArmComponent->TargetArmLength = 200.f;
	ThirdPersonSpringArmComponent->bUsePawnControlRotation = true;

	TirdPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>("TirdPersonCameraComponent");
	TirdPersonCameraComponent->SetupAttachment(ThirdPersonSpringArmComponent);

	FaceMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FaceMeshComponent"));
	EyesMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EyesMeshComponent"));
	HeadMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMeshComponent"));
	ChestArmorMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ChestArmorMeshComponent"));
	ChestMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ChestMeshComponent"));
	BackMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BackMeshComponent"));
	BeltMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BeltMeshComponent"));
	HandsMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandsMeshComponent"));
	LegsMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegsMeshComponent"));
	FeetMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FeetMeshComponent"));

	ClothesMeshComponents.Add(EEquippableSlot::EIS_Face, FaceMeshComponent);
	ClothesMeshComponents.Add(EEquippableSlot::EIS_Eyes, EyesMeshComponent);
	ClothesMeshComponents.Add(EEquippableSlot::EIS_Head, HeadMeshComponent);
	ClothesMeshComponents.Add(EEquippableSlot::EIS_ChestArmor, ChestArmorMeshComponent);
	ClothesMeshComponents.Add(EEquippableSlot::EIS_Chest, ChestMeshComponent);
	ClothesMeshComponents.Add(EEquippableSlot::EIS_Back, BackMeshComponent);
	ClothesMeshComponents.Add(EEquippableSlot::EIS_Belt, BeltMeshComponent);
	ClothesMeshComponents.Add(EEquippableSlot::EIS_Hands, HandsMeshComponent);
	ClothesMeshComponents.Add(EEquippableSlot::EIS_Legs, LegsMeshComponent);
	ClothesMeshComponents.Add(EEquippableSlot::EIS_Feet, FeetMeshComponent);

	TMapESlotSkeletalMeshes.Add(EEquippableSlot::EIS_Face, &FaceMesh);
	TMapESlotSkeletalMeshes.Add(EEquippableSlot::EIS_Eyes, &EyesMesh);
	TMapESlotSkeletalMeshes.Add(EEquippableSlot::EIS_Head, &HeadMesh);
	TMapESlotSkeletalMeshes.Add(EEquippableSlot::EIS_ChestArmor, &ChestArmorMesh);
	TMapESlotSkeletalMeshes.Add(EEquippableSlot::EIS_Chest, &ChestMesh);
	TMapESlotSkeletalMeshes.Add(EEquippableSlot::EIS_Back, &BackMesh);
	TMapESlotSkeletalMeshes.Add(EEquippableSlot::EIS_Belt, &BeltMesh);
	TMapESlotSkeletalMeshes.Add(EEquippableSlot::EIS_Hands, &HandsMesh);
	TMapESlotSkeletalMeshes.Add(EEquippableSlot::EIS_Legs, &LegsMesh);
	TMapESlotSkeletalMeshes.Add(EEquippableSlot::EIS_Feet, &FeetMesh);


	TMapESlotWeapons.Add(EEquippableSlot::EIS_PrimaryWeapon, &PrimaryWeapon);
	TMapESlotWeapons.Add(EEquippableSlot::EIS_SecondaryWeapon, &SecondaryWeapon);
	TMapESlotWeapons.Add(EEquippableSlot::EIS_TertiaryWeapon, &TertiaryWeapon);

	TMapESlotWeaponSocketName.Add(EEquippableSlot::EIS_PrimaryWeapon, FName("PrimaryWeaponSocket"));
	TMapESlotWeaponSocketName.Add(EEquippableSlot::EIS_SecondaryWeapon, FName("SecondaryWeaponSocket"));
	TMapESlotWeaponSocketName.Add(EEquippableSlot::EIS_TertiaryWeapon, FName("TertiaryWeaponSocket"));

	for (auto& mesh : ClothesMeshComponents){
		USkeletalMeshComponent* MeshComponent = mesh.Value;
		MeshComponent->SetupAttachment(GetMesh());
		MeshComponent->SetMasterPoseComponent(GetMesh());
	}
	//---


	EquipmentsInventory = CreateDefaultSubobject<UInventoryComponent>("EquipmentsInventory");
	EquipmentsInventory->SetSurvivalCharacter(this);

	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>("EquipmentComponent");

	ChestInventory = CreateDefaultSubobject<UInventoryComponent>("ChestInventory");
	LegsInventory = CreateDefaultSubobject<UInventoryComponent>("LegsInventory");
	BeltInventory = CreateDefaultSubobject<UInventoryComponent>("BeltInventory");
	BackInventory = CreateDefaultSubobject<UInventoryComponent>("BackInventory");

	EquipmentsInventory->SetSlot(EEquippableSlot::EIS_Equipments);

	ChestInventory->SetSlot(EEquippableSlot::EIS_Chest);
	BackInventory->SetSlot(EEquippableSlot::EIS_Back);
	BeltInventory->SetSlot(EEquippableSlot::EIS_Belt);
	LegsInventory->SetSlot(EEquippableSlot::EIS_Legs);
	

	

	TArrayESlotInventoryComponents.Add(EquipmentsInventory);

	TArrayESlotInventoryComponents.Add(ChestInventory);
	TArrayESlotInventoryComponents.Add(BackInventory);
	TArrayESlotInventoryComponents.Add(BeltInventory);
	TArrayESlotInventoryComponents.Add(LegsInventory);
	 

	SprintSpeed = 0.0f;
	WalkSpeed = 0.0f;

	InteractionCheckFrequency = 0.0f;
	InteractionCheckDistance = 1000.0f;

	MeleeAttackDistance = 300.0f;
	MeleeAttackDamage = 20.f;

	CurrentHealth = 100.0f;
	MaxHealth = 100.0f;

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	bHoldingRifle = false;

	WeaponSlots.Add(EEquippableSlot::EIS_PrimaryWeapon);
	WeaponSlots.Add(EEquippableSlot::EIS_SecondaryWeapon);
	WeaponSlots.Add(EEquippableSlot::EIS_TertiaryWeapon);

	CurrentHoldSlot = EEquippableSlot::EIS_PrimaryWeapon;
}

void ASurvivalCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 
	DOREPLIFETIME(ASurvivalCharacter, bSprinting);

	//DOREPLIFETIME(ASurvivalCharacter, EquippedWeapon);
	DOREPLIFETIME_CONDITION(ASurvivalCharacter, EquippedWeapon, COND_OwnerOnly);
	DOREPLIFETIME(ASurvivalCharacter, bHoldingRifle);

	DOREPLIFETIME_CONDITION(ASurvivalCharacter, CurrentHealth, COND_OwnerOnly);


	DOREPLIFETIME_CONDITION(ASurvivalCharacter, EquippedItems, COND_OwnerOnly);

	DOREPLIFETIME(ASurvivalCharacter, FaceMesh);
	DOREPLIFETIME(ASurvivalCharacter, EyesMesh);
	DOREPLIFETIME(ASurvivalCharacter, HeadMesh);
	DOREPLIFETIME(ASurvivalCharacter, ChestArmorMesh);
	DOREPLIFETIME(ASurvivalCharacter, ChestMesh);
	DOREPLIFETIME(ASurvivalCharacter, BackMesh);
	DOREPLIFETIME(ASurvivalCharacter, BeltMesh);
	DOREPLIFETIME(ASurvivalCharacter, HandsMesh);
	DOREPLIFETIME(ASurvivalCharacter, LegsMesh);
	DOREPLIFETIME(ASurvivalCharacter, FeetMesh);
}

void ASurvivalCharacter::BeginPlay(){
	Super::BeginPlay();
}

void ASurvivalCharacter::UpdateInteractable() {
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	UInteractionComponent* Interactable = DetectInteractable();

	if (Interactable) {
		if (Interactable != InteractionData.ViewedInteractionComponent) {
			if (InteractionData.ViewedInteractionComponent) {
				InteractionData.ViewedInteractionComponent->EndFocus(this);
			}
			Interactable->BeginFocus(this);
			InteractionData.ViewedInteractionComponent = Interactable;
		}
	} else {
		if (InteractionData.ViewedInteractionComponent) {
			InteractionData.ViewedInteractionComponent->EndFocus(this);
			InteractionData.ViewedInteractionComponent = nullptr;
		}
	}
}

void ASurvivalCharacter::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

	if (GetNetMode() != NM_DedicatedServer) {
		if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency) {
			UpdateInteractable();
		}
	}
}

void ASurvivalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASurvivalCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASurvivalCharacter::MoveRight);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ASurvivalCharacter::BeginInteract);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &ASurvivalCharacter::EndInteract);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASurvivalCharacter::StartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASurvivalCharacter::StopSprinting);

	PlayerInputComponent->BindAction("ToggleCrouch", IE_Pressed, this, &ASurvivalCharacter::ToggleCrouching);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ASurvivalCharacter::StartAttack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &ASurvivalCharacter::StopAttack);

	
	PlayerInputComponent->BindAction("HoldPrimaryWeapon", IE_Pressed, this, &ASurvivalCharacter::HoldPrimaryWeapon);
	PlayerInputComponent->BindAction("HoldSecondaryWeapon", IE_Pressed, this, &ASurvivalCharacter::HoldSecondaryWeapon);
	PlayerInputComponent->BindAction("HoldTertiaryWeapon", IE_Pressed, this, &ASurvivalCharacter::HoldTertiaryWeapon);

}

void ASurvivalCharacter::UnEquipUsingWeapon() {
	if (EquippedWeapon) {
		EquippedWeapon->AttachMeshToSocket(EquippedWeapon->AttachSocketName);
		EquippedWeapon = NULL;
		bHoldingRifle = false;
	}
}

void ASurvivalCharacter::HoldWeapon(EEquippableSlot Slot) {
	if (!HasAuthority()) {
		ServerHoldWeapon(Slot);
	} else {
		AWeapon** Weapon = *TMapESlotWeapons.Find(Slot);
		CurrentHoldSlot = Slot;
		UnEquipUsingWeapon();
		if (*Weapon) {
			(*Weapon)->AttachMeshToSocket(FName("RightHandWeaponSocket"));
			EquippedWeapon = (*Weapon);
			bHoldingRifle = true;
		} else {
			EquippedWeapon = (*Weapon);
		}
	}
}

void ASurvivalCharacter::ServerHoldWeapon_Implementation(EEquippableSlot Slot) {
	HoldWeapon(Slot);
}

void ASurvivalCharacter::HoldPrimaryWeapon() {
	HoldWeapon(EEquippableSlot::EIS_PrimaryWeapon);
}

void ASurvivalCharacter::HoldSecondaryWeapon() {
	HoldWeapon(EEquippableSlot::EIS_SecondaryWeapon);
}

void ASurvivalCharacter::HoldTertiaryWeapon() {
	HoldWeapon(EEquippableSlot::EIS_TertiaryWeapon);
}

void ASurvivalCharacter::StartAttack() {
	if (Role < ROLE_Authority) {
		ServerStartAttack();
	}

	if (EquippedWeapon) {
		EquippedWeapon->StartFire();
	} else {
		BeginMeleeAttack();
	}
}

void ASurvivalCharacter::ServerStartAttack_Implementation() {
	StartAttack();
}

void ASurvivalCharacter::StopAttack() {

}

void ASurvivalCharacter::NetMulticastPlayMeleeAttackMontage_Implementation() {
	if (!IsLocallyControlled()) {
		PlayAnimMontage(MeleeAttackMontage);
	}
}

void ASurvivalCharacter::NetMulticastPlayDeathMontage_Implementation() {
	//if (!IsLocallyControlled()) {
		PlayAnimMontage(BeShotMontage);
	//}
}

void ASurvivalCharacter::BeginMeleeAttack() {

	if (GetWorld()->TimeSince(LastMeleeAttackTime) > MeleeAttackMontage->GetPlayLength()) {

		if (HasAuthority()) {
			NetMulticastPlayMeleeAttackMontage();
			PlayAnimMontage(MeleeAttackMontage);
		} else {
			PlayAnimMontage(MeleeAttackMontage);
		}

		FHitResult MeleeHit;
		FCollisionShape Shape = FCollisionShape::MakeSphere(15.f);

		FVector StartTrace = TirdPersonCameraComponent->GetComponentLocation();
		FVector EndTrace = (TirdPersonCameraComponent->GetComponentRotation().Vector() * MeleeAttackDistance) + StartTrace;

		FCollisionQueryParams QueryParams = FCollisionQueryParams("MeleeSweep", false, this);

		//PlayAnimMontage(MeleeAttackMontage);

		if (GetWorld()->SweepSingleByChannel(MeleeHit, StartTrace, EndTrace, FQuat(), COLLISION_WEAPON, Shape, QueryParams)) {
			if (ASurvivalCharacter* HitPlayer = Cast<ASurvivalCharacter>(MeleeHit.GetActor())) {
				if (ASurvivalPlayerController* PC = Cast<ASurvivalPlayerController>(GetController())) {
					if (HasAuthority()) {
						//MulticastPlayMeleeFX();
						UGameplayStatics::ApplyPointDamage(MeleeHit.GetActor(), MeleeAttackDamage,
														   (MeleeHit.TraceStart - MeleeHit.TraceEnd).GetSafeNormal(), MeleeHit,
														   GetController(), this, UMeleeDamage::StaticClass());
						PC->ClientShotHitConfirmed();
					} else {
						PC->ClientShotHitConfirmed();
					}
				}
			}
		}

		//ServerProcessMeleeHit(Hit);

		LastMeleeAttackTime = GetWorld()->GetTimeSeconds();
	}
}

void ASurvivalCharacter::ToggleCrouching() {
	if (!GetCharacterMovement()->IsCrouching()) {
		Crouch();
	} else {
		UnCrouch();
	}
}

void ASurvivalCharacter::MoveForward(float Value){
	if (Value != 0.f){
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ASurvivalCharacter::MoveRight(float Value){
	if (Value != 0.f){
		AddMovementInput(GetActorRightVector(), Value);
	}
}

bool ASurvivalCharacter::CanSprint() const{
	return true;
}

void ASurvivalCharacter::StartSprinting(){
	SetSprinting(true);
}

void ASurvivalCharacter::StopSprinting(){
	SetSprinting(false);
}

void ASurvivalCharacter::SetSprinting(const bool bNewSprinting){
	if ((bNewSprinting && !CanSprint()) || bNewSprinting == bSprinting){
		return;
	}

	if (Role < ROLE_Authority){
		ServerSetSprinting(bNewSprinting);
	}

	bSprinting = bNewSprinting;

	GetCharacterMovement()->MaxWalkSpeed = bSprinting ? SprintSpeed : WalkSpeed;
}

void ASurvivalCharacter::ServerSetSprinting_Implementation(const bool bNewSprinting){
	SetSprinting(bNewSprinting);
}

//--- Interaction

// Client Domain
void ASurvivalCharacter::BeginInteract() {
	if (UInteractionComponent* Interactable = GetInteractable()) {
		Interactable->BeginInteract(this);

		if (FMath::IsNearlyZero(Interactable->GetInteractionTime())) {
			Interact();
		}else {
			GetWorldTimerManager().SetTimer(TimerHandle_Interact, this, &ASurvivalCharacter::Interact, Interactable->GetInteractionTime(), false);
		}
	}
}

void ASurvivalCharacter::EndInteract() {
	if (GetWorldTimerManager().IsTimerActive(TimerHandle_Interact)) {
		GetWorldTimerManager().ClearTimer(TimerHandle_Interact);
	}

	if (UInteractionComponent* Interactable = GetInteractable()) {
		Interactable->EndInteract(this);
	}
}

void ASurvivalCharacter::ServerInteract_Implementation() {
	Interact();
}

void ASurvivalCharacter::Interact() {
	
	if (!HasAuthority()) {
		ServerInteract();
		return;
	} else {

		//UE_LOG(LogTemp, Warning, TEXT("==>%d"), SlotSkeletalMeshes.Num());

		UInteractionComponent* Interactable = DetectInteractable();

		if (Interactable) {
			Interactable->Interact(this);
		}
	}
}

bool ASurvivalCharacter::IsInteracting() const{
	return GetWorldTimerManager().IsTimerActive(TimerHandle_Interact);
}

float ASurvivalCharacter::GetRemainingInteractTime() const{
	return GetWorldTimerManager().GetTimerRemaining(TimerHandle_Interact);
}

UInteractionComponent* ASurvivalCharacter::DetectInteractable() {
	if (GetController() == nullptr) {
		return nullptr;
	}

	FVector EyesLoc;
	FRotator EyesRot;

	GetController()->GetPlayerViewPoint(EyesLoc, EyesRot);

	FVector TraceStart = EyesLoc;
	FVector TraceEnd = (EyesRot.Vector() * InteractionCheckDistance) + TraceStart;
	FHitResult TraceHit;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams)) {
		if (TraceHit.GetActor()) {

			if (UInteractionComponent* InteractionComponent = Cast<UInteractionComponent>(TraceHit.GetActor()->GetComponentByClass(UInteractionComponent::StaticClass()))) {
				if (InteractionComponent->IsCustomActive()) {
					return InteractionComponent;
				}
			}
		}
	}

	return nullptr;
}



void ASurvivalCharacter::Player_UseItem(class UItem* Item) {
	if (!Item) {
		return;
	}

	if (HasAuthority()) {
		Item->Use(this);
	} else {
		Server_Player_UseItem(Item);
	}
}

void ASurvivalCharacter::Server_Player_UseItem_Implementation(class UItem* Item) {
	Player_UseItem(Item);
}

UEquippableItem* ASurvivalCharacter::FindFromEquippedItems(UItem* TrargetItem) {
	for (auto& Item : EquippedItems) {
		if (Item->GetClass() == TrargetItem->GetClass() && Item->GetUniqueID() == TrargetItem->GetUniqueID()) {
			return Item;
		}
	}
	return NULL;
}

void ASurvivalCharacter::SpawnPickup(UItem* Item, int32 DropQuantity) {
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

	ensure(PickupClass);

	APickup* Pickup = GetWorld()->SpawnActor<APickup>(PickupClass, SpawnTransform, SpawnParams);
	Pickup->InitializePickup(Item->GetClass(), DropQuantity);
}

void ASurvivalCharacter::TryFindEquippableWeaponSlot(UEquippableItem* Item) {
	for (auto slot : WeaponSlots) {
		UEquippableItem* AlreadyEquippedItem = FindEquippedItemBySlot(slot);
		if (!AlreadyEquippedItem) {
			Item->SetSlot(slot);
			Item->MarkDirtyForReplication();
			return;
		}
	}
}


void ASurvivalCharacter::OnRep_FaceMesh() {
	HeadMeshComponent->SetSkeletalMesh(FaceMesh);
}
void ASurvivalCharacter::OnRep_EyesMesh() {
	EyesMeshComponent->SetSkeletalMesh(EyesMesh);
}
void ASurvivalCharacter::OnRep_HeadMesh() {
	HeadMeshComponent->SetSkeletalMesh(HeadMesh);
}
void ASurvivalCharacter::OnRep_ChestArmorMesh() {
	ChestArmorMeshComponent->SetSkeletalMesh(ChestArmorMesh);
}
void ASurvivalCharacter::OnRep_ChestMesh() {
	ChestMeshComponent->SetSkeletalMesh(ChestMesh);
}
void ASurvivalCharacter::OnRep_BackMesh() {
	BackMeshComponent->SetSkeletalMesh(BackMesh);
}
void ASurvivalCharacter::OnRep_BeltMesh() {
	BeltMeshComponent->SetSkeletalMesh(BeltMesh);
}
void ASurvivalCharacter::OnRep_HandsMesh() {
	HandsMeshComponent->SetSkeletalMesh(HandsMesh);
}
void ASurvivalCharacter::OnRep_LegsMesh() {
	LegsMeshComponent->SetSkeletalMesh(LegsMesh);
}
void ASurvivalCharacter::OnRep_FeetMesh() {
	FeetMeshComponent->SetSkeletalMesh(FeetMesh);
}

void ASurvivalCharacter::EquipGear(class UGearItem* Gear) {
	if (HasAuthority()) {

		USkeletalMesh** SkeletalMesh = *TMapESlotSkeletalMeshes.Find(Gear->GetSlot());
		if (SkeletalMesh) {
			*SkeletalMesh = Gear->GetSkeletalMesh();
		}
		
		//if (GetNetMode() != NM_DedicatedServer) {
		//	OnRep_FaceMesh();
		//	OnRep_EyesMesh();
		//	OnRep_HeadMesh();
		//	OnRep_ChestArmorMesh();
		//	OnRep_ChestMesh();
		//	OnRep_BackMesh();
		//	OnRep_BeltMesh();
		//	OnRep_HandsMesh();
		//	OnRep_LegsMesh();
		//	OnRep_FeetMesh();
		//}
	}

}

void ASurvivalCharacter::UnEquipGear(const EEquippableSlot Slot) {
	if (HasAuthority()) {
		USkeletalMesh** SkeletalMesh = *TMapESlotSkeletalMeshes.Find(Slot);
		if (SkeletalMesh) {
			*SkeletalMesh = NULL;
		}
	}
}

bool ASurvivalCharacter::EquipWeapon(class UWeaponItem* WeaponItem) {
	if (HasAuthority()) {
		AWeapon** Weapon = *TMapESlotWeapons.Find(WeaponItem->GetSlot());
		*Weapon = WeaponItem->Spawn(this);


		if (Weapon) {
			(*Weapon)->AttachSocketName = *TMapESlotWeaponSocketName.Find(WeaponItem->GetSlot());

			if (CurrentHoldSlot == WeaponItem->GetSlot()) {
				(*Weapon)->AttachMeshToSocket(FName("RightHandWeaponSocket"));
				EquippedWeapon = (*Weapon);
				bHoldingRifle = true;
			} else {
				(*Weapon)->AttachMeshToSocket((*Weapon)->AttachSocketName);
			}
		}
	}
	return false;
}

void ASurvivalCharacter::UnEquipWeapon(UWeaponItem* WeaponItem) {
	if (HasAuthority()) {
		AWeapon** Weapon = *TMapESlotWeapons.Find(WeaponItem->GetSlot());

		if (Weapon) {
			(*Weapon)->Destroy();
		}
		
		if (*Weapon == EquippedWeapon) {
			bHoldingRifle = false;
			EquippedWeapon = NULL;
		}
		*Weapon = NULL;

	}
}

float ASurvivalCharacter::ModifyHealth(const float Delta) {
	const float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp<float>(CurrentHealth + Delta, 0.f, MaxHealth);
	return CurrentHealth - OldHealth;
}

void ASurvivalCharacter::OnRep_Health(float OldHealth) {
	OnHealthModified(CurrentHealth - OldHealth);
}

float ASurvivalCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	//if (!IsAlive()) {
	//	return 0.f;
	//}

	const float DamageDealt = ModifyHealth(-Damage);
	NetMulticastPlayDeathMontage();

	return DamageDealt;
}

void ASurvivalCharacter::OnRep_EquippedItems() {
	OnEquippedItemsChanged.Broadcast(EEquippableSlot::EIS_PrimaryWeapon, NULL);
}

UEquippableItem* ASurvivalCharacter::FindEquippedItemBySlot(EEquippableSlot Slot) {
	for (auto& Item : EquippedItems) {
		if (Item->GetSlot() == Slot) {
			return Item;
		}
	}
	return NULL;
}



bool ASurvivalCharacter::HaveEnoughWeightFromActiveInventory(int32 Weight, UInventoryComponent* BesidesInventory) {
	for (auto& InventoryComponent : TArrayESlotInventoryComponents) {
		if (!GetEquipmentComponent()->FindEquippableItemBySlot(InventoryComponent->GetSlot()) || InventoryComponent == BesidesInventory ) {
			continue;
		}

		if (InventoryComponent->GetRemainWeight() >= Weight) {
			return true;
		}
	}
	return false;
}

void ASurvivalCharacter::TryAddToActiveInventory(UItem* Item, UInventoryComponent* BesidesInventory) {
	for (auto& InventoryComponent : TArrayESlotInventoryComponents) {
		if (!GetEquipmentComponent()->FindEquippableItemBySlot(InventoryComponent->GetSlot()) || InventoryComponent == BesidesInventory) {
			continue;
		}

		if (Item->GetQuantity() > 0) {
			InventoryComponent->TryAddItemQuantity(Item, Item->GetQuantity());
		}
	}
}

void ASurvivalCharacter::ClientUnEquipItem(UEquippableItem* Item) {
	if (HasAuthority()) {
		Item->UnEquip(this);
	} else {
		ServerClientUnEquipItem(Item);
	}
}

void ASurvivalCharacter::ServerClientUnEquipItem_Implementation(UEquippableItem* Item) {
	ClientUnEquipItem(Item);
}

void ASurvivalCharacter::Player_TakeItem(UItem* Item) {
	if (HasAuthority()) {
		UEquippableItem* EquippableItem = Cast<UEquippableItem>(Item);
		if (EquippableItem) {
			bool result = GetEquipmentComponent()->EquipEquippableItem(EquippableItem);
	
			if (!result) {
				TryAddToActiveInventory(Item);
			}
		} else {
			TryAddToActiveInventory(Item);
		}
	} else {
		Server_Player_TakeItem(Item);
	}
}

void ASurvivalCharacter::Server_Player_TakeItem_Implementation(UItem* Item) {
	Player_TakeItem(Item);
}

UInventoryComponent* ASurvivalCharacter::FindInventoryComponentBySlot(EEquippableSlot slot) {
	for (auto& InventoryComponent : TArrayESlotInventoryComponents) {
		if (InventoryComponent->GetSlot() == slot) {
			return InventoryComponent;
		}
	}
	return NULL;
}

void ASurvivalCharacter::TryAddItemQuantityToInventory(UItem* Item, int32 ExpectAddQuantity, UInventoryComponent* DestinationInventory) {
	if (Item && ExpectAddQuantity > 0 && DestinationInventory) {
		if (HasAuthority()) {
			bool res = Item->Pre_AddItemQuantityToInventory(DestinationInventory, ExpectAddQuantity);

			if (res) {
				UE_LOG(LogTemp, Warning, TEXT("Yes"));
				Item->Post_AddItemQuantityToInventory(DestinationInventory, ExpectAddQuantity);
				DestinationInventory->TryAddItemQuantity(Item, ExpectAddQuantity);
			} else {
				UE_LOG(LogTemp, Warning, TEXT("No"));
			}
		} else {
			Server_TryAddItemQuantityToInventory(Item, ExpectAddQuantity, DestinationInventory);
		}
	}
}

void ASurvivalCharacter::Server_TryAddItemQuantityToInventory_Implementation(UItem* Item, int32 ExpectAddQuantity, UInventoryComponent* DestinationInventory) {
	TryAddItemQuantityToInventory(Item, ExpectAddQuantity, DestinationInventory);
}

//---

bool ASurvivalCharacter::EquipItem(UEquippableItem* EquippableItem, EEquippableSlot Slot) {
	Player_EquipItem(EquippableItem, Slot);
	return false;
}

bool ASurvivalCharacter::UnEquipItem(UEquippableItem* EquippableItem) {
	Player_UnEquipItem(EquippableItem);
	return false;
}

bool ASurvivalCharacter::DropItemFromEquipmentComponent(UEquippableItem* EquippableItem) {
	Player_DropItemFromEquipmentComponent(EquippableItem);
	return false;
}

bool ASurvivalCharacter::Player_DropItemFromEquipmentComponent(UEquippableItem* EquippableItem) {
	if (!EquippableItem) {
		return false;
	}

	if (HasAuthority()) {
		GetEquipmentComponent()->DropEquippableItem(EquippableItem);
	} else {
		Server_Player_DropItemFromEquipmentComponent(EquippableItem);
	}
	return false;
}

void ASurvivalCharacter::Server_Player_DropItemFromEquipmentComponent_Implementation(UEquippableItem* EquippableItem) {
	Player_DropItemFromEquipmentComponent(EquippableItem);
}

bool ASurvivalCharacter::Player_EquipItem(UEquippableItem* EquippableItem, EEquippableSlot TargetSlot) {
	if (!EquippableItem) {
		return false;
	}

	if (HasAuthority()) {
		GetEquipmentComponent()->EquipEquippableItem(EquippableItem, TargetSlot);
	} else {
		Server_Player_EquipItem(EquippableItem, TargetSlot);
	}
	return false;
}

void ASurvivalCharacter::Server_Player_EquipItem_Implementation(UEquippableItem* EquippableItem, EEquippableSlot TargetSlot) {
	Player_EquipItem(EquippableItem, TargetSlot);
}

bool ASurvivalCharacter::Player_UnEquipItem(UEquippableItem* EquippableItem) {
	if (!EquippableItem) {
		return false;
	}

	if (HasAuthority()) {
		GetEquipmentComponent()->UnEquipEquippableItem(EquippableItem);
	} else {
		Server_Player_UnEquipItem(EquippableItem);
	}
	return false;
}

void ASurvivalCharacter::Server_Player_UnEquipItem_Implementation(UEquippableItem* EquippableItem) {
	Player_UnEquipItem(EquippableItem);
}


void ASurvivalCharacter::Player_DropItemFromInventoryComponent(UItem* Item, const int32 Quantity, UInventoryComponent* SourceInventory) {
	if (!Item || !(Quantity > 0) || !SourceInventory) {
		return;
	}

	if (HasAuthority()) {
		SourceInventory->DropItemQuantity(Item, Quantity);
	} else {
		Server_Player_DropItemFromInventoryComponent(Item, Quantity, SourceInventory);
	}

	//if (HasAuthority()) {
	//	if (SourceInventory == GetEquipmentsInventory()) {
	//		if (UGearItem* gearItem = Cast<UGearItem>(Item)) {
	//			if (UInventoryItem* inventoryItem = Cast<UInventoryItem>(gearItem)) {
	//				if (!inventoryItem->GetInventoryComponent()->IsEmpty()) {
	//					return;
	//				}
	//				inventoryItem->GetInventoryComponent()->SetInventoryItem(NULL);
	//			}
	//			UnEquipGear(gearItem->GetSlot());
	//		}

	//		if (UWeaponItem* weaponItem = Cast<UWeaponItem>(Item)) {
	//			UnEquipWeapon(weaponItem);
	//		}
	//	}

	//	const int32 DroppedQuantity = SourceInventory->ConsumeItem(Item, Quantity);

	//	SpawnPickup(Item, DroppedQuantity);
	//} else {
	//	Server_Player_DropItemFromInventoryComponent(Item, Quantity, SourceInventory);
	//}
}

void ASurvivalCharacter::Server_Player_DropItemFromInventoryComponent_Implementation(class UItem* Item, const int32 Quantity, UInventoryComponent* SourceInventory) {
	Player_DropItemFromInventoryComponent(Item, Quantity, SourceInventory);
}