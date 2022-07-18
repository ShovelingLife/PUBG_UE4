#include "WeaponEnum.h"
#include "CoreWeapon.h"

EGunShootType GetNextShootType(ACoreWeapon* pWeapon)
{
    int currentType = static_cast<int>(pWeapon->ShootType);
    int maxType     = static_cast<int>(GetMaxShootType(pWeapon->WeaponData.GroupType));

    if (currentType != maxType)
    {
        switch (currentType)
        {
        case EGunShootType::SINGLE: return EGunShootType::BURST;
        case EGunShootType::BURST:  return EGunShootType::CONSECUTIVE;
        }
    }
    return EGunShootType::SINGLE;
}

EGunShootType GetMaxShootType(FString WeaponGroup)
{
    TMap<FString, EGunShootType> mapGunShootType
    {
        // ------- 오직 단발 -------
        {"HandGun"  , EGunShootType::SINGLE},
        {"SniperGun", EGunShootType::SINGLE},
        {"Special"  , EGunShootType::SINGLE},

        // ------- 단발 / 점사 -------
        {"ShotGun", EGunShootType::BURST},

        // ------- 단발 / 점사 / 연사 -------
        {"AssaultGun", EGunShootType::CONSECUTIVE},
        {"SMG"       , EGunShootType::CONSECUTIVE}
    };
    return mapGunShootType[WeaponGroup];
}

FString GetShootTypeStr(EGunShootType Type)
{
    if (Type == EGunShootType::MAX)
        return "Fail";

    TMap<EGunShootType, FString> mapGunShootType
    {
        { EGunShootType::SINGLE,      "Single" },
        { EGunShootType::BURST,       "Burst" },
        { EGunShootType::CONSECUTIVE, "Consecutive" }
    };
    return "Current type : " + mapGunShootType[Type];
} 