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
    // �ѱ��� ��
    if (auto p_gun = Cast<ACoreWeapon>(pObj))
    {
        FsWeaponData data = p_gun->WeaponData;
        return FsSlotItemData(data.Type, "Gun", data.Description, (int)p_gun->WeaponType);
    }
    // ���� ������ ��
    else if (auto p_melee = Cast<ACoreMeleeWeapon>(pObj))
    {
        FsOtherWeaponData data = p_melee->WeaponData;
        return FsSlotItemData(data.Type, "Melee", data.Description, (int)p_melee->WeaponType);
    }
    // ��ô���� ��
    else if (auto p_throwable = Cast<ACoreThrowableWeapon>(pObj))
    {
        FsOtherWeaponData data = p_throwable->WeaponData;
        return FsSlotItemData(data.Type, "Throwable", data.Description, (int)p_throwable->WeaponType);
    }
    // ���� �μ�ǰ�� ��
    else if(auto p_weaponAttachment = Cast<ACoreAttachment>(pObj))
    {
        FsWeaponAttachmentData data = p_weaponAttachment->WeaponAttachmentData;
        return FsSlotItemData(data.Type, "Attachment", data.Description, (int)p_weaponAttachment->WeaponAttachmentType);
    }
    // �Ѿ� �ڽ��� ��
    else if(auto p_ammoBox = Cast<ACoreAmmoBox>(pObj))
    {
        FsWeaponBulletData data = p_ammoBox->WeaponBulletData;
        return FsSlotItemData(data.Type, "AmmoBox", data.Description, (int)p_ammoBox->AmmoBoxType, 0, data.Count);
    }
    else
        return EmptyData;
}