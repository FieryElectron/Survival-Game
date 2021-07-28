#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

class UStaticMeshComponent;
class UInteractionComponent;
class UItem;

UCLASS()
class SURVIVALGAME_API APickup : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditDefaultsOnly)
	UInteractionComponent* InteractionComponent;

protected:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, ReplicatedUsing = OnRep_Item)
	UItem* Item;

	UFUNCTION()
	void OnRep_Item();

	UFUNCTION()
	void OnItemModified();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	UFUNCTION()
	void OnTakePickup(class ASurvivalCharacter* Taker);

#if WITH_EDITOR 
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif 
public:
	APickup();

	void InitializePickup(const TSubclassOf<class UItem> ItemClass, const int32 Quantity);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UItem* ItemTemplate;

	void UpdateStaticMesh();
	void UpdateInteractableInfo();
};
