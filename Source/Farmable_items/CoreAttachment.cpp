#include "CoreAttachment.h"
#include "PUBG_UE4/DataTableManager.h"
#include "Components/BoxComponent.h"

void ACoreAttachment::Init(EWeaponAttachmentType Type)
{
    this->WeaponAttachmentType = Type;
    WeaponAttachmentData = ADataTableManager::ArrWeaponAttachmentData[(int)Type];
    ABaseInteraction::InitStaticMesh(WeaponAttachmentData.MeshPath);
    ABaseInteraction::ChangeCollisionSettings(true);
    SetRootComponent(StaticMeshComp);

    if (ColliderComp)
        ColliderComp->DestroyComponent();

    ABaseInteraction::AttachComponents();
}