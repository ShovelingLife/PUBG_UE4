#include "CrateBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

ACrateBox::ACrateBox()
{
    PrimaryActorTick.bCanEverTick = true;
    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
    ObjectType = "CrateBox";
    UpdateCollider();
    InitVfx();
    InitMeshes();
}

void ACrateBox::Tick(float _DeltaTime)
{
    Super::Tick(_DeltaTime);

    // 착지 하지 않았으면 떨어짐
    if (IsRootComponentMovable())
        SetActorLocation(GetActorLocation() + FVector::DownVector);
    
    else // 착지 하였으면 카운트 시작
    {
        mCurrentTime += _DeltaTime;

        if(!mbParticleEnded)
        {
            if (mCurrentTime > mkMaxParticleTime)
            {
                mCurrentTime = 0.f;
                ParticleComp->Deactivate();
                mbParticleEnded = true;
            }
        }
    }
}

void ACrateBox::InitVfx()
{
    ABaseInteraction::InitParticleSystem("ParticleSystem'/Game/VFX/Items/Crate_box/Crate_Effect.Crate_Effect'");
    ParticleComp->SetRelativeTransform(FTransform(FRotator::ZeroRotator, FVector(0.f, 0.f, 70.f), FVector(1.35f)));
    ParticleComp->bAutoActivate = false;
    ParticleComp->Deactivate();
}

void ACrateBox::InitMeshes()
{
    // 보급 상자 메쉬 설정
    ABaseInteraction::InitStaticMesh("StaticMesh'/Game/Meshes/PUBG_CRATE/Box_crate_mesh.Box_crate_mesh'");
    StaticMeshComp->SetRelativeTransform(FTransform(FRotator::MakeFromEuler(FVector(-90.f, 0.f, 0.f)), FVector(0.f, 0.f, -70.f), FVector(0.1f)));

    // 낙하산 메쉬 설정
    auto parachuteMeshObj = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Meshes/PARACUTE_FREE/Parachute_mesh.Parachute_mesh'"));
    ParachuteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Parachute mesh"));

    if (parachuteMeshObj.Succeeded())
        ParachuteMesh->SetStaticMesh(parachuteMeshObj.Object);

    ParachuteMesh->SetRelativeTransform(FTransform(FRotator::MakeFromEuler(FVector(0.f, 800.f, 0.f)), FVector::ZeroVector, FVector(0.05f)));
    ParachuteMesh->AttachToComponent(StaticMeshComp, FAttachmentTransformRules::KeepRelativeTransform);
}

void ACrateBox::UpdateCollider()
{
    if (!ColliderComp)
        return;

    ColliderComp->SetCollisionProfileName("BlockAll");
    ColliderComp->OnComponentHit.AddDynamic(this, &ACrateBox::TurnOffAfterLanding);
    ColliderComp->BodyInstance.SetInstanceSimulatePhysics(true);
    ColliderComp->SetEnableGravity(false);
    ColliderComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
    ColliderComp->BodyInstance.bNotifyRigidBodyCollision = true;
    ColliderComp->SetBoxExtent(FVector(70.f));
}

void ACrateBox::TurnOffAfterLanding(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // 지면이랑 닿았을 시
    if (OtherActor->GetName() == "Floor")
    {
        // 이펙트 발동 후 고정

        if (ParachuteMesh)
            ParachuteMesh->DestroyComponent();
        
        if (ParticleComp)
            ParticleComp->Activate();
        
        if (ColliderComp)
            ColliderComp->SetMobility(EComponentMobility::Static);
    }
}