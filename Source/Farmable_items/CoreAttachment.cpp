#include "CoreAttachment.h"
#include "PUBG_UE4/DataTableManager.h"
#include "Components/BoxComponent.h"

void ACoreAttachment::Init(EWeaponAttachmentType Type)
{
    if (ColliderComp)
        ColliderComp->DestroyComponent();

    this->WeaponAttachmentType = Type;
    WeaponAttachmentData = ADataTableManager::ArrWeaponAttachmentData[(int)Type];
    ABaseInteraction::InitStaticMesh(WeaponAttachmentData.MeshPath);
    ABaseInteraction::ChangeCollisionSettings(true);
    SetRootComponent(StaticMeshComp);
    ABaseInteraction::AttachComponents();
}