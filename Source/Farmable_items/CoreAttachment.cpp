#include "CoreAttachment.h"
#include "PUBG_UE4/DataTableManager.h"
#include "Components/BoxComponent.h"

void ACoreAttachment::Init(EWeaponAttachmentType Type)
{
    this->WeaponAttachmentType = Type;
    WeaponAttachmentData = ADataTableManager::GetWeaponAttachmentData((int)Type);
    Super::InitStaticMesh(WeaponAttachmentData.MeshPath);
    Super::ChangeCollisionSettings();
}