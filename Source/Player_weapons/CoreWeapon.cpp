#include "CoreWeapon.h"
#include "CoreBullet.h"
#include "PUBG_UE4/DataTableManager.h"
#include "PUBG_UE4/SoundManager.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundBase.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
#include "Particles/ParticleSystemComponent.h"

ACoreWeapon::ACoreWeapon()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACoreWeapon::BeginPlay()
{
    Super::BeginPlay();
    ABaseInteraction::SetCollisionSettingsForObjects();
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

    ABaseInteraction::AttachComponents();
    Super::InitParticleSystem("/Game/VFX/FXVarietyPack/Particles/P_ky_shotShockwave.P_ky_shotShockwave");
    InitMesh();
    InitBullet();
    UpdateCollider();
    UpdateParticleSystem();
}

void ACoreWeapon::InitMesh()
{
    ABaseInteraction::InitSkeletalMesh(WeaponData.MeshPath);
    SkeletalMeshComp->SetRelativeRotation(FRotator::ZeroRotator);
    SkeletalMeshComp->SetRelativeLocation(FVector::ZeroVector);
}

void ACoreWeapon::InitBullet()
{
    ConstructorHelpers::FClassFinder<AActor> BP_BULLET(*(WeaponData.BulletBP_path));

    if (BP_BULLET.Succeeded())
    {
        auto p_bp_bullet = BP_BULLET.Class;
        pBullet          = p_bp_bullet->GetDefaultObject<ACoreBullet>();
    }
}

void ACoreWeapon::UpdateCollider()
{
    if (!ColliderComp)
        return;

    ColliderComp->AddLocalOffset(WeaponData.ColliderPos);
    ColliderComp->SetBoxExtent(WeaponData.ColliderSize);
    ColliderComp->AddRelativeLocation(FVector(0.f, 0.f, 8.f));
}

void ACoreWeapon::UpdateParticleSystem()
{
    if (!ParticleComp)
        return;

    FVector startPos = SkeletalMeshComp->GetSocketLocation(TEXT("MuzzleSock"));
    FVector scaleValue{ 0.05f };

    ParticleComp->SetWorldScale3D(scaleValue);
    ParticleComp->SetWorldLocation(startPos);
    ParticleComp->AddWorldRotation(FQuat::MakeFromEuler(FVector(0.f, 90.f, 0.f)));
}