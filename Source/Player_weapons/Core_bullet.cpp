// Fill out your copyright notice in the Description page of Project Settings.


#include "Core_bullet.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ACore_bullet::ACore_bullet()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

void ACore_bullet::BeginPlay()
{
    Super::BeginPlay();

    GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Cyan, m_weapon_data.bullet_mesh_path);
}

void ACore_bullet::Tick(float _delta_time)
{
    Super::Tick(_delta_time);

    m_current_life_time += _delta_time;

    if (m_current_life_time > mk_life_time)
        Destroy();

    if (mesh)
        mesh->SetRelativeLocation(FVector(0.f, -0.5f, 0.f));
}

void ACore_bullet::OnHit(UPrimitiveComponent* _my_comp, AActor* _other, UPrimitiveComponent* _other_comp, FVector _normal_impulse, const FHitResult& _hit)
{
    Destroy();
}

void ACore_bullet::Init(e_weapon_type _index)
{
    m_weapon_data = AData_table_manager::arr_weapon_data[(int)_index];
    Init_collider();
    Init_mesh();
    Init_projectile_movement_component();
}

void ACore_bullet::Init_mesh()
{
    // 메시 생성
    mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet_mesh"));
    mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    mesh->AttachToComponent(collider, FAttachmentTransformRules::KeepWorldTransform);

    ConstructorHelpers::FObjectFinder<UStaticMesh> BULLET_MESH(*m_weapon_data.bullet_mesh_path);

    if (BULLET_MESH.Succeeded())
        mesh->SetStaticMesh(BULLET_MESH.Object);
}

void ACore_bullet::Init_collider()
{
    collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
    RootComponent = collider;
    collider->SetCollisionProfileName(TEXT("Projectile"));
    collider->SetCollisionObjectType(ECC_Destructible);
    collider->SetSimulatePhysics(true);
    collider->OnComponentHit.AddDynamic(this, &ACore_bullet::OnHit);
    collider->AddWorldRotation(FRotator::MakeFromEuler(FVector(90.f, 0.f, 0.f)));
    collider->SetCapsuleHalfHeight(1.75f);
    collider->SetCapsuleRadius(1.75f);
}

void ACore_bullet::Init_projectile_movement_component()
{
    projectile_movement_component = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile_movement"));
    projectile_movement_component->SetUpdatedComponent(collider);
    projectile_movement_component->InitialSpeed = 1000.f;
    projectile_movement_component->MaxSpeed = 3000.f;
    projectile_movement_component->bRotationFollowsVelocity = true;
    projectile_movement_component->bShouldBounce = true;
    projectile_movement_component->Bounciness = 0.3f;
    projectile_movement_component->ProjectileGravityScale = 0.f;
}