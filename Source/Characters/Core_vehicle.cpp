#include "Core_vehicle.h"
#include "Custom_player.h"
#include "UI_PUBG/Interaction_UI.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "WheeledVehicleMovementComponent.h"

// Sets default values
ACore_vehicle::ACore_vehicle()
{
    PrimaryActorTick.bCanEverTick = true;
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = nullptr;
    Init_default_components();
}

// Called when the game starts or when spawned
void ACore_vehicle::BeginPlay()
{
    Super::BeginPlay();

    FString str = FString::Printf(TEXT("%s 열기"), *m_vehicle_data.type);

    // 위젯 설정
    if (widget_bp_class)
    {
        p_widget_component->SetWidgetClass(widget_bp_class);
        p_widget_component->SetRelativeLocation(FVector::ZeroVector);
        p_widget_component->SetWidgetSpace(EWidgetSpace::Screen);

        auto p_interaction_ui = Cast<UInteraction_UI>(p_widget_component->GetWidget());
        p_interaction_ui->Title_txt->SetText(FText::FromString(str));
    }
}

// Called every frame
void ACore_vehicle::Tick(float _delta_time)
{
    Super::Tick(_delta_time);

    if (!m_move_pos.IsNearlyZero())
    {
        const FVector new_pos = GetActorLocation() + (m_move_pos * _delta_time * m_move_speed);
        SetActorLocation(new_pos);
    }
}

void ACore_vehicle::NotifyActorBeginOverlap(AActor* _collided_actor)
{
    Super::NotifyActorBeginOverlap(_collided_actor);
    m_player = Cast<ACustom_player>(_collided_actor);

    auto player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    if (m_player)
        p_widget_component->SetVisibility(true);
}

void ACore_vehicle::NotifyActorEndOverlap(AActor* _collided_actor)
{
    Super::NotifyActorEndOverlap(_collided_actor);
    p_widget_component->SetVisibility(false);
}

void ACore_vehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // F키
    PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &ACore_vehicle::Player_exit);

    // 이동
    PlayerInputComponent->BindAxis(TEXT("Up_down"), this, &ACore_vehicle::Move_up_down);
    PlayerInputComponent->BindAxis(TEXT("Left_right"), this, &ACore_vehicle::Move_left_right);
}

void ACore_vehicle::Init(e_vehicle_type _vehicle_type_index)
{
    Load_from_csv_vehicle(_vehicle_type_index, m_vehicle_data);
    Init_skeletal_mesh("Vehicles");
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

void ACore_vehicle::Load_from_csv_vehicle(e_vehicle_type _index, Fs_vehicle_data& _vehicle_data)
{
    ConstructorHelpers::FClassFinder<AActor> DATA_TABLE_MANAGER(TEXT("Blueprint'/Game/Blueprints/Managers/BP_Data_table_manager.BP_Data_table_manager_C'"));
    AData_table_manager* p_data_table_manager = nullptr;

    if (DATA_TABLE_MANAGER.Succeeded())
        p_data_table_manager = Cast<AData_table_manager>(DATA_TABLE_MANAGER.Class->GetDefaultObject());

    if (!p_data_table_manager)
        return;

    _vehicle_data = p_data_table_manager->Get_vehicle_data((int)_index);
}

void ACore_vehicle::Init_default_components()
{
    m_box_collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
    RootComponent = m_box_collider;

    // 위젯 컴포넌트 초기화
    p_widget_component = CreateDefaultSubobject<UWidgetComponent>(TEXT("UI"));
    p_widget_component->SetVisibility(false);
    auto bp_class = ConstructorHelpers::FClassFinder<UInteraction_UI>(TEXT("/Game/Blueprints/UI/BP_Interaction_UI"));
    widget_bp_class = bp_class.Class;
}

void ACore_vehicle::Init_skeletal_mesh(FString _name)
{
    // 메시 생성
    skeletal_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(*_name);
    skeletal_mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
    p_widget_component->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void ACore_vehicle::Player_exit()
{
    if (!m_player)
        return;

    // 차량 > 플레이어 전환
    auto current_controller = UGameplayStatics::GetPlayerController(this, 0);
    current_controller->UnPossess();
    current_controller->Possess(m_player);
    m_player->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    m_current_player_count--;
}

void ACore_vehicle::Move_up_down(float _value)
{
    GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Cyan, "Pressed key");
    GetVehicleMovementComponent()->SetThrottleInput(_value);
}

void ACore_vehicle::Move_left_right(float _value)
{
    GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Cyan, "Pressed key");
    GetVehicleMovementComponent()->SetSteeringInput(_value);
}

bool ACore_vehicle::Check_if_seat_available(ACustom_player* _player)
{
    if (m_current_player_count < m_vehicle_data.max_seater - 1)
    {
        // 1. 플레이어 위치 / 2. 카메라 위치
        auto player_seatpos = m_vehicle_data.arr_player_seat_pos[m_current_player_count++];
        _player->SetActorRelativeLocation(player_seatpos.first);
        _player->p_spring_arm->SetRelativeLocation(player_seatpos.second);
        _player->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
        return true;
    }
    return false;
}