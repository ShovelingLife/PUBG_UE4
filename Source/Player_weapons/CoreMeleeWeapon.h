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
#include "PUBG_UE4/WeaponEnum.h"
#include "PUBG_UE4/BaseInteraction.h"
#include "PUBG_UE4/OtherWeaponData.h"
#include "CoreMeleeWeapon.generated.h"

UCLASS()
class PLAYER_WEAPONS_API ACoreMeleeWeapon : public ABaseInteraction
{
    GENERATED_BODY()

public:
    FsOtherWeaponData WeaponData;
    EMeleeWeaponType  WeaponType;	

public:
    ACoreMeleeWeapon();

public:
    virtual void ClickEvent() final;

protected:
    virtual void BeginPlay() override;

protected:
    /**
      * \brief 근접 무기 초기화
      * \param _weapon_type 무기 종류
     */
    void Init(EMeleeWeaponType WeaponType);
};