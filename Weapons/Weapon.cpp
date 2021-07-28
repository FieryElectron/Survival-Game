// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SkeletalMeshComponent.h"
#include "Weapon.h"

#include "Player/SurvivalPlayerController.h"
#include "Player/SurvivalCharacter.h"
#include "Net/UnrealNetwork.h"

#include "SurvivalGame.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundAttenuation.h"

#include "DrawDebugHelpers.h"

static const FName NAME_MuzzleSocket("Muzzle");

// Sets default values
AWeapon::AWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->bReceivesDecals = false;
	WeaponMesh->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	RootComponent = WeaponMesh;

	AttachSocket1P = FName("GripPoint");
	AttachSocket3P = FName("GripPoint");

	bReplicates = true;
	bNetUseOwnerRelevancy = true;

	PrimaryActorTick.bCanEverTick = true;

}

void AWeapon::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);


	if (HasAuthority()) {
		SimulateFlyingProjectiles(DeltaTime);
	}

}

void AWeapon::SimulateFlyingProjectiles(float DeltaTime) {
	for (int i = FlyingProjectiles.Num() - 1; i > -1; --i) {
		FlyingProjectiles[i].CurrentSimulateTime += DeltaTime;
		FlyingProjectiles[i].CurrentLifeTime += DeltaTime;

		if (FlyingProjectiles[i].CurrentLifeTime > FlyingProjectiles[i].MaxLifeTime) {
			FlyingProjectiles.RemoveAt(i);
			continue;
		}

		if (FlyingProjectiles[i].CurrentSimulateTime > FlyingProjectiles[i].MaxSimulateTime) {
			FHitResult HitResult;
			FVector LastTracePos;
			FVector LastTraceVelocity;

			bool Hit = PredictProjectileByChannel(FlyingProjectiles[i].StartPos, FlyingProjectiles[i].LaunchVelocity, HitResult, LastTracePos, LastTraceVelocity);

			if (Hit) {
				if (ASurvivalCharacter* OwnerCharacter = Cast<ASurvivalCharacter>(HitResult.GetActor())) {
					DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 15, FColor::Green, false, 3, 10);

					UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), 1,
													   FVector(0), HitResult,
													   PawnOwner->GetController(), this, UWeaponDamage::StaticClass());

					ASurvivalPlayerController* SurvivalPlayerController = Cast<ASurvivalPlayerController>(PawnOwner->GetController());
					SurvivalPlayerController->ClientShotHitConfirmed();

					ClientPlayHitPlayerSound();
				}
				FlyingProjectiles.RemoveAt(i);
			} else {
				FlyingProjectiles[i].StartPos = LastTracePos;
				FlyingProjectiles[i].LaunchVelocity = LastTraceVelocity;
				FlyingProjectiles[i].CurrentSimulateTime = 0;
			}
		}
	}

}
void AWeapon::NetMulticastPlaySound_Implementation() {
	//if (GetNetMode() == NM_Client) {
		if (!PawnOwner->IsLocallyControlled()) {
			UGameplayStatics::SpawnEmitterAttached(MuzzleParticles, WeaponMesh, NAME_MuzzleSocket);
			UGameplayStatics::SpawnSoundAttached(AttackSound, WeaponMesh, NAME_MuzzleSocket, FVector(0), FRotator(), EAttachLocation::KeepRelativeOffset, false, 1.0f, 1.0f, 0.0f, SoundAttenuation);
		}
	//}
}
UAudioComponent* AWeapon::PlayWeaponSound(USoundCue* Sound) {
	UAudioComponent* AC = NULL;
	if (Sound) {
		AC = UGameplayStatics::SpawnSoundAttached(Sound, GetRootComponent());
	}

	return AC;
}
void AWeapon::ClientPlayHitPlayerSound_Implementation() {
	if (PawnOwner->IsLocallyControlled()) {
		PlayWeaponSound(HitPlayerSound);
	}
}

bool AWeapon::PredictProjectileByChannel(FVector StartPos, FVector LaunchVelocity, FHitResult& HitResult, FVector& LastTracePos, FVector& LastTraceVelocity) {
	TArray<FVector> OutPathPositions;
	FVector OutLastTraceDestination;


	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Push(PawnOwner);


	float ProjectileRadius = 1;
	float MaxSimTime = 0.1;

	FPredictProjectilePathParams Params = FPredictProjectilePathParams(ProjectileRadius, StartPos, LaunchVelocity, MaxSimTime);
	Params.bTraceWithCollision = true;
	Params.bTraceComplex = false;
	Params.ActorsToIgnore = ActorsToIgnore;
	Params.DrawDebugType = EDrawDebugTrace::None;
	Params.DrawDebugTime = 0;
	Params.SimFrequency = 20;
	Params.OverrideGravityZ = -0.2;
	Params.TraceChannel = COLLISION_WEAPON;

	FPredictProjectilePathResult PredictResult;
	bool bHit = UGameplayStatics::PredictProjectilePath(GetWorld(), Params, PredictResult);

	HitResult = PredictResult.HitResult;
	OutLastTraceDestination = PredictResult.LastTraceDestination.Location;
	OutPathPositions.Empty(PredictResult.PathData.Num());

	LastTracePos = PredictResult.LastTraceDestination.Location;
	LastTraceVelocity = PredictResult.LastTraceDestination.Velocity;


	for (const FPredictProjectilePathPointData& PathPoint : PredictResult.PathData) {
		OutPathPositions.Add(PathPoint.Location);
	}

	for (int i = 0; i < OutPathPositions.Num() - 1; ++i) {
		DrawDebugLine(GetWorld(), OutPathPositions[i], OutPathPositions[i + 1], FColor::Red, false, 5, 0, 1);
	}

	return bHit;
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	

	DOREPLIFETIME(AWeapon, PawnOwner);

	//DOREPLIFETIME_CONDITION(AWeapon, CurrentAmmoInClip, COND_OwnerOnly);
	//DOREPLIFETIME_CONDITION(AWeapon, BurstCounter, COND_SkipOwner);
	//DOREPLIFETIME_CONDITION(AWeapon, bPendingReload, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AWeapon, Item, COND_InitialOnly);
}

