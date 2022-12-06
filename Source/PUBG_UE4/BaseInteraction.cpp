#include "BaseInteraction.h"
#include "DataTableManager.h"
#include "CustomGameInstance.h"
#include "InteractionComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ABaseInteraction::ABaseInteraction()
{
    PrimaryActorTick.bCanEverTick = true;
    InitComponents();
}

void ABaseInteraction::BeginPlay()
{
    Super::BeginPlay();
    InteractionComp->InitInteractionText(ObjectType);
}

void ABaseInteraction::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABaseInteraction::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
}

void ABaseInteraction::InitComponents()
{
    SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComp");
    StaticMeshComp   = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
    ParticleComp     = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
    InteractionComp  = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComp"));
    ColliderComp     = CreateDefaultSubobject<UBoxComponent>("ColliderComp");
}

void ABaseInteraction::InitStaticMesh(FString Path)
{
    if (IsValid(SkeletalMeshComp))
        SkeletalMeshComp->DestroyComponent();

    this->SetRootComponent(StaticMeshComp);

    // 경로로부터 메시 생성
    ConstructorHelpers::FObjectFinder<UStaticMesh> MESH(*Path);

    if (MESH.Succeeded())
        StaticMeshComp->SetStaticMesh(MESH.Object);

    StaticMeshComp->SetWorldLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);    
}

void ABaseInteraction::InitSkeletalMesh(FString Path)
{
    if (IsValid(StaticMeshComp))
        StaticMeshComp->DestroyComponent();

    this->SetRootComponent(SkeletalMeshComp);

    // 경로로부터 메시 생성
    ConstructorHelpers::FObjectFinder<USkeletalMesh> MESH(*Path);

    if (MESH.Succeeded())
        SkeletalMeshComp->SetSkeletalMesh(MESH.Object);

    SkeletalMeshComp->SetSimulatePhysics(false);
    SkeletalMeshComp->SetWorldLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);    

    InitCollider();
}

void ABaseInteraction::InitCollider()
{
    // 충돌체 초기화
    ColliderComp->SetupAttachment(RootComponent);
    ColliderComp->BodyInstance.SetCollisionProfileName("Object");
    ColliderComp->BodyInstance.bNotifyRigidBodyCollision = false;
    ColliderComp->SetSimulatePhysics(false);
}

UStaticMesh* ABaseInteraction::GetStaticMesh() const { return StaticMeshComp->GetStaticMesh(); }

USkeletalMesh* ABaseInteraction::GetSkeletalMesh() const { return SkeletalMeshComp->SkeletalMesh; }

void ABaseInteraction::SetStaticMesh(UStaticMesh* Mesh) { if (StaticMeshComp) StaticMeshComp->SetStaticMesh(Mesh); }

void ABaseInteraction::SetSkeletalMesh(USkeletalMesh* Mesh) { if (SkeletalMeshComp) SkeletalMeshComp->SetSkeletalMesh(Mesh); }

void ABaseInteraction::DestroyComponentsForUI() { InteractionComp->DestroyComponent(); }

void ABaseInteraction::SetForDummyCharacter()
{
    DestroyComponentsForUI();

    if (IsValid(SkeletalMeshComp))
    {
        this->SetRootComponent(SkeletalMeshComp);
        SkeletalMeshComp->SetOwnerNoSee(true);
        SkeletalMeshComp->SetVisibility(true);

        if (StaticMeshComp)
            StaticMeshComp->DestroyComponent();

    }
    if (IsValid(StaticMeshComp))
    {
        this->SetRootComponent(StaticMeshComp);
        StaticMeshComp->SetOwnerNoSee(true);
        StaticMeshComp->SetVisibility(true);       

        if (SkeletalMeshComp)
            SkeletalMeshComp->DestroyComponent();
    }
}

void ABaseInteraction::ChangeCollisionSettings(bool bTurned /* = true */)
{
    ColliderComp->SetCollisionProfileName(bTurned ? "Object" : "NoCollision");
    ColliderComp->CanCharacterStepUpOn = bTurned ? ECanBeCharacterBase::ECB_Yes : ECanBeCharacterBase::ECB_No;    
    // 컴포넌트에 따라 콜라이더 업데이트
    /*if      (IsValid(SkeletalMeshComp))
             ChangeCollisionSettings(SkeletalMeshComp, bTurned);

    else if (IsValid(StaticMeshComp))
             ChangeCollisionSettings(StaticMeshComp,bTurned);*/
}

void ABaseInteraction::ChangeCollisionSettings(UPrimitiveComponent* MeshComp, bool bTurned)
{
    
}

void ABaseInteraction::AttachToMesh(USceneComponent* RootComp, FString SocketName)
{
    if (SkeletalMeshComp)
        SkeletalMeshComp->AttachToComponent(RootComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, *SocketName);

    else if (StaticMeshComp)
             StaticMeshComp->AttachToComponent(RootComp, FAttachmentTransformRules::SnapToTargetIncludingScale, *SocketName);
}

void ABaseInteraction::Detach(FVector NewPos)
{
    UMeshComponent* meshComp = nullptr;

    if      (IsValid(SkeletalMeshComp))
             meshComp = SkeletalMeshComp;

    else if (IsValid(StaticMeshComp))
             meshComp = StaticMeshComp;

    else
        return;

    // 컴포넌트를 탈착 > 현재 루트 컴포넌트에 부착 > 트랜스폼 초기화
    meshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
    meshComp->ResetRelativeTransform();

    // 현재 무기를 탈착 후 월드에 소환
    this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    this->SetActorLocation(NewPos);
    this->ChangeCollisionSettings();
}

void ABaseInteraction::TurnUI(bool bOnOff /* = true */) { InteractionComp->bPlayerNear = bOnOff; }

void ABaseInteraction::InitParticleSystem(FString Path)
{
    // 파티클 컴포넌트 초기화
    ParticleComp->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
    ParticleComp->bAutoActivate = false;
    ParticleComp->SecondsBeforeInactive = 0.f;

    // 파티클 설정
    ConstructorHelpers::FObjectFinder<UParticleSystem> PARTICLE(*Path);

    if (PARTICLE.Succeeded())
    {
        ParticleComp->SetTemplate(PARTICLE.Object);
        Particle = PARTICLE.Object;
    }
}