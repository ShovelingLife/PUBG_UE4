#include "Core_vehicle.h"
#include "Custom_player.h"
#include "Animation/AnimInstance.h"
#include "PUBG_UE4/Global.h"
#include "PUBG_UE4/Data_table_manager.h"
#include "PUBG_UE4/Custom_game_instance.h"
#include "Camera/CameraComponent.h"
#include "ChaosVehicleMovementComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ACore_vehicle::ACore_vehicle()
{
    PrimaryActorTick.bCanEverTick = true;
    AutoPossessAI = EAutoPossessAI::Disabled;
}

// Called when the game starts or when spawned
void ACore_vehicle::BeginPlay()
{
    Super::BeginPlay();
    
    // 상호작용 UI 업데이트
    AGlobal::Get()->dele_update_interaction_widget_comp.ExecuteIfBound(mp_interaction_widget_comp, FString::Printf(TEXT("%s 탑승하기"), *m_vehicle_data.type));
}

// Called every frame
void ACore_vehicle::Tick(float _delta_time)
{
    Super::Tick(_delta_time);
    mp_interaction_widget_comp->SetVisibility(is_collided);
    Update_car_pos_data();

    if(m_player)
    {
        if (m_player->current_seat_type == e_seat_type::FIRST)
            m_is_player_in_first_seat = true;

        else
            m_is_player_in_first_seat = false;
    }
}

void ACore_vehicle::SetupPlayerInputComponent(UInputComponent* _player_input_component)
{    
    Super::SetupPlayerInputComponent(InputComponent);

    // 운전석
    InputComponent->BindAxis(TEXT("Set_throttle"), this, &ACore_vehicle::Accelerate);
    InputComponent->BindAxis(TEXT("Set_brake"),    this, &ACore_vehicle::Brake);
    InputComponent->BindAxis(TEXT("Set_steering"), this, &ACore_vehicle::Handling);
    InputComponent->BindAxis(TEXT("Look_up"),      this, &ACore_vehicle::Look_up);
    InputComponent->BindAxis(TEXT("Turn"),         this, &ACore_vehicle::Turn);

    InputComponent->BindAction(TEXT("Interact"),              EInputEvent::IE_Pressed, this, &ACore_vehicle::Player_exit);
    InputComponent->BindAction(TEXT("Change_to_first_seat"),  EInputEvent::IE_Pressed, this, &ACore_vehicle::Change_to_first_seat);
    InputComponent->BindAction(TEXT("Change_to_second_seat"), EInputEvent::IE_Pressed, this, &ACore_vehicle::Change_to_second_seat);
    InputComponent->BindAction(TEXT("Change_to_third_seat"),  EInputEvent::IE_Pressed, this, &ACore_vehicle::Change_to_third_seat);
    InputComponent->BindAction(TEXT("Change_to_fourth_seat"), EInputEvent::IE_Pressed, this, &ACore_vehicle::Change_to_fourth_seat);
}

void ACore_vehicle::Init(e_vehicle_type _vehicle_type_index)
{
    AData_table_manager::arr_vehicle_data[(int)_vehicle_type_index];
    Init_skeletal_mesh();
    Init_camera();
    Init_wheeled_component();
    Init_car_pos_component();
    Init_car_pos_data();
}

