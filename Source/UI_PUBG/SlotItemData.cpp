#include "SlotItemData.h"
#include "Farmable_items/CoreAttachment.h"
#include "Farmable_items/CoreAmmoBox.h"
#include "Player_weapons/CoreMeleeWeapon.h"
#include "Player_weapons/CoreThrowableWeapon.h"
#include "Player_weapons/CoreWeapon.h"
#include "PUBG_UE4/WeaponData.h"
#include "PUBG_UE4/OtherWeaponData.h"
#include "PUBG_UE4/BaseInteraction.h"

const FsSlotItemData FsSlotItemData::EmptyData;

FsSlotItemData FsSlotItemData::GetDataFrom(ABaseInteraction* pObj)
{
    // 총기일 시
    if (auto p_gun = Cast<ACoreWeapon>(pObj))
    {
        FsWeaponData data = p_gun->WeaponData;
        return FsSlotItemData(data.Type, "Gun", data.Description, (int)p_gun->WeaponType);
    }
    // 근접 무기일 시
    else if (auto p_melee = Cast<ACoreMeleeWeapon>(pObj))
    {
        FsOtherWeaponData data = p_melee->WeaponData;
        return FsSlotItemData(data.Type, "Melee", data.Description, (int)p_melee->WeaponType);
    }
    // 투척류일 시
    else if (auto p_throwable = Cast<ACoreThrowableWeapon>(pObj))
    {
        FsOtherWeaponData data = p_throwable->WeaponData;
        return FsSlotItemData(data.Type, "Throwable", data.Description, (int)p_throwable->WeaponType);
    }
    // 무기 부속품일 시
    else if(auto p_weaponAttachment = Cast<ACoreAttachment>(pObj))
    {
        FsWeaponAttachmentData data = p_weaponAttachment->WeaponAttachmentData;
        return FsSlotItemData(data.Type, "Attachment", data.Description, (int)p_weaponAttachment->WeaponAttachmentType);
    }
    // 총알 박스일 시
    else if(auto p_ammoBox = Cast<ACoreAmmoBox>(pObj))
    {
        FsWeaponBulletData data = p_ammoBox->WeaponBulletData;
        return FsSlotItemData(data.Type, "AmmoBox", data.Description, (int)p_ammoBox->AmmoBoxType, 0, data.Count);
    }
    else
        return EmptyData;
}