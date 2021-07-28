#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Items/EquippableItem.h"
#include "SurvivalCharacter.generated.h"

class UAudioComponent;
class USoundCue;
class UWeaponItem;
class USkeletalMeshComponent;
class USkeletalMesh;
class UItem;
class UInventoryComponent;
class UEquipmentComponent;

USTRUCT()
struct FInteractionData{
	GENERATED_BODY()

	FInteractionData(){
		ViewedInteractionComponent = nullptr;
		LastInteractionCheckTime = 0.f;
	}

	UPROPERTY()
	class UInteractionComponent* ViewedInteractionComponent;

	UPROPERTY()
	float LastInteractionCheckTime;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquippedItemsChanged, const EEquippableSlot, Slot, const UEquippableItem*, Item);

UCLASS()
class SURVIVALGAME_API ASurvivalCharacter : public ACharacter
{
	GENERATED_BODY()
		 
public:
	ASurvivalCharacter();

	//-- SkeletalMeshComponent
	UPROPERTY(EditAnywhere, Category = "SkeletalMeshComponent")
	USkeletalMeshComponent* FaceMeshComponent;

	UPROPERTY(EditAnywhere, Category = "SkeletalMeshComponent")
	USkeletalMeshComponent* EyesMeshComponent;

	UPROPERTY(EditAnywhere, Category = "SkeletalMeshComponent")
	USkeletalMeshComponent* HeadMeshComponent;

	UPROPERTY(EditAnywhere, Category = "SkeletalMeshComponent")
	USkeletalMeshComponent* ChestArmorMeshComponent;

	UPROPERTY(EditAnywhere, Category = "SkeletalMeshComponent")
	USkeletalMeshComponent* ChestMeshComponent;

	UPROPERTY(EditAnywhere, Category = "SkeletalMeshComponent")
	USkeletalMeshComponent* BackMeshComponent;

	UPROPERTY(EditAnywhere, Category = "SkeletalMeshComponent")
	USkeletalMeshComponent* BeltMeshComponent;

	UPROPERTY(EditAnywhere, Category = "SkeletalMeshComponent")
	USkeletalMeshComponent* HandsMeshComponent;

	UPROPERTY(EditAnywhere, Category = "SkeletalMeshComponent")
	USkeletalMeshComponent* LegsMeshComponent;

	UPROPERTY(EditAnywhere, Category = "SkeletalMeshComponent")
	USkeletalMeshComponent* FeetMeshComponent;

	//SkeletalMeshes

	UPROPERTY(ReplicatedUsing = OnRep_FaceMesh, EditAnywhere, Category = "SkeletalMesh")
	USkeletalMesh* FaceMesh;

	UPROPERTY(ReplicatedUsing = OnRep_EyesMesh, EditAnywhere, Category = "SkeletalMesh")
	USkeletalMesh* EyesMesh;

	UPROPERTY(ReplicatedUsing = OnRep_HeadMesh, EditAnywhere, Category = "SkeletalMesh")
	USkeletalMesh* HeadMesh;

	UPROPERTY(ReplicatedUsing = OnRep_ChestArmorMesh, EditAnywhere, Category = "SkeletalMesh")
	USkeletalMesh* ChestArmorMesh;

	UPROPERTY(ReplicatedUsing = OnRep_ChestMesh, EditAnywhere, Category = "SkeletalMesh")
	USkeletalMesh* ChestMesh;

	UPROPERTY(ReplicatedUsing = OnRep_BackMesh, EditAnywhere, Category = "SkeletalMesh")
	USkeletalMesh* BackMesh;

	UPROPERTY(ReplicatedUsing = OnRep_BeltMesh, EditAnywhere, Category = "SkeletalMesh")
	USkeletalMesh* BeltMesh;

	UPROPERTY(ReplicatedUsing = OnRep_HandsMesh, EditAnywhere, Category = "SkeletalMesh")
	USkeletalMesh* HandsMesh;

	UPROPERTY(ReplicatedUsing = OnRep_LegsMesh, EditAnywhere, Category = "SkeletalMesh")
	USkeletalMesh* LegsMesh;

	UPROPERTY(ReplicatedUsing = OnRep_FeetMesh, EditAnywhere, Category = "SkeletalMesh")
	USkeletalMesh* FeetMesh;

	// OnRep_SkeletalMeshes 

	UFUNCTION()
	void OnRep_FaceMesh();
	UFUNCTION()
	void OnRep_EyesMesh();
	UFUNCTION()
	void OnRep_HeadMesh();
	UFUNCTION()
	void OnRep_ChestArmorMesh();
	UFUNCTION()
	void OnRep_ChestMesh();
	UFUNCTION()
	void OnRep_BackMesh();
	UFUNCTION()
	void OnRep_BeltMesh();
	UFUNCTION()
	void OnRep_HandsMesh();
	UFUNCTION()
	void OnRep_LegsMesh();
	UFUNCTION()
	void OnRep_FeetMesh();


