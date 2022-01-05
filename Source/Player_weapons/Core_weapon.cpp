#include "Core_weapon.h"
#include "Core_bullet.h"
#include "PUBG_UE4/Global.h"
#include "PUBG_UE4/Data_table_manager.h"
#include "PUBG_UE4/Custom_game_instance.h"
#include "Components/AudioComponent.h"
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

// Called when the game starts or when spawned
void ACore_weapon::BeginPlay()
{
    Super::BeginPlay();
    Update_particle_system();
}

void ACore_weapon::NotifyActorBeginOverlap(AActor* _collided_actor)
{
    ABase_interaction::NotifyActorBeginOverlap(_collided_actor);
}

void ACore_weapon::NotifyActorEndOverlap(AActor* _collided_actor)
{
    ABase_interaction::NotifyActorEndOverlap(_collided_actor);
}

// Called every frame
void ACore_weapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACore_weapon::Init(e_weapon_type _index)
{    
    weapon_data = AGlobal::Get_data_table_manager()->Get_weapon_data((int)_index);
    m_weapon_type = _index;
    m_object_type = weapon_data.type;

    Update_collider();
    Init_mesh();
    Init_audio();

    if (m_is_gun)
    {
        Init_bullet();
        Init_UI_material();
        Init_particle_system();
    }
}

void ACore_weapon::Init_mesh()
{
    FName name = TEXT("Weapon_mesh");
    ABase_interaction::Init_skeletal_mesh(weapon_data.mesh_path, name);
    skeletal_mesh->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
    skeletal_mesh->SetRelativeLocation(FVector::ZeroVector);
}

void ACore_weapon::Update_collider()
{
    m_box_collider->AddLocalOffset(weapon_data.collider_pos);
    m_box_collider->SetBoxExtent(weapon_data.collider_size);
}

void ACore_weapon::Init_bullet()
{
    ConstructorHelpers::FClassFinder<AActor> BP_BULLET(*(weapon_data.bullet_bp_path));

    if (BP_BULLET.Succeeded())
    {
        auto p_bp_bullet = BP_BULLET.Class;
        p_bullet = p_bp_bullet->GetDefaultObject<ACore_bullet>();
    }
}

void ACore_weapon::Init_audio()
{
    // 오디오 포인터에 대한 생성
    mp_audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
    mp_audio->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void ACore_weapon::Init_UI_material()
{
    ConstructorHelpers::FObjectFinder< UMaterial> RENDERTARGET_MESH(*(weapon_data.UI_material_bp_path));

    if (RENDERTARGET_MESH.Succeeded())
        p_render_target_ui_mesh = RENDERTARGET_MESH.Object;
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

    FVector start_pos = skeletal_mesh->GetSocketLocation(TEXT("Muzzle_socket"));
    FVector scale_value{ 0.05f };

    p_gun_particle->SetWorldScale3D(scale_value);
    p_gun_particle->SetWorldLocation(start_pos);
    p_gun_particle->AddWorldRotation(FQuat::MakeFromEuler(FVector(0.f, 90.f, 0.f)));
}

void ACore_weapon::Interact()
{
}

void ACore_weapon::Play_sound(e_weapon_sound_type _sound_type)
{
    /*USoundBase*          tmp_sound            = nullptr;

    switch (_sound_type)
    {
    case e_weapon_sound_type::EMPTY_AMMO:   tmp_sound = p_data_table_manager->p_empty_ammo_sound;               break;
    case e_weapon_sound_type::BULLET_SOUND: tmp_sound = p_data_table_manager->p_shot_sound_arr[(int)m_weapon_type]; break;
    case e_weapon_sound_type::RELOAD_SOUND: tmp_sound = p_data_table_manager->p_reload_sound;                   break;
    }
    mp_audio->SetSound(tmp_sound);
    mp_audio->Play();*/
}