void ACore_vehicle::Init_car_pos_component()
{
    // 문짝 오브젝트 초기화
    first_door_pos_comp  = CreateDefaultSubobject<USceneComponent>(TEXT("First_Door_pos"));
    second_door_pos_comp = CreateDefaultSubobject<USceneComponent>(TEXT("Second_Door_pos"));
    third_door_pos_comp  = CreateDefaultSubobject<USceneComponent>(TEXT("Third_Door_pos"));
    fourth_door_pos_comp = CreateDefaultSubobject<USceneComponent>(TEXT("Fourth_Door_pos"));

    first_door_pos_comp->SetupAttachment(RootComponent);
    second_door_pos_comp->SetupAttachment(RootComponent);
    third_door_pos_comp->SetupAttachment(RootComponent);
    fourth_door_pos_comp->SetupAttachment(RootComponent);

    // 좌석 오브젝트 초기화
    first_seat_pos_comp  = CreateDefaultSubobject<USceneComponent>(TEXT("First_Seat_pos"));
    second_seat_pos_comp = CreateDefaultSubobject<USceneComponent>(TEXT("Second_Seat_pos"));
    third_seat_pos_comp  = CreateDefaultSubobject<USceneComponent>(TEXT("Third_Seat_pos"));
    fourth_seat_pos_comp = CreateDefaultSubobject<USceneComponent>(TEXT("Fourth_Seat_pos"));

    first_seat_pos_comp->SetupAttachment(RootComponent);
    second_seat_pos_comp->SetupAttachment(RootComponent);
    third_seat_pos_comp->SetupAttachment(RootComponent);
    fourth_seat_pos_comp->SetupAttachment(RootComponent);
}

void ACore_vehicle::Init_car_pos_data()
{
    // 좌석 및 문짝 위치 초기화
    m_map_door_pos.Add(e_seat_type::FIRST, first_door_pos_comp->GetComponentLocation());
    m_map_door_pos.Add(e_seat_type::SECOND, second_door_pos_comp->GetComponentLocation());
    m_map_seat_pos.Add(e_seat_type::FIRST, first_seat_pos_comp->GetComponentLocation());
    m_map_seat_pos.Add(e_seat_type::SECOND, second_seat_pos_comp->GetComponentLocation());

    if(m_vehicle_data.max_seater == 4)
    {
        m_map_door_pos.Add(e_seat_type::THIRD, third_door_pos_comp->GetComponentLocation());
        m_map_door_pos.Add(e_seat_type::FOURTH, fourth_door_pos_comp->GetComponentLocation());
        m_map_seat_pos.Add(e_seat_type::THIRD, third_seat_pos_comp->GetComponentLocation());
        m_map_seat_pos.Add(e_seat_type::FOURTH, fourth_seat_pos_comp->GetComponentLocation());
    }
}

void ACore_vehicle::Update_car_pos_data()
{
    // 좌석 및 문짝 위치 업데이트
    m_map_door_pos[e_seat_type::FIRST]  = first_door_pos_comp->GetComponentLocation();
    m_map_door_pos[e_seat_type::SECOND] = second_door_pos_comp->GetComponentLocation();
    m_map_seat_pos[e_seat_type::FIRST]  = first_seat_pos_comp->GetComponentLocation();
    m_map_seat_pos[e_seat_type::SECOND] = second_seat_pos_comp->GetComponentLocation();

    if (m_vehicle_data.max_seater == 4)
    {
        m_map_door_pos[e_seat_type::THIRD] = third_door_pos_comp->GetComponentLocation();
        m_map_door_pos[e_seat_type::FOURTH] = fourth_door_pos_comp->GetComponentLocation();
        m_map_seat_pos[e_seat_type::THIRD] = third_seat_pos_comp->GetComponentLocation();
        m_map_seat_pos[e_seat_type::FOURTH] = fourth_seat_pos_comp->GetComponentLocation();
    }
}

void ACore_vehicle::Init_camera()
{
    // 스프링 암 초기화 및 설정
    mp_spring_arm_comp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring_arm"));
    mp_spring_arm_comp->SetupAttachment(GetMesh());
    mp_spring_arm_comp->TargetArmLength = m_vehicle_data.spring_arm_length;
    mp_spring_arm_comp->bUsePawnControlRotation = true;

    // 카메라 초기화 및 설정
    mp_camera_comp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    mp_camera_comp->SetupAttachment(mp_spring_arm_comp);
    mp_camera_comp->SetRelativeRotation(FRotator::MakeFromEuler(m_vehicle_data.camera_rotation));
    mp_camera_comp->SetWorldLocation(m_vehicle_data.camera_location);
}

