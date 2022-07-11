#include "CoreAttachment.h"
#include "PUBG_UE4/DataTableManager.h"
#include "Components/BoxComponent.h"

void ACoreAttachment::Init(EWeaponAttachmentType Type)
{
    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
    this->WeaponAttachmentType = Type;
    WeaponAttachmentData = ADataTableManager::ArrWeaponAttachmentData[(int)Type];
    ABaseInteraction::InitStaticMesh(WeaponAttachmentData.MeshPath);
    ABaseInteraction::ChangeCollisionSettings(true);
    SetRootComponent(StaticMeshComp);
    ABaseInteraction::AttachComponents();
}