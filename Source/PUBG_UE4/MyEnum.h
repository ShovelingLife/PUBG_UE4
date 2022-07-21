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
 * \brief �÷��̾� �ִϸ��̼� Ÿ��
 * IDLE ������ ���ֱ�
 * WALK �ȱ�
 * SPRINT �ٱ�
 * JUMP �����ϱ�
 * CROUCH ���̱�
 * PRONING ���帮��
 * AIM �����ϱ� (������)
 * THROW ������
 * SWIM �����ϱ�
 * INJURED �λ�
 * DEAD ���
 * CROUCH_WALK ����ä �ȱ�
 * PRONING_WALK ���帰ä �ȱ�
 * AIM_WALK ������ä �ȱ�
 * SPRINT_JUMP �� �� ����
 * CROUCH_AIM �ɾƽ�
 * PRONING_AIM �������
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

/** \brief ���� ������ ���� */
UENUM()
enum class EConsumableType
{
    NONE
};

/** \brief �� ���� */
UENUM()
enum class EEquipmentType
{
    NONE
};

/** \brief ���� ���� */
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

/** \brief �¼� ���� */
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
    WEAPON_SWAP,
    NONE
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

UENUM()
enum class EPlayerStateAnimType
{
    BLINDED, // ����ź
    BURNED,  // ȭ����
    INJURED, // �λ�
    FIELD,   // �ڱ���
    NONE
};