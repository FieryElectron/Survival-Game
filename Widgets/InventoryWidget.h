// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/EquippableItem.h"
#include "InventoryWidget.generated.h"

class UInventoryComponent;


UCLASS()
class SURVIVALGAME_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	EEquippableSlot EquippableSlot;

	UPROPERTY(BlueprintReadWrite, Category = "Components")
	UInventoryComponent* Inventory;
	
};
