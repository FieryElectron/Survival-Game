#pragma once

#include "CoreMinimal.h"
#include "Items/EquippableItem.h"
#include "GearItem.generated.h"

class ASurvivalCharacter;
class UEquipmentComponent;

UCLASS(Blueprintable)
class SURVIVALGAME_API UGearItem : public UEquippableItem
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly)
	class USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditDefaultsOnly)
	class UMaterialInstance* MaterialInstance;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.0, ClampMax = 1.0))
	float DamageDefenceMultiplier;


public:
	UGearItem();

	//-- Gets -- 
	UFUNCTION(BlueprintPure, Category = "GearItem")
	FORCEINLINE	USkeletalMesh* GetSkeletalMesh() const { return SkeletalMesh; };

	//virtual bool Equip(class ASurvivalCharacter* Character) override;
	//virtual bool UnEquip(class ASurvivalCharacter* Character) override;

	virtual bool Pre_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity) override;
	virtual bool Post_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity) override;
	
	virtual bool Pre_EquipToEquipmentComponent(UEquipmentComponent* EquipmentComponent, EEquippableSlot TargetSlot) override;
	virtual bool Post_EquipToEquipmentComponent(UEquipmentComponent* EquipmentComponent) override;

	virtual bool Pre_UnEquipFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) override;
	virtual bool Post_UnEquipFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) override;

	virtual bool Pre_DropFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) override;
	virtual bool Post_DropFromEquipmentComponent(UEquipmentComponent* EquipmentComponent) override;

	



	
};
