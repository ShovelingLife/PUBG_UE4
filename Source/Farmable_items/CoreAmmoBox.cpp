#include "CoreAmmoBox.h"
#include "PUBG_UE4/DataTableManager.h"

void ACoreAmmoBox::Init(EAmmoBoxType Type)
{
    auto weaponBulletData = ADataTableManager::ArrWeaponBulletData[(int)Type];
    FString bulletType = "cal" + weaponBulletData.Type;
    FString meshPath = "/Game/AmmoSet/Models/" + bulletType + "/" + bulletType + "_" + weaponBulletData.MeshType;
    AmmoBoxType = Type;
    ABaseInteraction::InitStaticMesh(meshPath);
}