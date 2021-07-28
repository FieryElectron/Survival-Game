// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "FoodItem.generated.h"

class ASurvivalCharacter;
class DestinationInventory;


UCLASS()
class SURVIVALGAME_API UFoodItem : public UItem
{
	GENERATED_BODY()
	
public:
	virtual bool Pre_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity) override;
	virtual bool Post_AddItemQuantityToInventory(UInventoryComponent* DestinationInventory, int32 ExpectAddQuantity) override;

};
