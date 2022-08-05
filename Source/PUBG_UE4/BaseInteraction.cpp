#include "BaseInteraction.h"
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

void ABaseInteraction::InitComponents()
{
    ColliderComp = CreateDefaultSubobject<UBoxComponent>("ColliderComp");
    SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComp");
    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
    WidgetComp = CreateDefaultSubobject<UWidgetComponent>("InteractionWidgetComp");
    ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
}

void ABaseInteraction::InitInteractionUI()
{
    if (auto p_customGameInst = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance()))
        p_customGameInst->DeleUpdateInteractionWidgetComp.ExecuteIfBound(WidgetComp, FString::Printf(TEXT("%s 줍기"), *ObjectType));
}

void ABaseInteraction::AttachComponents()
{
    if (WidgetComp)
        WidgetComp->SetupAttachment(RootComponent);

    if (ParticleComp)
        ParticleComp->SetupAttachment(RootComponent);
}

void ABaseInteraction::SetCollisionSettingsForObjects()
{
    if (ColliderComp)
    {
        this->SetRootComponent(ColliderComp);
        ColliderComp->BodyInstance.SetCollisionProfileName("Object");
        ColliderComp->BodyInstance.bNotifyRigidBodyCollision = false;
    }
}

void ABaseInteraction::InitStaticMesh(FString Path)
{
    if (SkeletalMeshComp)
        SkeletalMeshComp->DestroyComponent();

    if (ColliderComp)
        ColliderComp->DestroyComponent();

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

    if (!RootComponent)
        this->SetRootComponent(SkeletalMeshComp);

    else
        SkeletalMeshComp->SetupAttachment(RootComponent);

    SkeletalMeshComp->RegisterComponent();

    // 경로로부터 메시 생성
    ConstructorHelpers::FObjectFinder<USkeletalMesh> MESH(*Path);

    if (MESH.Succeeded())
        SkeletalMeshComp->SetSkeletalMesh(MESH.Object);

    SkeletalMeshComp->SetSimulatePhysics(false);
    SkeletalMeshComp->SetWorldLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
    AttachComponents();
}

UStaticMesh* ABaseInteraction::GetStaticMesh() const { return StaticMeshComp->GetStaticMesh(); }

USkeletalMesh* ABaseInteraction::GetSkeletalMesh() const { return SkeletalMeshComp->SkeletalMesh; }

void ABaseInteraction::SetStaticMesh(UStaticMesh* Mesh) { if (StaticMeshComp) StaticMeshComp->SetStaticMesh(Mesh); }

void ABaseInteraction::SetSkeletalMesh(USkeletalMesh* Mesh) { if (SkeletalMeshComp) SkeletalMeshComp->SetSkeletalMesh(Mesh); }

void ABaseInteraction::DestroyComponentsForUI()
{
    if (ColliderComp)
        ColliderComp->DestroyComponent();
    
    if (WidgetComp)
        WidgetComp->DestroyComponent();
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

void ABaseInteraction::AttachToMesh(USceneComponent* RootComp, FString SocketName)
{
    if (SkeletalMeshComp)
        SkeletalMeshComp->AttachToComponent(RootComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, *SocketName);

    if (StaticMeshComp)
        StaticMeshComp->AttachToComponent(RootComp, FAttachmentTransformRules::SnapToTargetIncludingScale, *SocketName);
}

void ABaseInteraction::Detach(FTransform NewPos)
{
    UMeshComponent* meshComp = nullptr;

    if (SkeletalMeshComp)
        meshComp = SkeletalMeshComp;

    if (StaticMeshComp)
        meshComp = StaticMeshComp;

    if (!meshComp)
        return;

    // 컴포넌트를 탈착 > 현재 루트 컴포넌트에 부착 > 트랜스폼 초기화
    meshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
    //meshComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    meshComp->ResetRelativeTransform();

    // 현재 무기를 탈착 후 월드에 소환
    this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    this->SetActorTransform(NewPos);
    this->ChangeCollisionSettings(true);
}

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