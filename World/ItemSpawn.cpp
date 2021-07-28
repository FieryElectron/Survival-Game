#include "ItemSpawn.h"
#include "World/Pickup.h"
#include "Items/Item.h"
#include "Engine/World.h"
#include "TimerManager.h"

AItemSpawn::AItemSpawn(){
	SetActorTickEnabled(false);
	bNetLoadOnClient = false;

	SetReplicates(false);

	RespawnRange = FIntPoint(1, 3);
}

void AItemSpawn::BeginPlay(){
	Super::BeginPlay();

	if (HasAuthority()){
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnItem, this, &AItemSpawn::SpawnItem, 2, false);
		//SpawnItem();
	}
}

void AItemSpawn::SpawnItem(){
	if (HasAuthority() && LootTable){
		TArray<FLootTableRow*> LootAllRows;
		LootTable->GetAllRows("", LootAllRows);

		FLootTableRow* LootRow = LootAllRows[FMath::RandRange(0, LootAllRows.Num() - 1)];

		float RollProbability = FMath::FRandRange(0.0f, 1.0f);

		while (RollProbability > LootRow->Probability){
			LootRow = LootAllRows[FMath::RandRange(0, LootAllRows.Num() - 1)];
			RollProbability = FMath::FRandRange(0.0f, 1.0f);
		}

		if (LootRow && LootRow->Items.Num() && PickupClass){
			float Angle = 0.f;
			float DeltaAngle = 2.0f * PI / (float)LootRow->Items.Num();

			for (auto& ItemClass : LootRow->Items){

				const FVector LocationOffset = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f) * LootRow->SpawnRadius;

				FActorSpawnParameters SpawnParams;
				SpawnParams.bNoFail = true;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				const int32 ItemQuantity = ItemClass->GetDefaultObject<UItem>()->GetQuantity();

				FTransform SpawnTransform = GetActorTransform();
				SpawnTransform.AddToTranslation(LocationOffset);

				APickup* Pickup = GetWorld()->SpawnActor<APickup>(PickupClass, SpawnTransform, SpawnParams);
				Pickup->InitializePickup(ItemClass, ItemQuantity);
				Pickup->OnDestroyed.AddUniqueDynamic(this, &AItemSpawn::OnItemTaken);

				SpawnedPickups.Add(Pickup);

				Angle += DeltaAngle;
			}
		}
	}
}

void AItemSpawn::OnItemTaken(AActor* DestroyedActor){
	if (HasAuthority()){
		SpawnedPickups.Remove(DestroyedActor);

		if (SpawnedPickups.Num() <= 0){
			GetWorldTimerManager().SetTimer(TimerHandle_RespawnItem, this, &AItemSpawn::SpawnItem, FMath::RandRange(RespawnRange.GetMin(), RespawnRange.GetMax()), false);
		}
	}
}
