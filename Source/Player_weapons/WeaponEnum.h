#pragma once
#include "CoreMinimal.h"
#include "WeaponEnum.generated.h"

/**
 * \brief ���� ���� ���� ���� \n
 *
 * FIRST     ù��° �� \n
 * SECOND    �ι�° �� \n
 * PISTOL    ���� \n
 * MELEE     ������ \n
 * THROWABLE ��ô�� \n
 */
UENUM()
enum class ECurrentWeaponType
{
    FIRST = 1,
    SECOND,
    PISTOL,
    MELEE,
    THROWABLE,
    NONE
};

/**
 * \brief �ѱ� ���� \n
 *
 * ------- ���� : �ִ� �Ѿ� 30 ------- \n
 *
 * C7 \n
 * LS80 \n
 * M1 \n
 *
 * ------- ���� : �ִ� �Ѿ� 40 ------- \n
 *
 * KAROS \n
 * RogerLR220 \n
 *
 * ------- ��ź�� : �ִ� �Ѿ� 10 ------- \n
 *
 * DOUBLE_BARREL \n
 * SK12 \n
 *
 * ------- SMG : �ִ� �Ѿ� 50 ------- \n
 *
 * EON \n
 * PX_70 \n
 *
 * ------- ������ : �ִ� �Ѿ� 5 ------- \n
 *
 * AYAKASHI \n
 * VSS \n
 *
 * ------- ��Ÿ : �ִ� �Ѿ� ���� ------- \n
 *
 * RPG_17 / �Ѿ� ���� : 5 \n
 * CROSSBOW / �Ѿ� ���� : 10 \n
 */
UENUM()
enum class EWeaponType
{
    C7,
    LS80,
    M1,
    KAROS,
    ROGER_LR220,
    DOUBLE_BARREL,
    SK12,
    EON,
    PX_70,
    AYAKASHI,
    VSS,
    RPG_17,
    CROSSBOW,
    MAX
};

/**
 * \brief ��ô�� ���� \n
 *
 * ------- �Ϲ� ����ź : ������ ���� / ���� 100 / ���� �ð� 0 ------- \n
 *
 * Grenade_fragmentation1 / ������ : 50
 * Grenade_fragmentation2 / ������ : 100
 * Grenade_stick          / ������ : 75
 *
 * ------- Ư�� ����ź : ������ 0 / ���� ���� / ���� �ð� ���� ------- \n
 *
 * Grenade_illumination / ���� : 100, ���� �ð� : 3�� \n
 * Grenade_gray_smoke   / ���� : 100, ���� �ð� : 5�� \n
 * Grenade_red_smoke    / ���� : 100, ���� �ð� : 7�� \n
 *
 * ------- ������ ����ź : ������ ���� / ���� ���� / ���� �ð� ���� ------- \n
 *
 * Molotov /  ������ : 5,   ���� : 100, ���� �ð� : 10��
 * Claymore / ������ : 200, ���� : 200
 */
UENUM()
enum class EThrowableWeaponType
{
    FRAGMENTATION1,
    FRAGMENTATION2,
    ILLUMINATION,
    GRAY_SMOKE,
    RED_SMOKE,
    STICK,
    MOLOTOV,
    CLAYMORE,
    MAX
};

/**
  * \brief ���� ���� ���� \n
  * PAN �Ķ����� \n
  * KNIFE Į \n
 */
UENUM()
enum class EMeleeWeaponType
{
    PAN = (int)EThrowableWeaponType::MAX,
    KNIFE,
    MAX
};

/** \brief �� �ݹ� ���� \n
 *
 * SINGLE      �ܹ� \n
 * BURST       ���� \n
 * CONSECUTIVE ���� \n
 */
UENUM()
enum class EGunShootType
{
    SINGLE,
    BURST,
    CONSECUTIVE,
    MAX
};