void AWeapon::PostInitializeComponents() {
	Super::PostInitializeComponents();
}

void AWeapon::BeginPlay(){
	Super::BeginPlay();

	if (HasAuthority()) {
		PawnOwner = Cast<ASurvivalCharacter>(GetOwner());
	}
	
}
void AWeapon::OnUnEquip() {
	DetachMeshFromPawn();

	//SetHidden(true);
}


void AWeapon::OnEquip() {
	//if (HasAuthority()) {
	//	UE_LOG(LogTemp, Warning, TEXT("HasAuthority"));
	//} else {
	//	UE_LOG(LogTemp, Warning, TEXT("No HasAuthority"));
	//}

	AttachMeshToPawn();

	

	//bPendingEquip = true;
	//DetermineWeaponState();

	//OnEquipFinished();


	//if (PawnOwner && PawnOwner->IsLocallyControlled()) {
	//	PlayWeaponSound(EquipSound);
	//}
}


void AWeapon::AttachMeshToPawn() {
	if (PawnOwner) {
		if (const USkeletalMeshComponent* PawnMesh = PawnOwner->GetMesh()) {
			const FName AttachSocket = PawnOwner->IsLocallyControlled() ? AttachSocket1P : AttachSocket3P;
			AttachToComponent(PawnOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSocket);
		}
	}
}

void AWeapon::DetachMeshFromPawn() {
	if (PawnOwner) {
		if (const USkeletalMeshComponent* PawnMesh = PawnOwner->GetMesh()) {
			const FName AttachSocket = PawnOwner->IsLocallyControlled() ? AttachSocket1P : AttachSocket3P;
			DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			//DetachFromComponent(PawnOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSocket);
			//AttachToComponent(PawnOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSocket);
		}
	}
}

void AWeapon::OnRep_PawnOwner() {
	//UE_LOG(LogTemp, Warning, TEXT("OnRep_PawnOwner"));
}

bool AWeapon::ShootAngleIsValid(const FVector& ViewPoint, const FVector& MuzzlePoint, const FVector& ImpactPoint) {
	const FVector ViewVector = ImpactPoint - ViewPoint;
	const FVector MuzzleVector = ImpactPoint - MuzzlePoint;
	const float CosineAngle2D = MuzzleVector.CosineAngle2D(ViewVector);

	if (CosineAngle2D > 0.0f) {
		return true;
	} else {
		return false;
	}
}

void AWeapon::StartFire() {

	if (GetNetMode() < NM_Client) {

		if (GetNetMode() != NM_DedicatedServer) {
			UGameplayStatics::SpawnEmitterAttached(MuzzleParticles, WeaponMesh, NAME_MuzzleSocket);
			UGameplayStatics::SpawnSoundAttached(AttackSound, WeaponMesh, NAME_MuzzleSocket);
		}

		NetMulticastPlaySound();
		
		if (ASurvivalPlayerController* SurvivalPlayerController = Cast<ASurvivalPlayerController>(PawnOwner->GetController())) {
			FVector AimLoc;
			FRotator AimRot;

			SurvivalPlayerController->GetPlayerViewPoint(AimLoc, AimRot);
			FVector StartPos = AimLoc;

			FVector LaunchVelocity = AimRot.Vector() * 10000.0f;
			FHitResult HitResult;

			FProjectile projectile = FProjectile();
			projectile.StartPos = StartPos;
			projectile.LaunchVelocity = LaunchVelocity;

			FlyingProjectiles.Push(projectile);
		}
	} else {
		UGameplayStatics::SpawnEmitterAttached(MuzzleParticles, WeaponMesh, NAME_MuzzleSocket);
		UGameplayStatics::SpawnSoundAttached(AttackSound, WeaponMesh, NAME_MuzzleSocket);
	}
}

void AWeapon::DealDamage(const FHitResult& Impact, const FVector& ShootDir) {
	//UE_LOG(LogTemp, Warning, TEXT("DealDamage"));
	if (ASurvivalCharacter* OwnerCharacter = Cast<ASurvivalCharacter>(GetOwner())) {

		if (AActor* HitActor = Impact.GetActor()) {
			float DamageAmount = HitScanConfig.Damage;

			FPointDamageEvent PointDmg;
			PointDmg.DamageTypeClass = HitScanConfig.DamageType;
			PointDmg.HitInfo = Impact;
			PointDmg.ShotDirection = ShootDir;
			PointDmg.Damage = DamageAmount;

			HitActor->TakeDamage(PointDmg.Damage, PointDmg, OwnerCharacter->Controller, this);
		}
	}
}

void AWeapon::AttachMeshToSocket(FName Socket) {
	if (PawnOwner) {
		if (const USkeletalMeshComponent* PawnMesh = PawnOwner->GetMesh()) {
			AttachToComponent(PawnOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Socket);
		}
	}
}