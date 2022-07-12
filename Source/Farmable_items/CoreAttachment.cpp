#include "CoreAttachment.h"
#include "PUBG_UE4/DataTableManager.h"
#include "Components/BoxComponent.h"

void ACoreAttachment::Init(EWeaponAttachmentType Type)
{
    this->WeaponAttachmentType = Type;
    WeaponAttachmentData = ADataTableManager::ArrWeaponAttachmentData[(int)Type];
    Super::InitStaticMesh(WeaponAttachmentData.MeshPath);
    Super::ChangeCollisionSettings(true);
}