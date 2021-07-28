// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SurvivalDamageBase.h"
#include "Weapon.generated.h"

class UAudioComponent;
class USoundCue;
class UParticleSystem;
class USoundAttenuation;


USTRUCT()
struct FProjectile {
	GENERATED_BODY()

	FProjectile() {
		StartPos = FVector(0);
		LaunchVelocity = FVector(0);
		CurrentLifeTime = 0;
		MaxLifeTime = 3;
		CurrentSimulateTime = 1;
		MaxSimulateTime = 0.1;
	}

	UPROPERTY()
	FVector StartPos;

	UPROPERTY()
	FVector LaunchVelocity;

	UPROPERTY()
	float CurrentLifeTime;

	UPROPERTY()
	float MaxLifeTime;

	UPROPERTY()
	float CurrentSimulateTime;

	UPROPERTY()
	float MaxSimulateTime;
};

USTRUCT(BlueprintType)
struct FHitScanConfiguration {
	GENERATED_BODY()

		FHitScanConfiguration() {
		Distance = 10000.f;
		Damage = 25.f;
		Radius = 0.f;
		DamageType = UWeaponDamage::StaticClass();
		ClientSideHitLeeway = 300.f;
	}

	/**A map of bone -> damage amount. If the bone is a child of the given bone, it will use this damage amount.
	A value of 2 would mean double damage etc */
	UPROPERTY(EditDefaultsOnly, Category = "Trace Info")
	TMap<FName, float> BoneDamageModifiers;

	/**How far the hitscan traces for a hit*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Info")
	float Distance;

	/**The amount of damage to deal when we hit a player with the hitscan*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Info")
	float Damage;

	/**Optional trace radius. A value of zero is just a linetrace, anything higher is a spheretrace*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Info")
	float Radius;

	/**client side hit leeway for BoundingBox check*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Info")
	float ClientSideHitLeeway;

	/** type of damage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Info")
	TSubclassOf<UDamageType> DamageType;

};


UCLASS()
class SURVIVALGAME_API AWeapon : public AActor
{
	GENERATED_BODY()

	friend class ASurvivalCharacter;
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(EditAnywhere, Category = Components)
	USkeletalMeshComponent* WeaponMesh;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	virtual void OnEquip();
	virtual void OnUnEquip();

	UPROPERTY(Transient, ReplicatedUsing = OnRep_PawnOwner)
	class ASurvivalCharacter* PawnOwner;

	/**Name of the socket to attach to the character on*/
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	FName AttachSocket1P;

	/**Name of the socket to attach to the character on*/
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	FName AttachSocket3P;

	void AttachMeshToPawn();

	void DetachMeshFromPawn();

	void AttachMeshToSocket(FName Socket);

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	FName AttachSocketName;



	

	UFUNCTION()
	void OnRep_PawnOwner();

	UPROPERTY(Replicated, BlueprintReadOnly, Transient)
	class UWeaponItem* Item;

	virtual void StartFire();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Config)
	FHitScanConfiguration HitScanConfig;

	void DealDamage(const FHitResult& Impact, const FVector& ShootDir);
	
	bool ShootAngleIsValid(const FVector& ViewPoint, const FVector& MuzzlePoint, const FVector& ImpactPoint);

	UPROPERTY(EditDefaultsOnly, Category = Melee)
	float MeleeAttackDamage;

public:


	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundCue* HitPlayerSound;

	UAudioComponent* PlayWeaponSound(USoundCue* Sound);

	TArray<FProjectile> FlyingProjectiles;

	virtual void Tick(float DeltaTime) override;

	void SimulateFlyingProjectiles(float DeltaTime);

	bool PredictProjectileByChannel(FVector StartPos, FVector LaunchVelocity, FHitResult& HitResult, FVector& LastTracePos, FVector& LastTraceVelocity);

	UFUNCTION(Client, Reliable)
	void ClientPlayHitPlayerSound();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastPlaySound();

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* MuzzleParticles;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundCue* AttackSound;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundAttenuation* SoundAttenuation;
};