void ACore_vehicle::Init_skeletal_mesh()
{
    // 경로로부터 메시 생성
    ConstructorHelpers::FObjectFinder<USkeletalMesh> MESH(*m_vehicle_data.mesh_path);

    if (MESH.Succeeded())
        GetMesh()->SetSkeletalMesh(MESH.Object);

    GetMesh()->SetSimulatePhysics(true);
    RootComponent = GetMesh();

    // 애님 인스턴스 초기화
    auto anim_instance = ConstructorHelpers::FClassFinder<UAnimInstance>(*m_vehicle_data.anim_instance_path);

    if (anim_instance.Succeeded())
        GetMesh()->SetAnimInstanceClass(anim_instance.Class);

    // 위젯 컴포넌트 초기화
    mp_interaction_widget_comp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction_widget"));
    mp_interaction_widget_comp->SetupAttachment(RootComponent);
}

void ACore_vehicle::Init_wheeled_component()
{
    UChaosWheeledVehicleMovementComponent* vehicle_move_comp = CastChecked<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());
     
    // 바퀴 설정
    auto front_wheel_bp = ConstructorHelpers::FClassFinder<UChaosVehicleWheel>(*(mk_wheel_path + "BP_Front_wheel"));
    auto rear_wheel_bp  = ConstructorHelpers::FClassFinder<UChaosVehicleWheel>(*(mk_wheel_path + "BP_Rear_wheel"));
    auto float_curve    = ConstructorHelpers::FObjectFinder<UCurveFloat>(*(mk_wheel_path + "VehicleMovementComp_ExternalCurve"));
    
    // 예외처리
    if (!front_wheel_bp.Succeeded() ||
        !rear_wheel_bp.Succeeded() ||
        !float_curve.Succeeded())
        return;

    FChaosWheelSetup arr_car_wheels[4]{ FChaosWheelSetup() };
    arr_car_wheels[0].WheelClass = front_wheel_bp.Class;
    arr_car_wheels[1].WheelClass = front_wheel_bp.Class;
    arr_car_wheels[2].WheelClass = rear_wheel_bp.Class;
    arr_car_wheels[3].WheelClass = rear_wheel_bp.Class;

    arr_car_wheels[0].BoneName = "Wheel_Front_Left";
    arr_car_wheels[1].BoneName = "Wheel_Front_Right";
    arr_car_wheels[2].BoneName = "Wheel_Rear_Left";
    arr_car_wheels[3].BoneName = "Wheel_Rear_Right";

    for (int i = 0; i < 4; i++)
        vehicle_move_comp->WheelSetups.Add(arr_car_wheels[i]);
    
    vehicle_move_comp->EngineSetup.TorqueCurve.ExternalCurve = float_curve.Object;
}

void ACore_vehicle::Player_exit()
{
    // 플레이어 초기세팅으로
    auto player_controller = UGameplayStatics::GetPlayerController(this, 0);
    player_controller->Possess(m_player);
    m_map_empty_seat[m_player->current_seat_type] = false;
    m_player->Exit_from_vehicle(m_map_door_pos[m_player->current_seat_type]);
    m_current_player_count--;
}

void ACore_vehicle::Accelerate(float _value)
{
    if (m_is_player_in_first_seat)
        GetVehicleMovementComponent()->SetThrottleInput(_value);
}

void ACore_vehicle::Brake(float _value)
{
    if (m_is_player_in_first_seat)
        GetVehicleMovementComponent()->SetBrakeInput(_value);
}

void ACore_vehicle::Handling(float _value)
{
    if (m_is_player_in_first_seat)
        GetVehicleMovementComponent()->SetSteeringInput(_value);
}

void ACore_vehicle::Look_up(float _value)
{
    AddControllerPitchInput(_value);
}

void ACore_vehicle::Turn(float _value)
{
    AddControllerYawInput(_value);
}

