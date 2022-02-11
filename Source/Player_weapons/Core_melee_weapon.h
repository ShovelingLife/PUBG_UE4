/**
 * \file Core_melee_weapon.h
 *
 * \brief 모든 근접 무기들은 이로부터 상속받음
 *
 * \ingroup Player_weapons
 *
 * \author ShovelingLife
 */
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
    /**
      * \brief 근접 무기 초기화
      * \param _weapon_type 무기 종류
     */
    void Init(e_melee_weapon_type _weapon_type);

    /**
      * \brief 메쉬 초기화
     */
    void Init_mesh();

    /**
      * \brief 콜라이더 정보 갱신
     */
    void Update_collider();
};