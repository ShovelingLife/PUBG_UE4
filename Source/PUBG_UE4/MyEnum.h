/**
 * \file My_enum.h
 *
 * \brief 게임 내 쓰여질 열거형 관리
 *
 * \ingroup PUBG_UE4
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "MyEnum.generated.h"

 /**
   * \brief 플레이어 상태 관련
  */
UENUM()
enum class EPlayerState
{
    IDLE,
    WALK,
    SPRINT,
    JUMP,
    CROUCH,
    PRONING,
    AIM,
    THROW,
    SWIM,
    INJURED,
    DEAD,
    CROUCH_WALK,
    PRONING_WALK,
    AIM_WALK,
    SPRINT_JUMP,
    CROUCH_AIM,
    PRONING_AIM
};

/**
  * \brief 섭취 아이템 종류
 */
UENUM()
enum class EConsumableType
{
    NONE
};

/**
  * \brief 방어구 종류
 */
UENUM()
enum class EEquipmentType
{
    NONE
};

/**
  * \brief 차량 종류
 */
UENUM()
enum class EVehicleType
{
    Hatchback,
    Pickup,
    SportsCar,
    SUV,
    Truck,
    Truck_Chassis,
    MAX
};

/**
  * \brief 좌석 종류
 */
UENUM()
enum class ESeatType
{
    FIRST,
    SECOND,
    THIRD,
    FOURTH,
    NONE
};

/**
  * \brief 플레이어 사운드 종류 \n
  * ITEM_FARM 파밍 \n
  * WEAPON_EQUIP 장착 \n
  * WEAPON_SWAP 교체 \n
 */
UENUM()
enum class EPlayerSoundType
{
    ITEM_FARM,
    WEAPON_EQUIP,
    WEAPON_SWAP
};

/**
 * \brief 총기 사운드 관련 \n
 * EMPTY_AMMO 빈 탄창 \n
 * SHOT 격발 \n
 * RELOAD 장전 \n
 */
UENUM()
enum class EWeaponSoundType
{
    EMPTY_AMMO,
    SHOT,
    RELOAD
};