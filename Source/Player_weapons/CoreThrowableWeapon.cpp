#include "CoreThrowableWeapon.h"
#include "PUBG_UE4/DataTableManager.h"
#include "PUBG_UE4/SoundManager.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundBase.h"
#include "Materials/Material.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include <Runtime/Engine/Public/DrawDebugHelpers.h>

ACoreThrowableWeapon::ACoreThrowableWeapon()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACoreThrowableWeapon::BeginPlay()
{
    Super::BeginPlay();

    if (GrenadeColliderComp)
    {
        auto component_location = GrenadeColliderComp->GetComponentLocation();
        GrenadeColliderComp->SetRelativeLocation(FVector(component_location.X, component_location.Y, WeaponData.ColliderPosZ));
        //GrenadeColliderComp->AddRelativeLocation(FVector(0.f, 0.f, WeaponData.ColliderPosZ));
        GrenadeColliderComp->AddRelativeRotation(FRotator::MakeFromEuler(FVector(FMath::Abs(WeaponData.MeshRotationX), 0.f, 0.f)));
        GrenadeColliderComp->SetCollisionProfileName("Object");
    }
    if(ColliderComp)
    {
        ABaseInteraction::SetCollisionSettingsForObjects();
        ColliderComp->AddRelativeLocation(FVector(0.f, 0.f, 4.f));
        ColliderComp->SetBoxExtent(FVector(15.f, 15.f, 5.f));
        StaticMeshComp->AddRelativeLocation(FVector(0.f, 0.f, 18.f));
    }
}
    
void ACoreThrowableWeapon::Tick(float _DeltaTime)
{
    Super::Tick(_DeltaTime);
}

void ACoreThrowableWeapon::Init(EThrowableWeaponType _WeaponType)
{
    WeaponData = ADataTableManager::ArrOtherWeaponData[(int)_WeaponType];
    WeaponType = _WeaponType;
    ObjectType = WeaponData.Type;
    ObjectGroupType = WeaponData.GroupType;

    UpdateCollider();
    InitProjectileMovementComp();
    Super::AttachComponents();
    Super::InitParticleSystem(WeaponData.ParticlePath);
    InitMesh();

    if (SceneComp)
        SceneComp->DestroyComponent();
}

void ACoreThrowableWeapon::InitProjectileMovementComp()
{
    if (WeaponType == EThrowableWeaponType::CLAYMORE)
        return;

    ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
    ProjectileMovementComp->bShouldBounce = true;
    ProjectileMovementComp->InitialSpeed  = 100.f;
    ProjectileMovementComp->MaxSpeed      = 100.f;
}

void ACoreThrowableWeapon::InitMesh()
{
    ABaseInteraction::InitStaticMesh(WeaponData.MeshPath);

    if (StaticMeshComp)
    {
        StaticMeshComp->SetRelativeLocation(WeaponData.MeshPos);
        StaticMeshComp->SetRelativeRotation(FRotator::MakeFromEuler(FVector(WeaponData.MeshRotationX, 0.f, 0.f)));
        StaticMeshComp->SetRelativeScale3D(FVector(WeaponData.MeshSize));
        StaticMeshComp->SetCollisionProfileName("NoCollision");
        StaticMeshComp->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
    }
}

void ACoreThrowableWeapon::UpdateCollider()
{
    if (WeaponType != EThrowableWeaponType::CLAYMORE)
    {
        GrenadeColliderComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));

        if (GrenadeColliderComp)
        {
            RootComponent->DestroyComponent();
            GrenadeColliderComp->SetCapsuleRadius(0.f);
            GrenadeColliderComp->SetCapsuleHalfHeight(0.f);
            GrenadeColliderComp->SetCapsuleHalfHeight(WeaponData.ColliderHeight);
            GrenadeColliderComp->SetCapsuleRadius(WeaponData.ColliderSize);
            RootComponent = GrenadeColliderComp;
        }
    }
}