#include "CoreWeapon.h"
#include "CoreBullet.h"
#include "PUBG_UE4/DataTableManager.h"
#include "PUBG_UE4/SoundManager.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundBase.h"
#include "Materials/Material.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include <Runtime/Engine/Public/DrawDebugHelpers.h>

ACoreWeapon::ACoreWeapon()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACoreWeapon::BeginPlay()
{
    Super::BeginPlay();
    UpdateParticleSystem();
}

void ACoreWeapon::NotifyActorBeginOverlap(AActor* CollidedActor)
{
    ABaseInteraction::NotifyActorBeginOverlap(CollidedActor);
}

void ACoreWeapon::NotifyActorEndOverlap(AActor* ColliderActor)
{
    ABaseInteraction::NotifyActorEndOverlap(ColliderActor);
}

void ACoreWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACoreWeapon::Init(EWeaponType _WeaponType)
{
    WeaponData = ADataTableManager::ArrWeaponData[(int)_WeaponType];
    WeaponType = _WeaponType;
    ObjectType = WeaponData.Type;
    ObjectGroupType = WeaponData.GroupType;

    ABaseInteraction::InitAudio();
    UpdateCollider();
    InitMesh();
    InitBullet();
    InitParticleSystem();
}

void ACoreWeapon::InitMesh()
{
    FName name = TEXT("Weapon_mesh");
    ABaseInteraction::InitSkeletalMesh(WeaponData.MeshPath, name);
    SkeletalMeshComp->SetRelativeRotation(FRotator::ZeroRotator);
    SkeletalMeshComp->SetRelativeLocation(FVector::ZeroVector);
}

void ACoreWeapon::UpdateCollider()
{
    ColliderComp->AddLocalOffset(WeaponData.ColliderPos);
    ColliderComp->SetBoxExtent(WeaponData.ColliderSize);
    ColliderComp->AddRelativeLocation(FVector(0.f, 0.f, 8.f));
}

void ACoreWeapon::InitBullet()
{
    ConstructorHelpers::FClassFinder<AActor> BP_BULLET(*(WeaponData.BulletBP_path));

    if (BP_BULLET.Succeeded())
    {
        auto p_bp_bullet = BP_BULLET.Class;
        pBullet         = p_bp_bullet->GetDefaultObject<ACoreBullet>();
    }
}

void ACoreWeapon::InitParticleSystem()
{
    // 총기 관련
    ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
    ParticleComp->bAutoActivate = false;
    static ConstructorHelpers::FObjectFinder<UParticleSystem> GUN_PARTICLE(TEXT("/Game/VFX/FXVarietyPack/Particles/P_ky_shotShockwave.P_ky_shotShockwave"));

    if (GUN_PARTICLE.Succeeded())
        ParticleComp->SetTemplate(GUN_PARTICLE.Object);
}

void ACoreWeapon::UpdateParticleSystem()
{
    if (!ParticleComp)
        return;

    FVector start_pos = SkeletalMeshComp->GetSocketLocation(TEXT("Muzzle_socket"));
    FVector scale_value{ 0.05f };

    ParticleComp->SetWorldScale3D(scale_value);
    ParticleComp->SetWorldLocation(start_pos);
    ParticleComp->AddWorldRotation(FQuat::MakeFromEuler(FVector(0.f, 90.f, 0.f)));
}