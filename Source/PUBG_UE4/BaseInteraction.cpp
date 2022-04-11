#include "BaseInteraction.h"
#include "DataTableManager.h"
#include "CustomGameInstance.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
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

    StaticMeshComp->SetupAttachment(RootComponent);
    // 경로로부터 메시 생성
    ConstructorHelpers::FObjectFinder<UStaticMesh> MESH(*Path);

    if (MESH.Succeeded())
        StaticMeshComp->SetStaticMesh(MESH.Object);
}

void ABaseInteraction::InitSkeletalMesh(FString Path)
{
    if (StaticMeshComp)
        StaticMeshComp->DestroyComponent();

    SkeletalMeshComp->SetupAttachment(RootComponent);

    // 경로로부터 메시 생성
    ConstructorHelpers::FObjectFinder<USkeletalMesh> MESH(*Path);

    if (MESH.Succeeded())
        SkeletalMeshComp->SetSkeletalMesh(MESH.Object);
}

void ABaseInteraction::InitComponents()
{
    SceneComp        = CreateDefaultSubobject<UBoxComponent>(TEXT("SceneComp"));
    ColliderComp     = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComp"));
    RootComponent    = ColliderComp;
    WidgetComp       = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidgetComp"));    
    AudioComp        = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
    SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComp");
    StaticMeshComp   = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
}

void ABaseInteraction::InitInteractionUI()
{
    auto p_customGameInst = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance());

    if (p_customGameInst)
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

void ABaseInteraction::InitParticleSystem(FString Path)
{
    // 파티클 컴포넌트 초기화
    ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
    ParticleComp->bAutoActivate = false;

    // 파티클 설정
    ConstructorHelpers::FObjectFinder<UParticleSystem> PARTICLE(*Path);

    if (PARTICLE.Succeeded())
        ParticleComp->SetTemplate(PARTICLE.Object);
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