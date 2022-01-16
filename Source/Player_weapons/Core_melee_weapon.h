// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PUBG_UE4/Base_interaction.h"
#include "Weapon_enum.h"
#include "Core_melee_weapon.generated.h"

/**
 * 
 */
UCLASS()
class PLAYER_WEAPONS_API ACore_melee_weapon : public ABase_interaction
{
    GENERATED_BODY()

public:
    e_melee_weapon_type weapon_type;	
};
