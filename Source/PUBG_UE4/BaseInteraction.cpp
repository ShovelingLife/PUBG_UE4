#include "BaseInteraction.h"
#include "DataTableManager.h"
#include "CustomGameInstance.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ABaseInteraction::ABaseInteraction()
{
    PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
    
    // 박스 콜라이더 초기화
    ColliderComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
    ColliderComp->SetupAttachment(RootComponent);

    // UI 위젯 컴포넌트 초기화
    WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidgetComp"));
    WidgetComp->SetupAttachment(RootComponent);

    // 스태틱 메시 생성
    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
    StaticMeshComp->SetupAttachment(RootComponent);

    // 스켈레탈 메시 생성
    SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComp");
    SkeletalMeshComp->SetupAttachment(RootComponent);
}

void ABaseInteraction::BeginPlay()
{
    Super::BeginPlay();
    InitInteractionUI();
    ColliderComp->BodyInstance.SetCollisionProfileName("Object");
    ColliderComp->BodyInstance.bNotifyRigidBodyCollision = false;
}

void ABaseInteraction::Tick(float _DeltaTime)
{
    Super::Tick(_DeltaTime);

    if (WidgetComp)
    {
        mCurrentTime = (bPlayerNear) ? mCurrentTime += _DeltaTime : 0.f;

        // 0.25초 지날 시 UI설정
        if (mCurrentTime > 0.25f)
            WidgetComp->SetVisibility(true);

        else
            WidgetComp->SetVisibility(false);
    }
}

void ABaseInteraction::InitStaticMesh(FString _Path)
{
    SkeletalMeshComp->DestroyComponent();

    // 경로로부터 메시 생성
    ConstructorHelpers::FObjectFinder<UStaticMesh> MESH(*_Path);

    if (MESH.Succeeded())
        StaticMeshComp->SetStaticMesh(MESH.Object);
}

void ABaseInteraction::InitSkeletalMesh(FString _Path)
{
    StaticMeshComp->DestroyComponent();

    // 경로로부터 메시 생성
    ConstructorHelpers::FObjectFinder<USkeletalMesh> MESH(*_Path);

    if (MESH.Succeeded())
        SkeletalMeshComp->SetSkeletalMesh(MESH.Object);
}

void ABaseInteraction::InitAudio()
{
    // 오디오 포인터에 대한 생성
    AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
    AudioComp->SetupAttachment(RootComponent);
}

void ABaseInteraction::InitInteractionUI()
{
    auto p_customGameInst = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance());

    if (p_customGameInst)
        p_customGameInst->DeleUpdateInteractionWidgetComp.ExecuteIfBound(WidgetComp, FString::Printf(TEXT("%s 줍기"), *ObjectType));
}

void ABaseInteraction::DestroyComponentsForUI(FString _MeshType)
{
    ColliderComp->DestroyComponent();
    WidgetComp->DestroyComponent();

    if (_MeshType == "Skeletal")
        StaticMeshComp->DestroyComponent();

    else
        SkeletalMeshComp->DestroyComponent();
}