#include "Core_melee_weapon.h"
#include "PUBG_UE4/Data_table_manager.h"
#include "PUBG_UE4/Global.h"
#include "PUBG_UE4/Sound_manager.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundBase.h"
#include "Materials/Material.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include <Runtime/Engine/Public/DrawDebugHelpers.h>

ACore_melee_weapon::ACore_melee_weapon()
{
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACore_melee_weapon::BeginPlay()
{
    Super::BeginPlay();
}

void ACore_melee_weapon::Init(e_melee_weapon_type _weapon_type)
{
    weapon_data   = AData_table_manager::arr_other_weapon_data[(int)_weapon_type];
    weapon_type   = _weapon_type;
    object_type = weapon_data.type;

    ABase_interaction::Init_audio();
    Update_collider();
    Init_mesh();
}

void ACore_melee_weapon::Init_mesh()
{
    FName name = TEXT("Weapon_mesh");
    ABase_interaction::Init_static_mesh(weapon_data.mesh_path, name);
    p_static_mesh_comp->SetRelativeRotation(FRotator::ZeroRotator);
    p_static_mesh_comp->SetRelativeLocation(FVector::ZeroVector);
}

void ACore_melee_weapon::Update_collider()
{
    m_box_collider->AddLocalOffset(weapon_data.collider_pos);
    m_box_collider->SetBoxExtent(weapon_data.collider_size);
    m_box_collider->AddRelativeLocation(FVector(0.f, 0.f, 8.f));
}