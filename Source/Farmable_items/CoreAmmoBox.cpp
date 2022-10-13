#include "CoreAmmoBox.h"
#include "PUBG_UE4/DataTableManager.h"

void ACoreAmmoBox::Init(EAmmoBoxType Type)
{
    ObjectGroupType = "AmmoBox";
    AmmoBoxType        = Type;
    WeaponBulletData   = ADataTableManager::GetWeaponBulletData((int)Type);
    FString bulletType = "cal" + WeaponBulletData.Type;
    FString meshPath   = "/Game/2_Meshes/AmmoBox/Models/" + bulletType + "/" + bulletType + "_" + WeaponBulletData.MeshType;
    ABaseInteraction::InitStaticMesh(meshPath);
}