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
     RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root_comp"));
     
     // 박스 콜라이더 초기화
     ColliderComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision_comp"));
     ColliderComp->SetupAttachment(RootComponent);

     // UI 위젯 컴포넌트 초기화
     WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction_widget"));
     WidgetComp->SetupAttachment(RootComponent);
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

     mCurrentTime = (bPlayerNear) ? mCurrentTime += _DeltaTime : 0.f;

     // 0.25초 지날 시 UI설정
     if (mCurrentTime > 0.25f)
         WidgetComp->SetVisibility(true);

     else
         WidgetComp->SetVisibility(false);
 }

 void ABaseInteraction::InitStaticMesh(FString _Path, FName _Name)
 {
     // 메시 생성
     StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(_Name);
     StaticMeshComp->SetupAttachment(RootComponent);

     // 경로로부터 메시 생성
     ConstructorHelpers::FObjectFinder<UStaticMesh> MESH(*_Path);

     if (MESH.Succeeded())
         StaticMeshComp->SetStaticMesh(MESH.Object);
 }

 void ABaseInteraction::InitSkeletalMesh(FString _Path, FName _Name)
 {
     // 메시 생성
     SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(_Name);
     SkeletalMeshComp->SetupAttachment(RootComponent);

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