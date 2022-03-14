#include "SlotItemData.h"
#include "Player_weapons/CoreMeleeWeapon.h"
#include "Player_weapons/CoreThrowableWeapon.h"
#include "Player_weapons/CoreWeapon.h"
#include "PUBG_UE4/WeaponData.h"
#include "PUBG_UE4/OtherWeaponData.h"
#include "PUBG_UE4/BaseInteraction.h"

const FsSlotItemData FsSlotItemData::EmptyData;

FsSlotItemData FsSlotItemData::GetDataFrom(ABaseInteraction* _pObj)
{
    if (!_pObj)
        return FsSlotItemData::EmptyData;

    // √—±‚¿œ Ω√
    if (auto p_gun = Cast<ACoreWeapon>(_pObj))
    {
        FsWeaponData data = p_gun->WeaponData;
        return FsSlotItemData(data.Type, data.GroupType, data.Description, (int)p_gun->WeaponType);
    }
    else if (auto p_melee = Cast<ACoreMeleeWeapon>(_pObj))
    {
        FsOtherWeaponData data = p_melee->WeaponData;
        return FsSlotItemData(data.Type, data.GroupType, data.Description, (int)p_melee->WeaponType);
    }
    else if (auto p_throwable = Cast<ACoreThrowableWeapon>(_pObj))
    {
        FsOtherWeaponData data = p_throwable->WeaponData;
        return FsSlotItemData(data.Type, data.GroupType, data.Description, (int)p_throwable->WeaponType);
    }
    else
        return EmptyData;
}