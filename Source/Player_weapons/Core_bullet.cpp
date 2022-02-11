#include "Core_bullet.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ACore_bullet::ACore_bullet()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACore_bullet::BeginPlay()
{
    Super::BeginPlay();
}

void ACore_bullet::Tick(float _delta_time)
{
    Super::Tick(_delta_time);

    m_current_life_time += _delta_time;

    if (m_current_life_time > mk_life_time)
        Destroy();

    if (p_mesh_comp)
        p_mesh_comp->SetRelativeLocation(FVector(0.f, -0.5f, 0.f));
}

void ACore_bullet::OnHit(UPrimitiveComponent* _my_comp, AActor* _other, UPrimitiveComponent* _other_comp, FVector _normal_impulse, const FHitResult& _hit)
{
    Destroy();
}

void ACore_bullet::Init_mesh()
{
    // 메시 생성
    p_mesh_comp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet_mesh"));
    p_mesh_comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    p_mesh_comp->AttachToComponent(p_collider_comp, FAttachmentTransformRules::KeepWorldTransform);

    ConstructorHelpers::FObjectFinder<UStaticMesh> BULLET_MESH(*m_weapon_data.bullet_mesh_path);

    if (BULLET_MESH.Succeeded())
        p_mesh_comp->SetStaticMesh(BULLET_MESH.Object);
}

void ACore_bullet::Init_collider()
{
    p_collider_comp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
    RootComponent = p_collider_comp;
    p_collider_comp->SetCollisionProfileName(TEXT("Projectile"));
    p_collider_comp->SetCollisionObjectType(ECC_Destructible);
    p_collider_comp->SetSimulatePhysics(true);
    p_collider_comp->OnComponentHit.AddDynamic(this, &ACore_bullet::OnHit);
    p_collider_comp->AddWorldRotation(FRotator::MakeFromEuler(FVector(90.f, 0.f, 0.f)));
    p_collider_comp->SetCapsuleHalfHeight(1.75f);
    p_collider_comp->SetCapsuleRadius(1.75f);
}

void ACore_bullet::Init_projectile_movement_component()
{
    p_projectile_movement_comp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile_movement"));
    p_projectile_movement_comp->SetUpdatedComponent(p_collider_comp);
    p_projectile_movement_comp->InitialSpeed = 1000.f;
    p_projectile_movement_comp->MaxSpeed = 3000.f;
    p_projectile_movement_comp->bRotationFollowsVelocity = true;
    p_projectile_movement_comp->bShouldBounce = true;
    p_projectile_movement_comp->Bounciness = 0.3f;
    p_projectile_movement_comp->ProjectileGravityScale = 0.f;
}

void ACore_bullet::Init(e_weapon_type _index)
{
    m_weapon_data = AData_table_manager::arr_weapon_data[(int)_index];
    Init_collider();
    Init_mesh();
    Init_projectile_movement_component();
}