// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "SurvivalDamageBase.generated.h"

UCLASS()
class SURVIVALGAME_API USurvivalDamageBase : public UDamageType
{
	GENERATED_BODY()	
};


UCLASS()
class UWeaponDamage : public USurvivalDamageBase {
	GENERATED_BODY()
};

UCLASS()
class UMeleeDamage : public USurvivalDamageBase {
	GENERATED_BODY()
};

UCLASS()
class UExplosiveDamage : public USurvivalDamageBase {
	GENERATED_BODY()
};

UCLASS()
class UVehicleDamage : public USurvivalDamageBase {
	GENERATED_BODY()
};