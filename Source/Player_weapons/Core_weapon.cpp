#include "Core_weapon.h"
#include "Core_bullet.h"
#include "PUBG_UE4/Data_table_manager.h"
#include "PUBG_UE4/Sound_manager.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundBase.h"
#include "Materials/Material.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include <Runtime/Engine/Public/DrawDebugHelpers.h>

ACore_weapon::ACore_weapon()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACore_weapon::BeginPlay()
{
    Super::BeginPlay();
    Update_particle_system();
    weapon_data.current_bullet_count = 100;
}

void ACore_weapon::NotifyActorBeginOverlap(AActor* _collided_actor)
{
    ABase_interaction::NotifyActorBeginOverlap(_collided_actor);
}

void ACore_weapon::NotifyActorEndOverlap(AActor* _collided_actor)
{
    ABase_interaction::NotifyActorEndOverlap(_collided_actor);
}

void ACore_weapon::Tick(float _delta_time)
{
    Super::Tick(_delta_time);
}

void ACore_weapon::Init(e_weapon_type _weapon_type)
{
    weapon_data   = AData_table_manager::arr_weapon_data[(int)_weapon_type];
    weapon_type   = _weapon_type;
    object_type = weapon_data.type;

    ABase_interaction::Init_audio();
    Update_collider();
    Init_mesh();
    Init_bullet();
    Init_particle_system();
}

void ACore_weapon::Init_mesh()
{
    FName name = TEXT("Weapon_mesh");
    ABase_interaction::Init_skeletal_mesh(weapon_data.mesh_path, name);
    p_skeletal_mesh_comp->SetRelativeRotation(FRotator::ZeroRotator);
    p_skeletal_mesh_comp->SetRelativeLocation(FVector::ZeroVector);
}

void ACore_weapon::Update_collider()
{
    m_box_collider->AddLocalOffset(weapon_data.collider_pos);
    m_box_collider->SetBoxExtent(weapon_data.collider_size);
    m_box_collider->AddRelativeLocation(FVector(0.f, 0.f, 8.f));
}

void ACore_weapon::Init_bullet()
{
    ConstructorHelpers::FClassFinder<AActor> BP_BULLET(*(weapon_data.bullet_bp_path));

    if (BP_BULLET.Succeeded())
    {
        auto p_bp_bullet = BP_BULLET.Class;
        p_bullet         = p_bp_bullet->GetDefaultObject<ACore_bullet>();
    }
}

void ACore_weapon::Init_particle_system()
{
    // 총기 관련
    p_gun_particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
    p_gun_particle->bAutoActivate = false;
    static ConstructorHelpers::FObjectFinder<UParticleSystem> GUN_PARTICLE(TEXT("/Game/VFX/FXVarietyPack/Particles/P_ky_shotShockwave.P_ky_shotShockwave"));

    if (GUN_PARTICLE.Succeeded())
        p_gun_particle->SetTemplate(GUN_PARTICLE.Object);
}

void ACore_weapon::Update_particle_system()
{
    if (!p_gun_particle)
        return;

    FVector start_pos = p_skeletal_mesh_comp->GetSocketLocation(TEXT("Muzzle_socket"));
    FVector scale_value{ 0.05f };

    p_gun_particle->SetWorldScale3D(scale_value);
    p_gun_particle->SetWorldLocation(start_pos);
    p_gun_particle->AddWorldRotation(FQuat::MakeFromEuler(FVector(0.f, 90.f, 0.f)));
}