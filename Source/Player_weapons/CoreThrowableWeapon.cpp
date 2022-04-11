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

void ACoreThrowableWeapon::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

    if (!bTouched)
    {
        StaticMeshComp->SetSimulatePhysics(false);
        bTouched = true;
    }
}

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
    /*if(ColliderComp)
    {
        ABaseInteraction::SetCollisionSettingsForObjects();
        ColliderComp->AddRelativeLocation(FVector(0.f, 0.f, 4.f));
        ColliderComp->SetBoxExtent(FVector(15.f, 15.f, 5.f));
        StaticMeshComp->AddRelativeLocation(FVector(0.f, 0.f, 18.f));
    }*/
}
    
void ACoreThrowableWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACoreThrowableWeapon::Init(EThrowableWeaponType WeaponType)
{
    WeaponData = ADataTableManager::ArrOtherWeaponData[(int)WeaponType];
    this->CurrentWeaponType = WeaponType;
    ObjectType = WeaponData.Type;
    ObjectGroupType = WeaponData.GroupType;

    InitMesh();
    //UpdateCollider();
    InitProjectileMovementComp();
    Super::AttachComponents();
    Super::InitParticleSystem(WeaponData.ParticlePath);

    if (SceneComp)
        SceneComp->DestroyComponent();
}

void ACoreThrowableWeapon::InitProjectileMovementComp()
{
    if (CurrentWeaponType == EThrowableWeaponType::CLAYMORE)
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
        RootComponent->DestroyComponent();
        RootComponent = StaticMeshComp;

        // 위치 관련
        StaticMeshComp->SetRelativeLocation(WeaponData.MeshPos);
        StaticMeshComp->SetRelativeRotation(FRotator::MakeFromEuler(FVector(WeaponData.MeshRotationX, 0.f, 0.f)));
        StaticMeshComp->SetRelativeScale3D(FVector(WeaponData.MeshSize));

        // 강체 관련
        StaticMeshComp->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
        StaticMeshComp->SetSimulatePhysics(true);
        StaticMeshComp->SetCollisionProfileName("PhysicsActor");
        StaticMeshComp->SetNotifyRigidBodyCollision(true);
    }
}

void ACoreThrowableWeapon::UpdateCollider()
{
    if (CurrentWeaponType != EThrowableWeaponType::CLAYMORE)
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

void ACoreThrowableWeapon::Throw(FVector Velocity)
{
    if (!StaticMeshComp ||
        !ProjectileMovementComp)
        return;

    StaticMeshComp->SetSimulatePhysics(true);
    StaticMeshComp->SetCollisionProfileName("BlockAll");
    ProjectileMovementComp->AddForce(Velocity);
    this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);    
}