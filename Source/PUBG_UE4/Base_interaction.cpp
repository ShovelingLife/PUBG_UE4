#include "Base_interaction.h"
#include "Data_table_manager.h"
#include "Custom_game_instance.h"
#include "Global.h"
#include "UI_manager.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

 ABase_interaction::ABase_interaction()
 {
     // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
     PrimaryActorTick.bCanEverTick = true;

     // 박스 콜라이더 초기화
     m_box_collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
     RootComponent  = m_box_collider;

     // UI 위젯 컴포넌트 초기화
     mp_interaction_widget_comp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction_widget"));
     mp_interaction_widget_comp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
 }

 // Called when the game starts or when spawned
 void ABase_interaction::BeginPlay()
 {
     Super::BeginPlay();
     Init_interaction_UI();
 }

 // Called every frame
 void ABase_interaction::Tick(float DeltaTime)
 {
     Super::Tick(DeltaTime);
     mp_interaction_widget_comp->SetVisibility(is_player_near);
 }

 void ABase_interaction::NotifyActorBeginOverlap(AActor* _collided_actor)
 {
     Super::NotifyActorBeginOverlap(_collided_actor);

     APawn* current_player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

     if (current_player == Cast<APawn>(_collided_actor))
         is_player_near = true;
 }

 void ABase_interaction::NotifyActorEndOverlap(AActor* _collided_actor)
 {
     Super::NotifyActorEndOverlap(_collided_actor);

     APawn* current_player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

     if (current_player == Cast<APawn>(_collided_actor))
         is_player_near = false;
 }

 void ABase_interaction::Init_static_mesh(FString _path, FName _name)
 {
     // 메시 생성
     m_static_mesh = CreateDefaultSubobject<UStaticMeshComponent>(_name);
     m_static_mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

     // 경로로부터 메시 생성
     ConstructorHelpers::FObjectFinder<UStaticMesh> MESH(*_path);

     if (MESH.Succeeded())
         m_static_mesh->SetStaticMesh(MESH.Object);
 }

 void ABase_interaction::Init_skeletal_mesh(FString _path, FName _name)
 {
     // 메시 생성
     skeletal_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(_name);
     skeletal_mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

     // 경로로부터 메시 생성
     ConstructorHelpers::FObjectFinder<USkeletalMesh> MESH(*_path);

     if (MESH.Succeeded())
         skeletal_mesh->SetSkeletalMesh(MESH.Object);
 }

 void ABase_interaction::Init_interaction_UI()
 {
     auto p_ui_manager = AGlobal::Get_UI_manager();
     p_ui_manager->Update_interaction_UI(mp_interaction_widget_comp, FString::Printf(TEXT("%s 줍기"), *m_object_type));
 }