void ACore_vehicle::Check_for_door_pos()
{
    e_seat_type seat_type = e_seat_type::NONE;
    float arr_vec_distance[4]{ 0.f };
    bool  is_not_near_door = false;

    // 벡터 확인
    for (int i = 0; i < m_vehicle_data.max_seater; i++)
    {
        FVector player_pos   = m_player->GetActorLocation();
        FVector car_door_pos = m_map_door_pos[(e_seat_type)i];
        arr_vec_distance[i]  = FVector::Distance(player_pos.GetAbs(), car_door_pos.GetAbs());
    }
    // 문짝이 2개일 시
    if (m_vehicle_data.max_seater == 2)
    {
        // 왼쪽 첫번째 좌석
        if      (arr_vec_distance[0] < arr_vec_distance[1])
                 seat_type = e_seat_type::FIRST;

        // 왼쪽 두번째 좌석
        else if (arr_vec_distance[1] < arr_vec_distance[0])
                 seat_type = e_seat_type::SECOND;
    }
    // 문짝이 4개일 시
    else
    {
        // 왼쪽 첫번째 좌석
        if      (arr_vec_distance[0] < arr_vec_distance[1] &&
                 arr_vec_distance[0] < arr_vec_distance[2] &&
                 arr_vec_distance[0] < arr_vec_distance[3])
                 seat_type = e_seat_type::FIRST;

        // 왼쪽 두번째 좌석
        else if (arr_vec_distance[1] < arr_vec_distance[0] &&
                 arr_vec_distance[1] < arr_vec_distance[2] &&
                 arr_vec_distance[1] < arr_vec_distance[3])
                 seat_type = e_seat_type::SECOND;

        // 오른쪽 첫번째 좌석
        else if (arr_vec_distance[2] < arr_vec_distance[3] &&
                 arr_vec_distance[2] < arr_vec_distance[0] &&
                 arr_vec_distance[2] < arr_vec_distance[1])
                 seat_type = e_seat_type::THIRD;

        // 오른쪽 첫번째 좌석
        else if (arr_vec_distance[3] < arr_vec_distance[0] &&
                 arr_vec_distance[3] < arr_vec_distance[1] &&
                 arr_vec_distance[3] < arr_vec_distance[2])
                 seat_type = e_seat_type::FOURTH;
    }
    if (seat_type != e_seat_type::NONE)
    {
        m_map_empty_seat[seat_type] = true;
        m_player->current_seat_type = seat_type;
    }
}

void ACore_vehicle::Set_player_into_seat_location()
{
    auto player_controller = UGameplayStatics::GetPlayerController(this, 0);
    auto player_collider   = m_player->GetCapsuleComponent();

    // 플레이어 세팅
    m_player->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    m_player->SetActorLocation(m_map_seat_pos[m_player->current_seat_type]);

    // 플레이어 콜라이더 세팅
    player_collider->SetMobility(EComponentMobility::Static);
    player_collider->SetCollisionProfileName("OverlapAll");
    player_controller->Possess(this);
    m_current_player_count++;
}

void ACore_vehicle::Update_player_seat_location(e_seat_type _seat_type)
{
    auto player_collider = m_player->GetCapsuleComponent();
    player_collider->SetMobility(EComponentMobility::Movable);
    m_player->current_seat_type = _seat_type;
    m_player->SetActorLocation(m_map_seat_pos[_seat_type]);
    player_collider->SetMobility(EComponentMobility::Static);
}

bool ACore_vehicle::Check_available_seat(ACustom_player* _player)
{
    if (m_current_player_count < m_vehicle_data.max_seater - 1)
    {
        m_player = _player;

        // 빈 좌석 확인 후 위치 가져오기
        Check_for_door_pos();

        if (!m_map_empty_seat[_player->current_seat_type])
            return false;

        Set_player_into_seat_location();
        return true;
    }
    return false;
}