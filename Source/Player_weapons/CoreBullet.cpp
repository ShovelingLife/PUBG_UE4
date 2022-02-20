#include "CoreBullet.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ACoreBullet::ACoreBullet()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACoreBullet::BeginPlay()
{
    Super::BeginPlay();
}

void ACoreBullet::Tick(float _DeltaTime)
{
    Super::Tick(_DeltaTime);

    mCurrentLifeTime += _DeltaTime;

    if (mCurrentLifeTime > mkLifeTime)
        Destroy();

    if (MeshComp)
        MeshComp->SetRelativeLocation(FVector(0.f, -0.5f, 0.f));
}

void ACoreBullet::OnHit(UPrimitiveComponent* _my_comp, AActor* _other, UPrimitiveComponent* _other_comp, FVector _normal_impulse, const FHitResult& _hit)
{
    Destroy();
}

void ACoreBullet::InitMesh()
{
    // 메시 생성
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet_mesh"));
    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MeshComp->AttachToComponent(ColliderComp, FAttachmentTransformRules::KeepWorldTransform);

    ConstructorHelpers::FObjectFinder<UStaticMesh> BULLET_MESH(*mWeaponData.BulletMeshPath);

    if (BULLET_MESH.Succeeded())
        MeshComp->SetStaticMesh(BULLET_MESH.Object);
}

void ACoreBullet::InitCollider()
{
    ColliderComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
    RootComponent = ColliderComp;
    ColliderComp->SetCollisionProfileName(TEXT("Projectile"));
    ColliderComp->SetCollisionObjectType(ECC_Destructible);
    ColliderComp->SetSimulatePhysics(true);
    ColliderComp->OnComponentHit.AddDynamic(this, &ACoreBullet::OnHit);
    ColliderComp->AddWorldRotation(FRotator::MakeFromEuler(FVector(90.f, 0.f, 0.f)));
    ColliderComp->SetCapsuleHalfHeight(1.75f);
    ColliderComp->SetCapsuleRadius(1.75f);
}

void ACoreBullet::InitProjectileMovementComp()
{
    ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile_movement"));
    ProjectileMovementComp->SetUpdatedComponent(ColliderComp);
    ProjectileMovementComp->InitialSpeed = 1000.f;
    ProjectileMovementComp->MaxSpeed = 3000.f;
    ProjectileMovementComp->bRotationFollowsVelocity = true;
    ProjectileMovementComp->bShouldBounce = true;
    ProjectileMovementComp->Bounciness = 0.3f;
    ProjectileMovementComp->ProjectileGravityScale = 0.f;
}

void ACoreBullet::Init(EWeaponType _Index)
{
    //mWeaponData = ADataTableManager::ArrWeaponData[(int)_Index];
    //InitCollider();
    //InitMesh();
    //InitProjectileMovementComp();
}