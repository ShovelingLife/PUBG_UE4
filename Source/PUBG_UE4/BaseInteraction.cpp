﻿#include "BaseInteraction.h"
#include "DataTableManager.h"
#include "CustomGameInstance.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

void ABaseInteraction::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
}

ABaseInteraction::ABaseInteraction()
{
    PrimaryActorTick.bCanEverTick = true;
    InitComponents();
}

void ABaseInteraction::BeginPlay()
{
    Super::BeginPlay();
    InitInteractionUI();
}

void ABaseInteraction::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (WidgetComp)
    {
        mCurrentTime = (bPlayerNear) ? mCurrentTime += DeltaTime : 0.f;

        // 0.25초 지날 시 UI설정
        WidgetComp->SetVisibility((mCurrentTime > 0.25f));
    }
}
void ABaseInteraction::InitStaticMesh(FString Path)
{
    if (SkeletalMeshComp)
        SkeletalMeshComp->DestroyComponent();

    this->SetRootComponent(StaticMeshComp);
    StaticMeshComp->RegisterComponent();

    // 경로로부터 메시 생성
    ConstructorHelpers::FObjectFinder<UStaticMesh> MESH(*Path);

    if (MESH.Succeeded())
        StaticMeshComp->SetStaticMesh(MESH.Object);

    StaticMeshComp->SetWorldLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
    AttachComponents();
}

void ABaseInteraction::InitSkeletalMesh(FString Path)
{
    if (StaticMeshComp)
        StaticMeshComp->DestroyComponent();

    this->SetRootComponent(SkeletalMeshComp);
    SkeletalMeshComp->RegisterComponent();

    // 경로로부터 메시 생성
    ConstructorHelpers::FObjectFinder<USkeletalMesh> MESH(*Path);

    if (MESH.Succeeded())
        SkeletalMeshComp->SetSkeletalMesh(MESH.Object);

    SkeletalMeshComp->SetWorldLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
    AttachComponents();
}

void ABaseInteraction::InitComponents()
{
    SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComp");
    StaticMeshComp   = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
    WidgetComp       = CreateDefaultSubobject<UWidgetComponent>("InteractionWidgetComp");
    AudioComp        = CreateDefaultSubobject<UAudioComponent>("AudioComp");
}

void ABaseInteraction::InitInteractionUI()
{
    if (auto p_customGameInst = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance()))
        p_customGameInst->DeleUpdateInteractionWidgetComp.ExecuteIfBound(WidgetComp, FString::Printf(TEXT("%s 줍기"), *ObjectType));
}

void ABaseInteraction::DestroyComponentsForUI()
{
    if (ColliderComp)
        ColliderComp->DestroyComponent();
    
    if (WidgetComp)
        WidgetComp->DestroyComponent();

    if (AudioComp)
        AudioComp->DestroyComponent();
}

void ABaseInteraction::SetForDummyCharacter()
{
    DestroyComponentsForUI();
    if (SkeletalMeshComp)
    {
        this->SetRootComponent(SkeletalMeshComp);
        SkeletalMeshComp->SetOwnerNoSee(true);
        SkeletalMeshComp->SetVisibility(true);

        if (StaticMeshComp)
            StaticMeshComp->DestroyComponent();

    }
    if (StaticMeshComp)
    {
        this->SetRootComponent(StaticMeshComp);
        StaticMeshComp->SetOwnerNoSee(true);
        StaticMeshComp->SetVisibility(true);       

        if (SkeletalMeshComp)
            SkeletalMeshComp->DestroyComponent();
    }
}

void ABaseInteraction::ChangeCollisionSettings(bool bTurned)
{
    // 컴포넌트에 따라 콜라이더 업데이트
    TArray<UPrimitiveComponent*> arrComp
    {
        ColliderComp,
        SkeletalMeshComp,
        StaticMeshComp
    };
    for (auto comp : arrComp)
    {
        if (comp)
        {
            comp->SetCollisionProfileName(bTurned ? "Object" : "NoCollision");
            comp->CanCharacterStepUpOn = bTurned ? ECanBeCharacterBase::ECB_Yes : ECanBeCharacterBase::ECB_No;
            comp->SetSimulatePhysics(false);
            break;
        }
    }
}

UStaticMesh* ABaseInteraction::GetStaticMesh() const
{
    return StaticMeshComp->GetStaticMesh();
}

USkeletalMesh* ABaseInteraction::GetSkeletalMesh() const
{
    return SkeletalMeshComp->SkeletalMesh;
}

void ABaseInteraction::SetStaticMesh(UStaticMesh* Mesh)
{
    if (StaticMeshComp)
        StaticMeshComp->SetStaticMesh(Mesh);
}

void ABaseInteraction::SetSkeletalMesh(USkeletalMesh* Mesh)
{
    if (SkeletalMeshComp)
        SkeletalMeshComp->SetSkeletalMesh(Mesh);
}

void ABaseInteraction::InitParticleSystem(FString Path)
{
    // 파티클 컴포넌트 초기화
    ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
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

void ABaseInteraction::AttachComponents()
{
    if (WidgetComp)
        WidgetComp->SetupAttachment(RootComponent);
    
    if (AudioComp)
        AudioComp->SetupAttachment(RootComponent);
    
    if (ParticleComp)
        ParticleComp->SetupAttachment(RootComponent);
}

void ABaseInteraction::SetCollisionSettingsForObjects()
{
    if (ColliderComp)
    {
        ColliderComp->BodyInstance.SetCollisionProfileName("Object");
        ColliderComp->BodyInstance.bNotifyRigidBodyCollision = false;
    }
}