#include "CoreBullet.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "UObject/ConstructorHelpers.h"
#include <NiagaraFunctionLibrary.h>

ACoreBullet::ACoreBullet()
{
    PrimaryActorTick.bCanEverTick = true;
    RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
}

void ACoreBullet::BeginPlay()
{
    Super::BeginPlay();

    if (NiagaraComp)
        NiagaraComp->Activate();
}

void ACoreBullet::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
    //NiagaraComp->SetAsset(ImpactEffect);
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactEffect, HitLocation);
    Destroy();
}

void ACoreBullet::Tick(float _DeltaTime)
{
    Super::Tick(_DeltaTime);

    mCurrentLifeTime += _DeltaTime;

    if (mCurrentLifeTime > mkLifeTime)
        Destroy();
}

void ACoreBullet::InitMesh()
{
    // 메시 생성
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMeshComp"));
    this->SetRootComponent(MeshComp);
    MeshComp->SetWorldScale3D(FVector(0.25f));
    MeshComp->SetCollisionProfileName("Destructible");
    MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComp->BodyInstance.SetInstanceSimulatePhysics(false);    
    ConstructorHelpers::FObjectFinder<UStaticMesh> BULLET_MESH(*(mWeaponData.BulletMeshPath));

    if (BULLET_MESH.Succeeded())
        MeshComp->SetStaticMesh(BULLET_MESH.Object);
}

void ACoreBullet::InitProjectileMovementComp()
{
    ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectilMovementComp"));
    ProjectileMovementComp->SetUpdatedComponent(MeshComp);
    ProjectileMovementComp->InitialSpeed = 10000.f;
    ProjectileMovementComp->MaxSpeed = 15000.f;
    ProjectileMovementComp->bRotationFollowsVelocity = true;
    ProjectileMovementComp->bShouldBounce = true;
    ProjectileMovementComp->Bounciness = 0.3f;
    ProjectileMovementComp->ProjectileGravityScale = 0.f;
}

void ACoreBullet::InitVFX()
{
    NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>("Effect");
    NiagaraComp->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
    NiagaraComp->SetWorldScale3D(FVector(0.5f));

    // 파티클 로드
    const FString defaultPath = "/Game/3_VFX/sA_Megapack_v1/sA_ShootingVfxPack/FX/NiagaraSystems/NS_";

    // 발사 즉시 발동될 이펙트
    FString particleType = (mWeaponData.Type == "RPG_17") ? "ROCKET_Trail" : "BulletTrail_1";
    ConstructorHelpers::FObjectFinder<UNiagaraSystem> TRAIL_SYS(*(defaultPath + particleType));

    if (TRAIL_SYS.Succeeded())
        TrailEffect = TRAIL_SYS.Object;

    // 타격 시 발동될 이펙트
    particleType = (mWeaponData.Type == "RPG_17") ? "Explossion" : "Impact_1";
    ConstructorHelpers::FObjectFinder<UNiagaraSystem> IMPACT_SYS(*(defaultPath + particleType));

    if (IMPACT_SYS.Succeeded())
        ImpactEffect = IMPACT_SYS.Object;

    NiagaraComp->SetAsset(TrailEffect);
}

void ACoreBullet::Init(EGunType Type)
{
    mWeaponData = ADataTableManager::GetWeaponData((int)Type);
    InitMesh();
    InitProjectileMovementComp();
    InitVFX();
}

void ACoreBullet::SetSpeed(const FVector& Dir)
{
    //ProjectileMovementComp->Velocity = Dir * ProjectileMovementComp->InitialSpeed;  
    ProjectileMovementComp->Velocity = Dir;
}