	TMap<EEquippableSlot, USkeletalMesh**> TMapESlotSkeletalMeshes;

	TMap<EEquippableSlot, AWeapon**> TMapESlotWeapons;

	TMap<EEquippableSlot, FName> TMapESlotWeaponSocketName;

	//UPROPERTY(BlueprintReadOnly, Category = "SkeletalMeshComponentMap")
	TMap<EEquippableSlot, USkeletalMeshComponent*> ClothesMeshComponents;

	//-- Cameras
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* ThirdPersonSpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* TirdPersonCameraComponent;
	 
	//-- Interaction
	UPROPERTY()
	FInteractionData InteractionData;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float InteractionCheckFrequency;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float InteractionCheckDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UEquipmentComponent* EquipmentComponent;
	//-- Inventory
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UInventoryComponent* EquipmentsInventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UInventoryComponent* ChestInventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UInventoryComponent* BeltInventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UInventoryComponent* LegsInventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UInventoryComponent* BackInventory;

	FORCEINLINE UEquipmentComponent* GetEquipmentComponent() { return EquipmentComponent; }

	FORCEINLINE UInventoryComponent* GetEquipmentsInventory() { return EquipmentsInventory; }

	FORCEINLINE UInventoryComponent* GetChestInventory() { return ChestInventory; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float MaxHealth;

	UFUNCTION()
	void OnRep_Health(float OldHealth);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthModified(const float HealthDelta);

	float ModifyHealth(const float Delta);

	void MoveForward(float Value);
	void MoveRight(float Value);

	UPROPERTY(EditDefaultsOnly, Category = Movement)
	float SprintSpeed;

	UPROPERTY(EditDefaultsOnly, Category = Movement)
	float WalkSpeed;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = Movement)
	bool bSprinting;

	bool CanSprint() const;

	void StartSprinting();
	void StopSprinting();

	void SetSprinting(const bool bNewSprinting);

	UFUNCTION(Server, Reliable)
	void ServerSetSprinting(const bool bNewSprinting);

	//-- Interaction

	UInteractionComponent* DetectInteractable();

	void UpdateInteractable();

	FORCEINLINE class UInteractionComponent* GetInteractable() const { return InteractionData.ViewedInteractionComponent; }

	UFUNCTION(BlueprintCallable)
	void BeginInteract();

	void EndInteract();

	UFUNCTION(Server, Reliable)
	void ServerInteract();

	void Interact();

	FTimerHandle TimerHandle_Interact;

	bool IsInteracting() const;
	float GetRemainingInteractTime() const;

	//UFUNCTION(BlueprintCallable)
	//void ResetInteractionData();

	//Item operation

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<class APickup> PickupClass;

protected:

	UPROPERTY(ReplicatedUsing = OnRep_EquippedItems, VisibleAnywhere, Category = "Items")
	TArray<UEquippableItem*> EquippedItems;


public:
	UEquippableItem* FindFromEquippedItems(UItem* TrargetItem);

	UEquippableItem* FindEquippedItemBySlot(EEquippableSlot Slot);

	FORCEINLINE void AddEquippedItem(UEquippableItem* Item) { EquippedItems.Add(Item); }

	FORCEINLINE void RemoveEquippedItem(UEquippableItem* Item) { EquippedItems.RemoveSingle(Item); }

	UFUNCTION()
	void OnRep_EquippedItems();

	UFUNCTION(BlueprintPure)
	FORCEINLINE TArray<UEquippableItem*> GetEquippedItems() const { return EquippedItems; }


	AWeapon* PrimaryWeapon;
	AWeapon* SecondaryWeapon;
	AWeapon* TertiaryWeapon;

	UPROPERTY(BlueprintAssignable, Category = "Items")
	FOnEquippedItemsChanged OnEquippedItemsChanged;







	void EquipGear(class UGearItem* Gear);
	void UnEquipGear(const EEquippableSlot Slot);

	// Weapon
	UPROPERTY(VisibleAnywhere, Replicated)
	class AWeapon* EquippedWeapon;

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bHoldingRifle;

	bool EquipWeapon(class UWeaponItem* WeaponItem);
	void UnEquipWeapon(class UWeaponItem* WeaponItem);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	FORCEINLINE class AWeapon* GetEquippedWeapon() const { return EquippedWeapon; }

	void ToggleCrouching();

	void StartAttack();
	void StopAttack();

	void BeginMeleeAttack();

	UPROPERTY()
	float LastMeleeAttackTime;

