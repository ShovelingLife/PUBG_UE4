/**
 * \file My_enum.h
 *
 * \brief ���� �� ������ ������ ����
 *
 * \ingroup PUBG_UE4
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "MyEnum.generated.h"

 /**
   * \brief �÷��̾� ���� ����
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
  * \brief ���� ������ ����
 */
UENUM()
enum class EConsumableType
{
    NONE
};

/**
  * \brief �� ����
 */
UENUM()
enum class EEquipmentType
{
    NONE
};

/**
  * \brief ���� ����
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
  * \brief �¼� ����
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
  * \brief �÷��̾� ���� ���� \n
  * ITEM_FARM �Ĺ� \n
  * WEAPON_EQUIP ���� \n
  * WEAPON_SWAP ��ü \n
 */
UENUM()
enum class EPlayerSoundType
{
    ITEM_FARM,
    WEAPON_EQUIP,
    WEAPON_SWAP
};

/**
 * \brief �ѱ� ���� ���� \n
 * EMPTY_AMMO �� źâ \n
 * SHOT �ݹ� \n
 * RELOAD ���� \n
 */
UENUM()
enum class EWeaponSoundType
{
    EMPTY_AMMO,
    SHOT,
    RELOAD
};