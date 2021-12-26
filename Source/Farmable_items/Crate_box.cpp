#include "Crate_box.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

ACrate_box::ACrate_box()
{
    PrimaryActorTick.bCanEverTick = true;
    m_box_collider->OnComponentHit.AddDynamic(this, &ACrate_box::Turn_off_after_landing);
    Init_vfx();
    Init_meshes();
    Update_collider();
}

void ACrate_box::Tick(float _delta_time)
{
    Super::Tick(_delta_time);

    // 착지 하지 않았으면 떨어짐
    if (!m_is_landed)
        SetActorLocation(GetActorLocation() + FVector::DownVector);

    else // 착지 하였으면 카운트 시작
    {
        m_current_time += _delta_time;

        if(!m_is_particle_ended)
        {
            if (m_current_time > mk_max_particle_time)
            {
                m_current_time = 0.f;
                mp_particle_component->Deactivate();
                m_is_particle_ended = true;
            }
        }
        else // 파티클 끝난 후 시간 경과 시 파괴
        {
            if (m_current_time > mk_max_spawn_time)
                Destroy();
        }
    }
}

void ACrate_box::Init_vfx()
{
    // 리소스 가지고와서 부여
    mp_particle_component = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Crate box effect"));
    mp_particle_component->SetRelativeTransform(FTransform(FRotator::ZeroRotator, FVector(0.f, 0.f, 70.f), FVector(1.35f)));
    mp_particle_component->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    auto crate_box_effect = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("ParticleSystem'/Game/VFX/Items/Crate_box/Crate_Effect.Crate_Effect'"));
    
    if (crate_box_effect.Succeeded())
        mp_particle_component->SetTemplate(crate_box_effect.Object);

    mp_particle_component->bAutoActivate = false;
    mp_particle_component->Deactivate();
}

void ACrate_box::Init_meshes()
{
    // 보급 상자 메쉬 설정
    ABase_interaction::Init_static_mesh("StaticMesh'/Game/Meshes/PUBG_CRATE/Box_crate_mesh.Box_crate_mesh'","Box_crate");
    m_static_mesh->SetRelativeTransform(FTransform(FRotator::MakeFromEuler(FVector(-90.f, 0.f, 0.f)), FVector(0.f, 0.f, -70.f), FVector(0.1f)));

    // 낙하산 메쉬 설정
    auto parachute_mesh_obj = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Meshes/PARACUTE_FREE/Parachute_mesh.Parachute_mesh'"));
    mp_parachute_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Parachute mesh"));

    if (parachute_mesh_obj.Succeeded())
        mp_parachute_mesh->SetStaticMesh(parachute_mesh_obj.Object);

    mp_parachute_mesh->SetRelativeTransform(FTransform(FRotator::MakeFromEuler(FVector(0.f, 800.f, 0.f)), FVector::ZeroVector, FVector(0.05f)));
    mp_parachute_mesh->AttachToComponent(m_static_mesh, FAttachmentTransformRules::KeepRelativeTransform);
}

void ACrate_box::Update_collider()
{
    m_box_collider->SetSimulatePhysics(true);
    m_box_collider->SetEnableGravity(false);
    m_box_collider->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
    m_box_collider->BodyInstance.bNotifyRigidBodyCollision = true;
    m_box_collider->SetCollisionProfileName("BlockAll");
    m_box_collider->SetBoxExtent(FVector(70.f));
}

void ACrate_box::Turn_off_after_landing(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    FString actor_name = "";
    OtherActor->GetName(actor_name);
    GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Cyan, actor_name);

    // 지면이랑 닿았을 시
    if (actor_name == "Floor")
    {
        // 이펙트 발동 후 고정
        m_is_landed = true;
        mp_parachute_mesh->DestroyComponent();
        mp_particle_component->Activate();
        m_box_collider->SetMobility(EComponentMobility::Static);
    }
}