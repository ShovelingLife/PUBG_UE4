#include "SlotItemData.h"
#include "Farmable_items/CoreAttachment.h"
#include "Player_weapons/CoreMeleeWeapon.h"
#include "Player_weapons/CoreThrowableWeapon.h"
#include "Player_weapons/CoreWeapon.h"
#include "PUBG_UE4/WeaponData.h"
#include "PUBG_UE4/OtherWeaponData.h"
#include "PUBG_UE4/BaseInteraction.h"

const FsSlotItemData FsSlotItemData::EmptyData;

FsSlotItemData FsSlotItemData::GetDataFrom(ABaseInteraction* pObj)
{
    // 醚扁老 矫
    if (auto p_gun = Cast<ACoreWeapon>(pObj))
    {
        FsWeaponData data = p_gun->WeaponData;
        return FsSlotItemData(data.Type, "Gun", data.Description, (int)p_gun->WeaponType);
    }
    // 辟立 公扁老 矫
    else if (auto p_melee = Cast<ACoreMeleeWeapon>(pObj))
    {
        FsOtherWeaponData data = p_melee->WeaponData;
        return FsSlotItemData(data.Type, "Melee", data.Description, (int)p_melee->WeaponType);
    }
    // 捧么幅老 矫
    else if (auto p_throwable = Cast<ACoreThrowableWeapon>(pObj))
    {
        FsOtherWeaponData data = p_throwable->WeaponData;
        return FsSlotItemData(data.Type, "Throwable", data.Description, (int)p_throwable->WeaponType);
    }
    // 公扁 何加前老 矫
    else if(auto p_weaponAttachment = Cast<ACoreAttachment>(pObj))
    {
        FsWeaponAttachmentData data = p_weaponAttachment->WeaponAttachmentData;
        return FsSlotItemData(data.Type, "Attachment", data.Description, (int)p_weaponAttachment->WeaponAttachmentType);
    }
    else
        return EmptyData;
}