	UPROPERTY(EditDefaultsOnly, Category = Melee)
	class UAnimMontage* MeleeAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Melee)
	class UAnimMontage* BeShotMontage;

	UPROPERTY(EditDefaultsOnly, Category = Melee)
	float MeleeAttackDistance;

	UPROPERTY(EditDefaultsOnly, Category = Melee)
	float MeleeAttackDamage;

	UFUNCTION(Reliable, Server)
	void ServerStartAttack();

	UFUNCTION(Reliable, NetMulticast)
	void NetMulticastPlayMeleeAttackMontage();

	UFUNCTION(Reliable, NetMulticast)
	void NetMulticastPlayDeathMontage();

	void HoldWeapon(EEquippableSlot Slot);

	UFUNCTION(Server, Reliable)
	void ServerHoldWeapon(EEquippableSlot Slot);

	void HoldPrimaryWeapon();

	void HoldSecondaryWeapon();

	void HoldTertiaryWeapon();



	void UnEquipUsingWeapon();

	//--
	TArray<EEquippableSlot> WeaponSlots;

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	FORCEINLINE TArray<EEquippableSlot> GetWeaponSlots() const { return WeaponSlots; }

	void TryFindEquippableWeaponSlot(UEquippableItem* Item);

	EEquippableSlot CurrentHoldSlot;

	UFUNCTION(BlueprintCallable, Category = "TakeItem")
	void Player_TakeItem(UItem* Item);

	UFUNCTION(Reliable, Server)
	void Server_Player_TakeItem(UItem* Item);

	UFUNCTION(BlueprintCallable, Category = "TakeItem")
	void ClientUnEquipItem(UEquippableItem* Item);

	UFUNCTION(Reliable, Server)
	void ServerClientUnEquipItem(UEquippableItem* Item);

	//--
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TArrayESlotInventoryComponents")
	TArray<UInventoryComponent*> TArrayESlotInventoryComponents;

	void TryAddToActiveInventory(UItem* Item, UInventoryComponent* BesidesInventory = NULL);

	bool HaveEnoughWeightFromActiveInventory(int32 Weight, UInventoryComponent* BesidesInventory = NULL);

	UFUNCTION(BlueprintCallable, Category = "TArrayESlotInventoryComponents")
	UInventoryComponent* FindInventoryComponentBySlot(EEquippableSlot slot);

	//--
	UFUNCTION(BlueprintCallable, Category = "Character")
	void TryAddItemQuantityToInventory(UItem* Item, int32 ExpectAddQuantity, UInventoryComponent* DestinationInventory);

	UFUNCTION(Reliable, Server)
	void Server_TryAddItemQuantityToInventory(UItem* Item, int32 ExpectAddQuantity, UInventoryComponent* DestinationInventory);

	bool EquipItem(UEquippableItem* EquippableItem, EEquippableSlot Slot = EEquippableSlot::EIS_None);

	UFUNCTION(BlueprintCallable, Category = "UnEquipItem")
	bool UnEquipItem(class UEquippableItem* EquippableItem);

	UFUNCTION(BlueprintCallable, Category = "UnEquipItem")
	bool DropItemFromEquipmentComponent(UEquippableItem* EquippableItem);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void SpawnPickup(UItem* Item, int32 DropQuantity);

	//-- Use Item
	UFUNCTION(BlueprintCallable, Category = "Items")
	void Player_UseItem(class UItem* Item);

	UFUNCTION(Server, Reliable)
	void Server_Player_UseItem(class UItem* Item);



	//-- Equip Item
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	bool Player_EquipItem(UEquippableItem* EquippableItem, EEquippableSlot TargetSlot = EEquippableSlot::EIS_None);

	UFUNCTION(Server, Reliable)
	void Server_Player_EquipItem(UEquippableItem* EquippableItem, EEquippableSlot TargetSlot = EEquippableSlot::EIS_None);

	//-- UnEquip Item
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	bool Player_UnEquipItem(UEquippableItem* EquippableItem);

	UFUNCTION(Server, Reliable)
	void Server_Player_UnEquipItem(UEquippableItem* EquippableItem);
	
	//-- Drop Item From EquipmentComponent
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	bool Player_DropItemFromEquipmentComponent(UEquippableItem* EquippableItem);

	UFUNCTION(Server, Reliable)
	void Server_Player_DropItemFromEquipmentComponent(UEquippableItem* EquippableItem);

	//-- Drop Item From InventoryComponent
	UFUNCTION(BlueprintCallable, Category = "Items")
	void Player_DropItemFromInventoryComponent(UItem* Item, const int32 Quantity, UInventoryComponent* SourceInventory);

	UFUNCTION(Server, Reliable)
	void Server_Player_DropItemFromInventoryComponent(class UItem* Item, const int32 Quantity, UInventoryComponent* SourceInventory);

};
