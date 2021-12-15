//Fill out your copyright notice in the Description page of Project Settings.


#include "Base_interaction.h"
#include "PUBG_UE4/Data_table_manager.h"
#include "UI_PUBG/Interaction_UI.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

 ABase_interaction::ABase_interaction()
 {
     // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
     PrimaryActorTick.bCanEverTick = true;

     // F키 바인딩
     if (InputComponent)
         InputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &ABase_interaction::Interact);

     m_box_collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
     RootComponent  = m_box_collider;

     // 위젯 컴포넌트 초기화
     p_widget_component = CreateDefaultSubobject<UWidgetComponent>(TEXT("UI"));
     p_widget_component->AttachToComponent(m_box_collider, FAttachmentTransformRules::KeepRelativeTransform);
     auto bp_class      = ConstructorHelpers::FClassFinder<UInteraction_UI>(TEXT("/Game/Blueprints/UI/BP_Interaction_UI"));
     widget_bp_class    = bp_class.Class;

 }

 // Called when the game starts or when spawned
 void ABase_interaction::BeginPlay()
 {
     Super::BeginPlay();

     // 위젯 설정
     if (widget_bp_class)
     {
         p_widget_component->SetWidgetClass(widget_bp_class);
         p_widget_component->SetRelativeLocation(FVector::ZeroVector);
         p_widget_component->SetWidgetSpace(EWidgetSpace::Screen);
         p_widget = Cast<UInteraction_UI>(p_widget_component->GetWidget());
     }
 }

 // Called every frame
 void ABase_interaction::Tick(float DeltaTime)
 {
     Super::Tick(DeltaTime);
 }

 void ABase_interaction::NotifyActorBeginOverlap(AActor* _collided_actor)
 {
     Super::NotifyActorBeginOverlap(_collided_actor);

     APawn* current_player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

     if (current_player == Cast<APawn>(_collided_actor))
         p_widget_component->SetVisibility(true);
 }

 void ABase_interaction::NotifyActorEndOverlap(AActor* _collided_actor)
 {
     Super::NotifyActorEndOverlap(_collided_actor);

     APawn* current_player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

     if (current_player == Cast<APawn>(_collided_actor))
         p_widget_component->SetVisibility(false);
 }

 void ABase_interaction::Set_UI_widget_text(FText _text)
 {
     p_widget->Title_txt->SetText(_text);
 }

 void ABase_interaction::Init_static_mesh(FString _path, FName _name)
 {
     // 메시 생성
     m_static_mesh = CreateDefaultSubobject<UStaticMeshComponent>(_name);
     RootComponent = m_static_mesh;
     m_box_collider->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

     // 경로로부터 메시 생성
     ConstructorHelpers::FObjectFinder<UStaticMesh> MESH(*_path);

     if (MESH.Succeeded())
         m_static_mesh->SetStaticMesh(MESH.Object);

     p_widget_component->AttachToComponent(m_static_mesh, FAttachmentTransformRules::KeepRelativeTransform);
 }

 void ABase_interaction::Init_skeletal_mesh(FString _path, FName _name)
 {
     // 메시 생성
     skeletal_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(_name);

     // 경로로부터 메시 생성
     ConstructorHelpers::FObjectFinder<USkeletalMesh> MESH(*_path);

     if (MESH.Succeeded())
         skeletal_mesh->SetSkeletalMesh(MESH.Object);

     p_widget_component->AttachToComponent(skeletal_mesh, FAttachmentTransformRules::KeepRelativeTransform);
 }