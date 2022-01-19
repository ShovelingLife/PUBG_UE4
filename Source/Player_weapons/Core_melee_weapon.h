// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon_enum.h"
#include "PUBG_UE4/Base_interaction.h"
#include "PUBG_UE4/Other_weapon_data.h"
#include "Core_melee_weapon.generated.h"

UCLASS()
class PLAYER_WEAPONS_API ACore_melee_weapon : public ABase_interaction
{
    GENERATED_BODY()

public:
    Fs_other_weapon_data weapon_data;
    e_melee_weapon_type  weapon_type;	

public:
    ACore_melee_weapon();

protected:
    virtual void BeginPlay() override;

protected:
    void Init(e_melee_weapon_type);

    // 메쉬 초기화
    void Init_mesh();

    // 콜라이더 정보 갱신
    void Update_collider();
};
