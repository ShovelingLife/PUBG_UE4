// Fill out your copyright notice in the Description page of Project Settings.


#include "Core_vehicle.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ACore_vehicle::ACore_vehicle()
{
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACore_vehicle::BeginPlay()
{
    Super::BeginPlay();

    FString str = FString::Printf(TEXT("%s 열기"), *m_vehicle_type);
    ABase_interaction_pawn::Set_UI_widget_text(FText::FromString(str));
    p_widget_component->SetVisibility(false);
}

void ACore_vehicle::NotifyActorBeginOverlap(AActor* _collided_actor)
{
    Super::NotifyActorBeginOverlap(_collided_actor);
}

void ACore_vehicle::NotifyActorEndOverlap(AActor* _collided_actor)
{
    Super::NotifyActorEndOverlap(_collided_actor);
}

void ACore_vehicle::Init(e_vehicle_type _vehicle_type_index)
{
    ABase_interaction_pawn::Load_from_csv_vehicle(_vehicle_type_index, m_vehicle_data);
    ABase_interaction_pawn::Init_skeletal_mesh("Vehicles");
    Init_interaction_collider();
    Update_mesh();
    Init_camera();
}

void ACore_vehicle::Init_camera()
{
    // 스프링 암 초기화 및 설정
    mp_spring_arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring_arm"));
    mp_spring_arm->SetupAttachment(skeletal_mesh);
    mp_spring_arm->AttachToComponent(skeletal_mesh, FAttachmentTransformRules::KeepRelativeTransform);
    mp_spring_arm->TargetArmLength = m_vehicle_data.spring_arm_length;
    mp_spring_arm->bUsePawnControlRotation = true;

    // 카메라 초기화 및 설정
    mp_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    mp_camera->SetupAttachment(mp_spring_arm);
    mp_camera->SetRelativeRotation(FRotator::MakeFromEuler(m_vehicle_data.camera_rotation));
    mp_camera->SetWorldLocation(m_vehicle_data.camera_location);
}

void ACore_vehicle::Update_mesh()
{
    m_mesh_path = m_vehicle_data.mesh_path;

    // 경로로부터 메시 생성
    ConstructorHelpers::FObjectFinder<USkeletalMesh> MESH(*m_mesh_path);
    
    if (MESH.Succeeded())
        skeletal_mesh->SetSkeletalMesh(MESH.Object);
}

void ACore_vehicle::Init_interaction_collider()
{
    m_box_collider->AddLocalOffset(m_vehicle_data.collider_pos);
    m_box_collider->SetBoxExtent(m_vehicle_data.collider_size);

    // 상호작용 콜라이더 초기화
    mp_interation_collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction collider"));
    mp_interation_collider->AddLocalOffset(m_vehicle_data.collider_pos);
    mp_interation_collider->SetBoxExtent(m_vehicle_data.collider_size);
}

// Called every frame
void ACore_vehicle::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}