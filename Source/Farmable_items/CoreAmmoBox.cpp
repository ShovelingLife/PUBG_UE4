#include "CoreAmmoBox.h"
#include "PUBG_UE4/DataTableManager.h"

void ACoreAmmoBox::Init(EAmmoBoxType Type)
{
    AmmoBoxType        = Type;
    WeaponBulletData   = ADataTableManager::ArrWeaponBulletData[(int)Type];
    FString bulletType = "cal" + WeaponBulletData.Type;
    FString meshPath   = "/Game/AmmoSet/Models/" + bulletType + "/" + bulletType + "_" + WeaponBulletData.MeshType;
    ABaseInteraction::InitStaticMesh(meshPath);
}