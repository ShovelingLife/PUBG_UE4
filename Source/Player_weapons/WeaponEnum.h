﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponEnum.generated.h"

/** 
 * \brief 현재 착용 중인 무기 \n
 * 
 * FIRST     첫번째 총 \n
 * SECOND    두번째 총 \n
 * PISTOL    권총 \n
 * MELEE     근접류 \n
 * THROWABLE 투척류 \n
 */
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
 * \brief 부속품 종류 \n
 */
enum class EAttachmentType : uint8
{

};

/** 
 * \brief 총기 종류 \n
 * 
 * ------- 소총 : 최대 총알 30 ------- \n
 * 
 * C7 \n
 * LS80 \n
 * M1 \n
 * 
 * ------- 권총 : 최대 총알 40 ------- \n
 * 
 * Karos \n
 * RogerLR220 \n
 * 
 * ------- 산탄총 : 최대 총알 10 ------- \n
 * 
 * Double_barrel \n
 * SK12 \n
 * 
 * ------- SMG : 최대 총알 50 ------- \n
 * 
 * EON \n
 * PX_70 \n
 * 
 * ------- 저격총 : 최대 총알 5 ------- \n
 * 
 * Ayakashi \n
 * VSS \n
 * 
 * ------- 기타 : 최대 총알 상이 ------- \n
 * 
 * RPG_17 / 총알 개수 : 5 \n
 * Crossbow / 총알 개수 : 10 \n
 */
enum class EWeaponType : uint8
{
    C7,
    LS80,
    M1,
    Karos,
    RogerLR220,
    Double_barrel,
    SK12,
    EON,
    PX_70,
    Ayakashi,
    VSS,
    RPG_17,
    Crossbow,
    MAX
};

/**
 * \brief 투척류 종류 \n
 *
 * ------- 일반 수류탄 : 데미지 상이 / 범위 100 / 지속 시간 0 ------- \n
 *
 * Grenade_fragmentation1 / 데미지 : 50
 * Grenade_fragmentation2 / 데미지 : 100
 * Grenade_stick          / 데미지 : 75
 *
 * ------- 특수 수류탄 : 데미지 0 / 범위 상이 / 지속 시간 상이 ------- \n
 *
 * Grenade_illumination / 범위 : 100, 지속 시간 : 3초 \n
 * Grenade_gray_smoke   / 범위 : 100, 지속 시간 : 5초 \n
 * Grenade_red_smoke    / 범위 : 100, 지속 시간 : 7초 \n
 *
 * ------- 광범위 수류탄 : 데미지 상이 / 범위 상이 / 지속 시간 상이 ------- \n
 *
 * Molotov /  데미지 : 5,   범위 : 100, 지속 시간 : 10초
 * Claymore / 데미지 : 200, 범위 : 200
 */
enum class EThrowableWeaponType : uint8
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
  * \brief 근접 무기 종류 \n
  * PAN 후라이팬 \n
  * KNIFE 칼 \n
 */
enum class EMeleeWeaponType : uint8
{
    PAN = (uint8)EThrowableWeaponType::MAX,
    KNIFE,
    MAX
};

/** \brief 총 격발 종류 \n
 * 
 * SINGLE      단발 \n
 * BURST       점사 \n
 * CONSECUTIVE 연사 \n
 */
enum class EGunShootType
{
    SINGLE,
    BURST,
    CONSECUTIVE,
    MAX
};

UCLASS()
class PLAYER_WEAPONS_API AWeaponEnum : public AActor
{
	GENERATED_BODY()
};