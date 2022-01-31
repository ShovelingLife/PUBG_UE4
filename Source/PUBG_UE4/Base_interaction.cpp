#include "Base_interaction.h"
#include "Data_table_manager.h"
#include "Custom_game_instance.h"
#include "Global.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

 ABase_interaction::ABase_interaction()
 {
     PrimaryActorTick.bCanEverTick = true;
     RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root_comp"));
     
     // 박스 콜라이더 초기화
     m_box_collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision_comp"));
     m_box_collider->SetupAttachment(RootComponent);

     // UI 위젯 컴포넌트 초기화
     mp_interaction_widget_comp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction_widget"));
     mp_interaction_widget_comp->SetupAttachment(RootComponent);
 }

 void ABase_interaction::BeginPlay()
 {
     Super::BeginPlay();
     Init_interaction_UI();
     m_box_collider->BodyInstance.SetCollisionProfileName("Object");
     m_box_collider->BodyInstance.bNotifyRigidBodyCollision = false;
 }

 // Called every frame
 void ABase_interaction::Tick(float _delta_time)
 {
     Super::Tick(_delta_time);

     if (is_player_near)
         m_current_time += _delta_time;

     else
         m_current_time = 0.f;

     // 0.25초 지날 시 UI설정
     if (m_current_time > 0.25f)
         mp_interaction_widget_comp->SetVisibility(true);

     else
         mp_interaction_widget_comp->SetVisibility(false);
 }

 void ABase_interaction::Init_static_mesh(FString _path, FName _name)
 {
     // 메시 생성
     p_static_mesh_comp = CreateDefaultSubobject<UStaticMeshComponent>(_name);
     p_static_mesh_comp->SetupAttachment(RootComponent);

     // 경로로부터 메시 생성
     ConstructorHelpers::FObjectFinder<UStaticMesh> MESH(*_path);

     if (MESH.Succeeded())
         p_static_mesh_comp->SetStaticMesh(MESH.Object);
 }

 void ABase_interaction::Init_skeletal_mesh(FString _path, FName _name)
 {
     // 메시 생성
     p_skeletal_mesh_comp = CreateDefaultSubobject<USkeletalMeshComponent>(_name);
     p_skeletal_mesh_comp->SetupAttachment(RootComponent);

     // 경로로부터 메시 생성
     ConstructorHelpers::FObjectFinder<USkeletalMesh> MESH(*_path);

     if (MESH.Succeeded())
         p_skeletal_mesh_comp->SetSkeletalMesh(MESH.Object);
 }

 void ABase_interaction::Init_audio()
 {
     // 오디오 포인터에 대한 생성
     p_audio_comp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
     p_audio_comp->SetupAttachment(RootComponent);
 }

 void ABase_interaction::Init_interaction_UI()
 {
     AGlobal::Get()->dele_update_interaction_widget_comp.ExecuteIfBound(mp_interaction_widget_comp, FString::Printf(TEXT("%s 줍기"), *object_type));
 }