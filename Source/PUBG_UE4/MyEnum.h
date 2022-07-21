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
 * \brief 플레이어 애니메이션 타입
 * IDLE 가만히 서있기
 * WALK 걷기
 * SPRINT 뛰기
 * JUMP 점프하기
 * CROUCH 숙이기
 * PRONING 엎드리기
 * AIM 조준하기 (서서쏴)
 * THROW 던지기
 * SWIM 수영하기
 * INJURED 부상
 * DEAD 사망
 * CROUCH_WALK 숙인채 걷기
 * PRONING_WALK 엎드린채 걷기
 * AIM_WALK 조준한채 걷기
 * SPRINT_JUMP 뛴 후 점프
 * CROUCH_AIM 앉아쏴
 * PRONING_AIM 엎드려쏴
 * TEST
 */
UENUM(BlueprintType)
enum class EPlayerState : uint8
{
    IDLE         UMETA(DisplayName = "IDLE"),
    WALK         UMETA(DisplayName = "WALK"),
    SPRINT       UMETA(DisplayName = "SPRINT"),
    JUMP         UMETA(DisplayName = "JUMP"),
    CROUCH       UMETA(DisplayName = "CROUCH"),
    PRONING      UMETA(DisplayName = "PRONING"),
    AIM          UMETA(DisplayName = "AIM"),
    THROW        UMETA(DisplayName = "THROW"),
    SWIM         UMETA(DisplayName = "SWIM"),
    INJURED      UMETA(DisplayName = "INJURED"),
    DEAD         UMETA(DisplayName = "DEAD"),
    CROUCH_WALK  UMETA(DisplayName = "CROUCH_WALKING"),
    PRONING_WALK UMETA(DisplayName = "PRONING_WALKING"),
    AIM_WALK     UMETA(DisplayName = "AIM_WALKING"),
    SPRINT_JUMP  UMETA(DisplayName = "JUMP_ON_SPRINT"),
    CROUCH_AIM   UMETA(DisplayName = "CROUCH_ON_AIM"),
    PRONING_AIM  UMETA(DisplayName = "CROUCH_ON_PRONING")
};

UENUM()
enum class EPlayerOtherState
{
    BURNED,
    NONE
};

/** \brief 섭취 아이템 종류 */
UENUM()
enum class EConsumableType
{
    NONE
};

/** \brief 방어구 종류 */
UENUM()
enum class EEquipmentType
{
    NONE
};

/** \brief 차량 종류 */
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

/** \brief 좌석 종류 */
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
    WEAPON_SWAP,
    NONE
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

UENUM()
enum class EPlayerStateAnimType
{
    BLINDED, // 섬광탄
    BURNED,  // 화염병
    INJURED, // 부상
    FIELD,   // 자기장
    NONE
};