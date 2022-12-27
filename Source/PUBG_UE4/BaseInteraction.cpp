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

UStaticMesh* ABaseInteraction::GetStaticMesh() const { return StaticMeshComp->GetStaticMesh(); }

USkeletalMesh* ABaseInteraction::GetSkeletalMesh() const { return SkeletalMeshComp->SkeletalMesh; }

void ABaseInteraction::SetStaticMesh(UStaticMesh* Mesh) { if (IsValid(StaticMeshComp)) StaticMeshComp->SetStaticMesh(Mesh); }

void ABaseInteraction::SetSkeletalMesh(USkeletalMesh* Mesh) { if (IsValid(SkeletalMeshComp)) SkeletalMeshComp->SetSkeletalMesh(Mesh); }

void ABaseInteraction::SetTurnUI(bool bOnOff /* = true */) { } // if (IsValid(InteractionComp)) InteractionComp->bPlayerNear = bOnOff;

ABaseInteraction::ABaseInteraction()
{
    PrimaryActorTick.bCanEverTick = true;
    InitComponents();
}

void ABaseInteraction::BeginPlay()
{
    Super::BeginPlay();    
}

void ABaseInteraction::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    InitWidget();    
}

void ABaseInteraction::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
}

void ABaseInteraction::InitComponents()
{
    WidgetComp       = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
    SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComp");
    StaticMeshComp   = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
    ParticleComp     = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
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
    WidgetComp->SetupAttachment(RootComponent);
}

void ABaseInteraction::InitCollider()
{
    // 충돌체 초기화
    ColliderComp->SetupAttachment(RootComponent);
    ColliderComp->BodyInstance.SetCollisionProfileName("Object");
    ColliderComp->BodyInstance.bNotifyRigidBodyCollision = false;
    ColliderComp->SetSimulatePhysics(false);
}

void ABaseInteraction::InitWidget()
{
    if (!mbWidgetInitialized)
    {
        // 상호작용 텍스트 == ""일 시
        if (!ObjectType.IsEmpty())
        {
            // 상호작용 텍스트 초기화
            if (auto p_customGameInst = UCustomGameInstance::GetInst())
            {
                auto event = p_customGameInst->DeleSetInteractionWidgetComp;

                if (event.IsBound())
                    mbWidgetInitialized = event.Execute(WidgetComp, FString::Printf(TEXT("%s PickUp "), *ObjectType));
            }
        }
    }
}

UMeshComponent* ABaseInteraction::GetMeshComp() const
{
    if (StaticMeshComp)
        return StaticMeshComp;

    else if (SkeletalMeshComp)
        return SkeletalMeshComp;

    else
        return nullptr;
}

void ABaseInteraction::DestroyComponentsForUI() { if (WidgetComp) WidgetComp->DestroyComponent(); }

void ABaseInteraction::SetForDummyCharacter()
{
    // 메쉬 판별해내기
    UMeshComponent* meshComp = GetMeshComp();

    if (!meshComp)
        return;

    // 메쉬 설정 변경
    DestroyComponentsForUI();
    SetRootComponent(meshComp);
    meshComp->SetOwnerNoSee(true);
    meshComp->SetVisibility(true);
}

void ABaseInteraction::ChangeCollisionSettings(bool bTurned /* = true */)
{
    if (!ColliderComp)
        return;

    // 컴포넌트에 따라 콜라이더 업데이트
    ColliderComp->SetCollisionProfileName(bTurned ? "Object" : "NoCollision");
    ColliderComp->CanCharacterStepUpOn = bTurned ? ECanBeCharacterBase::ECB_Yes : ECanBeCharacterBase::ECB_No;
}

void ABaseInteraction::ChangeMeshSettings(bool bTurned /* = true */)
{
    // 메쉬 가져오기
    UMeshComponent* meshComp = GetMeshComp();

    if (!meshComp)
        return;

    // 컴포넌트에 따라 콜라이더 업데이트
    meshComp->SetCollisionProfileName("Object");
    meshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    meshComp->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Yes;
    meshComp->SetSimulatePhysics(true);
}

void ABaseInteraction::AttachToMesh(USceneComponent* RootComp, FString SocketName)
{
    // 메쉬 가져오기    
    UMeshComponent* meshComp = GetMeshComp();
    FAttachmentTransformRules transformRules = FAttachmentTransformRules::SnapToTargetIncludingScale;

    if (!meshComp)
        return;

    // 메쉬에 따라 설정하기
    meshComp->AttachToComponent(RootComp, transformRules, *SocketName);
}

void ABaseInteraction::Detach()
{
    // 현재 무기를 탈착 후 월드에 소환
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    ChangeMeshSettings();
    ResetSettings();
}

void ABaseInteraction::InitParticleSystem(FString Path)
{
    // 파티클 컴포넌트 초기화
    ParticleComp->SetupAttachment(RootComponent);
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