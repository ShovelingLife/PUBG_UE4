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
    ObjectType = "보급상자";
    UpdateCollider();
    InitVfx();
    InitMeshes();
}

void ACrateBox::Tick(float _DeltaTime)
{
    Super::Tick(_DeltaTime);

    if (ColliderComp->GetCollisionProfileName() == "Object")
        ColliderComp->SetCollisionProfileName("BlockAll");

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
    // 리소스 가지고와서 부여
    ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Crate box effect"));
    ParticleComp->SetRelativeTransform(FTransform(FRotator::ZeroRotator, FVector(0.f, 0.f, 70.f), FVector(1.35f)));
    ParticleComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    auto crateBoxEffect = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("ParticleSystem'/Game/VFX/Items/Crate_box/Crate_Effect.Crate_Effect'"));
    
    if (crateBoxEffect.Succeeded())
        ParticleComp->SetTemplate(crateBoxEffect.Object);

    ParticleComp->bAutoActivate = false;
    ParticleComp->Deactivate();
}

void ACrateBox::InitMeshes()
{
    // 보급 상자 메쉬 설정
    ABaseInteraction::InitStaticMesh("StaticMesh'/Game/Meshes/PUBG_CRATE/Box_crate_mesh.Box_crate_mesh'","Box_crate");
    StaticMeshComp->SetRelativeTransform(FTransform(FRotator::MakeFromEuler(FVector(-90.f, 0.f, 0.f)), FVector(0.f, 0.f, -70.f), FVector(0.1f)));

    // 낙하산 메쉬 설정
    auto parachuteMeshObj = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Meshes/PARACUTE_FREE/Parachute_mesh.Parachute_mesh'"));
    mParachuteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Parachute mesh"));

    if (parachuteMeshObj.Succeeded())
        mParachuteMesh->SetStaticMesh(parachuteMeshObj.Object);

    mParachuteMesh->SetRelativeTransform(FTransform(FRotator::MakeFromEuler(FVector(0.f, 800.f, 0.f)), FVector::ZeroVector, FVector(0.05f)));
    mParachuteMesh->AttachToComponent(StaticMeshComp, FAttachmentTransformRules::KeepRelativeTransform);
}

void ACrateBox::UpdateCollider()
{
    // 루트컴포넌트 재정의 후 재부착
    RootComponent->DestroyComponent();
    RootComponent = ColliderComp;

    ColliderComp->OnComponentHit.AddDynamic(this, &ACrateBox::TurnOffAfterLanding);
    ColliderComp->SetSimulatePhysics(true);
    ColliderComp->SetEnableGravity(false);
    ColliderComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
    ColliderComp->BodyInstance.bNotifyRigidBodyCollision = true;
    ColliderComp->SetBoxExtent(FVector(70.f));
}

void ACrateBox::TurnOffAfterLanding(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    FString actor_name = "";
    OtherActor->GetName(actor_name);

    // 지면이랑 닿았을 시
    if (actor_name == "Floor")
    {
        // 이펙트 발동 후 고정
        mParachuteMesh->DestroyComponent();
        ParticleComp->Activate();
        ColliderComp->SetMobility(EComponentMobility::Static);
    }
}