#include "SlotItemData.h"
#include "Player_weapons/CoreMeleeWeapon.h"
#include "Player_weapons/CoreThrowableWeapon.h"
#include "Player_weapons/CoreWeapon.h"
#include "PUBG_UE4/WeaponData.h"
#include "PUBG_UE4/OtherWeaponData.h"
#include "PUBG_UE4/BaseInteraction.h"

const FsSlotItemData FsSlotItemData::EmptyData;

FsSlotItemData FsSlotItemData::GetDataFrom(ABaseInteraction* pObj)
{
    if (!pObj)
        return FsSlotItemData::EmptyData;

    // √—±‚¿œ Ω√
    if (auto p_gun = Cast<ACoreWeapon>(pObj))
    {
        FsWeaponData data = p_gun->WeaponData;
        return FsSlotItemData(data.Type, data.GroupType, data.Description, (int)p_gun->WeaponType);
    }
    else if (auto p_melee = Cast<ACoreMeleeWeapon>(pObj))
    {
        FsOtherWeaponData data = p_melee->WeaponData;
        return FsSlotItemData(data.Type, data.GroupType, data.Description, (int)p_melee->CurrentWeaponType);
    }
    else if (auto p_throwable = Cast<ACoreThrowableWeapon>(pObj))
    {
        FsOtherWeaponData data = p_throwable->WeaponData;
        return FsSlotItemData(data.Type, data.GroupType, data.Description, (int)p_throwable->CurrentWeaponType);
    }
    else
        return EmptyData;
}