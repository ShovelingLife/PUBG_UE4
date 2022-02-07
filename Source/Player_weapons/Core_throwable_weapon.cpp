#include "Core_throwable_weapon.h"
#include "PUBG_UE4/Data_table_manager.h"
#include "PUBG_UE4/Sound_manager.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundBase.h"
#include "Materials/Material.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include <Runtime/Engine/Public/DrawDebugHelpers.h>

ACore_throwable_weapon::ACore_throwable_weapon()
{
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACore_throwable_weapon::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ACore_throwable_weapon::Tick(float _delta_time)
{
    Super::Tick(_delta_time);
}

void ACore_throwable_weapon::Init(e_throwable_weapon_type _weapon_type)
{
    weapon_data   = AData_table_manager::arr_other_weapon_data[(int)_weapon_type];
    weapon_type   = _weapon_type;
    object_type = weapon_data.type;

    ABase_interaction::Init_audio();
    Update_collider();
    Init_mesh();
    Init_particle_system();
}

void ACore_throwable_weapon::Init_mesh()
{
    FName name = TEXT("Weapon_mesh");
    ABase_interaction::Init_static_mesh(weapon_data.mesh_path, name);
    p_static_mesh_comp->SetRelativeRotation(FRotator::ZeroRotator);
    p_static_mesh_comp->SetRelativeLocation(FVector::ZeroVector);
}

void ACore_throwable_weapon::Update_collider()
{
    m_box_collider->AddLocalOffset(weapon_data.collider_pos);
    m_box_collider->SetBoxExtent(weapon_data.collider_size);
    m_box_collider->AddRelativeLocation(FVector(0.f, 0.f, 8.f));
}

void ACore_throwable_weapon::Init_particle_system()
{
    // ÃÑ±â °ü·Ã
    p_grenade_particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
    p_grenade_particle->bAutoActivate = false;
    p_grenade_particle->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}