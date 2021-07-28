#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

class UInventoryComponent;
class UStaticMesh;
class UTexture2D;
class ASurvivalCharacter;
class APickup;
class UEquipmentComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemModified);

UENUM(BlueprintType)
enum class EItemRarity : uint8{
	IR_Common		UMETA(DisplayName = "Common"),
	IR_Uncommon		UMETA(DisplayName = "Uncommon"),
	IR_Rare			UMETA(DisplayName = "Rare"),
	IR_VeryRare		UMETA(DisplayName = "Very Rare"),
	IR_Epic 		UMETA(DisplayName = "Epic"),
	IR_Legendary 	UMETA(DisplayName = "Legendary")
};

USTRUCT(BlueprintType)
struct FItemAddResult {
	GENERATED_BODY()

	UPROPERTY()
	int32 AddedQuantity;

	UPROPERTY()
	int32 RemainQuantity;
};

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class SURVIVALGAME_API UItem : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(ReplicatedUsing = OnRep_Quantity, EditDefaultsOnly, meta = (UIMin = 1/*, EditCondition = bStackable*/))
	int32 Quantity;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1/*, EditCondition = bStackable*/))
	int32 MaxStackSize;

	UPROPERTY(EditDefaultsOnly)
	bool bStackable;

	UPROPERTY(Replicated)
	UInventoryComponent* OwningInventory;

	UPROPERTY(Replicated)
	UEquipmentComponent* OwningEquipment;

	UPROPERTY(Replicated)
	APickup* OwningPickup;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* PickupMesh;

	UPROPERTY(EditDefaultsOnly)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* Thumbnail;

	UPROPERTY(EditDefaultsOnly, meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditDefaultsOnly)
	FText UseActionText;

	UPROPERTY(EditDefaultsOnly)
	EItemRarity Rarity;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1))
	int32 Weight;

	UPROPERTY(EditDefaultsOnly)
	bool bHasBindOnModified;

	UPROPERTY()
	int32 RepKey;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetStackable(bool NewStackable) { bStackable = NewStackable; }

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	UItem();

	static FItemAddResult ItemAddResultConstructor(const int32 AddedQuantity, const int32 RemainQuantity) {
		FItemAddResult ItemAddResult;
		ItemAddResult.AddedQuantity = AddedQuantity;
		ItemAddResult.RemainQuantity = RemainQuantity;

		return ItemAddResult;
	}

	UPROPERTY(BlueprintAssignable)
	FOnItemModified OnItemModified;

	//-- Gets --
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE int32 GetMaxStackSize() const { return MaxStackSize; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE int32 GetQuantity() const { return Quantity; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE int32 GetWeight() const { return Weight; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE bool IsStackable() const { return bStackable; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE UInventoryComponent* GetOwningInventory() const { return OwningInventory; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE UEquipmentComponent* GetOwningEquipment() const { return OwningEquipment; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE APickup* GetOwningPickup() const { return OwningPickup; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE UStaticMesh* GetPickupMesh() const { return PickupMesh; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE FText GetDisplayName() const { return DisplayName; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE UTexture2D* GetThumbnail() const { return Thumbnail; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE FText GetDescription() const { return Description; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE FText GetUseActionText() const { return UseActionText; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE bool IsStackFull() const { return Quantity >= MaxStackSize; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE bool HasBindOnModified() const { return bHasBindOnModified; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE int32 GetRepKey() const { return RepKey; }


	//-- Sets --

	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetDisplayName(const FText NewDisplayName) { DisplayName = NewDisplayName; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetThumbnail(UTexture2D* NewThumbnail) { Thumbnail = NewThumbnail; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetQuantity(const int32 NewQuantity) { Quantity = NewQuantity;/*Quantity = FMath::Clamp(NewQuantity, 0, bStackable ? MaxStackSize : 1);*/ }

	UFUNCTION(BlueprintCallable, Category = "Item")
	FItemAddResult TryAddQuantityTillStackFull(const int32 ExpectAddQuantity);

	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetOwningInventory(UInventoryComponent* NewInventoryComponent) { OwningInventory = NewInventoryComponent; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetOwningEquipment(UEquipmentComponent* NewEquipmentComponent) { OwningEquipment = NewEquipmentComponent; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetOwningPickup(APickup* NewPickup) { OwningPickup = NewPickup; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetHasBindOnModified(bool NewHasBindOnModified) { bHasBindOnModified = NewHasBindOnModified; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	void IncrementRepKey() { ++RepKey; }

	//--

	virtual void MarkDirtyForReplication();

	UFUNCTION()
	void OnRep_Quantity();

	virtual void Use(ASurvivalCharacter* Character);

	virtual bool Pre_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity);
	virtual bool Post_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity);

	virtual bool Pre_DropFromInventoryComponent(UInventoryComponent* SourceInventory, int32 DropQuantity);
	virtual bool Post_DropFromInventoryComponent(UInventoryComponent* SourceInventory, int32 DropQuantity);
	
};
