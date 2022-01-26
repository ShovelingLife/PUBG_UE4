#include "Custom_player.h"
#include "Core_vehicle.h"
#include "PUBG_gamemode.h"
#include "Player_weapons/Core_weapon.h"
#include "Player_weapons/Core_melee_weapon.h"
#include "Player_weapons/Core_throwable_weapon.h"
#include "Player_weapons/Core_bullet.h"
#include "Player_weapons/Weapon_manager.h"
#include "AI_PUBG/AI_character.h"
#include "PUBG_UE4/Global.h" 
#include "PUBG_UE4/My_enum.h"
#include "PUBG_UE4/Sound_manager.h" 
#include "PUBG_UE4/Custom_game_instance.h" 
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Framework/Application/SlateApplication.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
ACustom_player::ACustom_player()
{
    PrimaryActorTick.bCanEverTick = true;
    RootComponent = GetCapsuleComponent();
    Update_player_settings();
    Init_camera_comp();
    Init_mesh_comp();
    Init_audio_comp();
    Init_anim_instance();
    Init_particle_system();
}

void ACustom_player::BeginPlay()
{
    Super::BeginPlay();

    // 무기 매니저 생성
    mp_weapon_manager = GetWorld()->SpawnActor<AWeapon_manager>(AWeapon_manager::StaticClass());
    mp_weapon_manager->GetRootComponent()->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void ACustom_player::Tick(float _delta_time)
{
    Super::Tick(_delta_time);

    if (!mp_sound_manager)
        mp_sound_manager = AGlobal::Get_sound_manager();

    Check_if_moving();
    Check_if_is_vehicle_near();
    //Play_walk_sound();
    Check_for_object();
    Try_to_get_collided_component();
}

void ACustom_player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(InputComponent);

    InputComponent->BindAxis(FName(TEXT("Up_down")), this, &ACustom_player::Move_forward_back);
    InputComponent->BindAxis(FName(TEXT("Left_right")), this, &ACustom_player::Move_left_right);
    InputComponent->BindAxis(FName(TEXT("Look_up")), this, &ACustom_player::Look_up);
    InputComponent->BindAxis(FName(TEXT("Turn")), this, &ACustom_player::Turn);

    // 이동 관련
    InputComponent->BindAction(FName(TEXT("Jump")), IE_Pressed, this, &ACustom_player::Custom_jump);
    InputComponent->BindAction(FName(TEXT("Crouch")), IE_Pressed, this, &ACustom_player::Custom_crouch);
    InputComponent->BindAction(FName(TEXT("Sprint")), IE_Pressed, this, &ACustom_player::Begin_sprint);
    InputComponent->BindAction(FName(TEXT("Sprint")), IE_Released, this, &ACustom_player::End_sprint);
    InputComponent->BindAction(FName(TEXT("Aim")), IE_Pressed, this, &ACustom_player::Aim);

    // 무기 관련
    InputComponent->BindAction(FName(TEXT("Equip_first_weapon")), IE_Pressed, this,  &ACustom_player::Equip_first_weapon);
    InputComponent->BindAction(FName(TEXT("Equip_second_weapon")), IE_Pressed, this, &ACustom_player::Equip_second_weapon);
    InputComponent->BindAction(FName(TEXT("Equip_third_weapon")), IE_Pressed, this, &ACustom_player::Equip_third_weapon);
    InputComponent->BindAction(FName(TEXT("Equip_fourth_weapon")), IE_Pressed, this, &ACustom_player::Equip_fourth_weapon);
    InputComponent->BindAction(FName(TEXT("Equip_fifth_weapon")), IE_Pressed, this, &ACustom_player::Equip_fifth_weapon);
    InputComponent->BindAction(FName(TEXT("Change_shoot_mode")), IE_Pressed, this, &ACustom_player::Change_shoot_mode);
    InputComponent->BindAction(FName(TEXT("Swap_scrolling_up")), IE_Pressed, this, &ACustom_player::Swap_scrolling_up);
    InputComponent->BindAction(FName(TEXT("Swap_scrolling_down")), IE_Pressed, this, &ACustom_player::Swap_scrolling_down);
    InputComponent->BindAction(FName(TEXT("Shoot")), IE_Pressed, this, &ACustom_player::Begin_shooting);
    InputComponent->BindAction(FName(TEXT("Shoot")), IE_Released, this, &ACustom_player::End_shooting);
    InputComponent->BindAction(FName(TEXT("Reload")), IE_Pressed, this, &ACustom_player::Reload);
    InputComponent->BindAction(FName(TEXT("Proning")), IE_Pressed, this, &ACustom_player::Proning);
    InputComponent->BindAction(FName(TEXT("Interact")), IE_Pressed, this, &ACustom_player::Begin_interact);
    InputComponent->BindAction(FName(TEXT("Interact")), IE_Released, this, &ACustom_player::End_interact);
    InputComponent->BindAction(FName(TEXT("Inventory")), IE_Pressed, this, &ACustom_player::Open_inventory);
}

void ACustom_player::Update_player_settings()
{
    // 플레이어 설정
    GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
    GetCharacterMovement()->MaxWalkSpeed = 350.f;
}

void ACustom_player::Init_camera_comp()
{
    // 카메라 컴포넌트 초기화
    p_spring_arm_comp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring_arm"));
    p_camera_comp     = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

    // 카메라를 부모 컴포넌트에 부착
    p_spring_arm_comp->SetupAttachment(GetCapsuleComponent());
    p_camera_comp->SetupAttachment(p_spring_arm_comp);

    // 카메라 설정
    AGlobal* p_global = AGlobal::Get();
    p_spring_arm_comp->TargetArmLength = p_global->player_spring_arm_length;
    p_spring_arm_comp->SetRelativeRotation(p_global->player_spring_arm_rotation);
    p_spring_arm_comp->SetWorldLocation(p_global->player_spring_arm_location);
    p_spring_arm_comp->bUsePawnControlRotation = true;
}

void ACustom_player::Init_mesh_comp()
{
    // 메쉬 초기화
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/Characters/UE4_Mannequin/Mesh/SK_Mannequin"));

    if (SK_MANNEQUIN.Succeeded())
        GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);

    GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
}

void ACustom_player::Init_audio_comp()
{
    mp_audio_comp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio_comp"));
    mp_audio_comp->SetupAttachment(RootComponent);
}

void ACustom_player::Init_anim_instance()
{
    // 애니메이션 초기화
    static ConstructorHelpers::FClassFinder<UAnimInstance> BP_ANIM(TEXT("/Game/Blueprints/Animations/BP_Player_anim_instance"));

    if (BP_ANIM.Succeeded())
        GetMesh()->SetAnimInstanceClass(BP_ANIM.Class);
}

void ACustom_player::Init_particle_system()
{

}

void ACustom_player::Check_if_moving()
{
    AGlobal* p_global = AGlobal::Get();

    // 움직이고 있지 않음
    if (GetVelocity().IsZero())
    {
        if (m_is_moving)
        {
            if (current_state == e_player_state::CROUCH_WALK)
                current_state = e_player_state::CROUCH;

            else if (current_state == e_player_state::PRONING_WALK)
                current_state = e_player_state::PRONING;

            else if (current_state == e_player_state::AIM_WALK)
                current_state = e_player_state::AIM;

            else
                current_state = e_player_state::IDLE;
        }
        m_is_moving = false;
        is_sprinting = false;
    }
    else
    {
        // 숙이고 있음
        if (current_state == e_player_state::CROUCH ||
            current_state == e_player_state::CROUCH_AIM)
            current_state = e_player_state::CROUCH_WALK;

        // 엎드리고 있음
        else if (current_state == e_player_state::PRONING)
            current_state = e_player_state::PRONING_WALK;

        else if (current_state == e_player_state::AIM)
            current_state = e_player_state::AIM_WALK;

        else
        {
            // 떨어지고 있음
            if (GetCharacterMovement()->IsFalling())
            {
                // 뛰면서 점프후 착지
                if (m_sprint_multiplier > 1.f)
                {
                    is_sprinting = true;
                    current_state = e_player_state::SPRINT_JUMP;
                }
                else // 점프함
                    current_state = e_player_state::JUMP;
            }
            else // 지면에 닿고있음
            {
                if (current_state == e_player_state::SPRINT_JUMP)
                    current_state = e_player_state::SPRINT;

                // 뛰고있음
                else if (current_state == e_player_state::SPRINT)
                {
                    if (current_oxygen < 0)
                    {
                        current_state = e_player_state::IDLE;
                        m_sprint_multiplier = 1;
                        GetCharacterMovement()->MaxWalkSpeed = 350.f;
                        is_sprinting = false;
                        return;
                    }
                    is_sprinting = true;

                    if (m_sprint_multiplier < 1.75f)
                    {
                        m_sprint_multiplier += 0.25f;
                        GetCharacterMovement()->MaxWalkSpeed *= m_sprint_multiplier;
                    }
                }
                // 
                else if (current_state == e_player_state::IDLE ||
                         current_state == e_player_state::JUMP)
                         current_state = e_player_state::WALK;
            }
        }
        m_is_moving = true;
    }
}

void ACustom_player::Check_for_object()
{
    FVector    direction = GetActorForwardVector() * 50;
    FVector    begin_pos = GetMesh()->GetSocketLocation("detect_object_ray_sock");
    FVector    end_pos = begin_pos + direction;
    FHitResult hit_result;

    GetWorld()->LineTraceSingleByProfile(hit_result, begin_pos, end_pos, "Object");

    AActor* hitted_actor = hit_result.GetActor();

    // 충돌한 오브젝트가 있을 시
    if (hitted_actor)
    {
        // 충돌한 오브젝트가 무기일 시
        if (hitted_actor->IsA<ACore_weapon>() ||
            hitted_actor->IsA<ACore_throwable_weapon>() ||
            hitted_actor->IsA<ACore_melee_weapon>())
        {
            m_collided_weapon = hitted_actor;
            Set_item_UI(true);
        }
    }
    else
        Set_item_UI(false);
}

void ACustom_player::Try_to_get_collided_component()
{
    if (m_is_interacting)
    {
        // 무기랑 충돌 시
        if (m_collided_weapon)
        {
            mp_sound_manager->Play_player_sound(mp_audio_comp, e_player_sound_type::WEAPON_EQUIP);
            mp_weapon_manager->Equip(m_collided_weapon);
            Set_item_UI(false);
        }
        if (m_collided_vehicle)
        {
            //  차량 탑승 상태
            if (m_collided_vehicle->Check_available_seat(this))
            {
                m_collided_vehicle->is_collided = false;
                is_in_vehicle = true;
            }
            m_collided_vehicle = nullptr;
        }
    }
}

void ACustom_player::Check_if_is_vehicle_near()
{
    if (is_in_vehicle)
        return;

    APlayerCameraManager* camera_manager = UGameplayStatics::GetPlayerCameraManager(this, 0);
    FVector               begin_pos      = GetActorLocation();
    FVector               forward_vec    = GetActorForwardVector() * 50;
    FHitResult            hit_result;
    FVector               end_pos        = begin_pos + forward_vec;
    auto                  p_world        = GetWorld();
    p_world->LineTraceSingleByObjectType(hit_result, begin_pos, end_pos, FCollisionObjectQueryParams(ECC_Vehicle));
    //DrawDebugLine(p_world, begin_pos, end_pos, FColor::Red, true, 5.f, (uint8)0U, 1.f);

    // 차량을 감지
    AActor* hitted_actor = hit_result.GetActor();

    if (!hitted_actor)
    {
        if (m_collided_vehicle)
        {
            m_collided_vehicle->is_collided = false;
            m_collided_vehicle              = nullptr;
        }
        return;
    }
    if(hitted_actor->IsA(ACore_vehicle::StaticClass()))
    {
        m_collided_vehicle = Cast<ACore_vehicle>(hitted_actor);
        
        if (m_collided_vehicle)
            m_collided_vehicle->is_collided = true;
    }
}

void ACustom_player::Custom_jump()
{
    if (current_state == e_player_state::CROUCH ||
        current_state == e_player_state::PRONING)
        return;

    Jump();
}

void ACustom_player::Move_forward_back(float _value)
{
    if (is_animation_playing)
        return;

    AddMovementInput(GetActorForwardVector() * _value * m_sprint_multiplier);
}

void ACustom_player::Move_left_right(float _value)
{
    if (is_animation_playing)
        return;

    AddMovementInput(GetActorRightVector() * _value * m_sprint_multiplier);
}

void ACustom_player::Look_up(float _value)
{
    //AddControllerPitchInput(_value);
    if (!m_is_inventory_opened)
        AddControllerPitchInput(_value);
}

void ACustom_player::Turn(float _value)
{
    //AddControllerYawInput(_value);
    if (!m_is_inventory_opened)
        AddControllerYawInput(_value);
}

void ACustom_player::Custom_crouch()
{
    if (m_is_moving ||
        GetCharacterMovement()->IsFalling())
        return;

    p_spring_arm_comp->SetRelativeLocation(FVector(0.f, 0.f, 80.f));

    // 숙이고 있음
    if (GetCharacterMovement()->IsCrouching())
    {
        if (current_state == e_player_state::CROUCH_AIM)
            current_state = e_player_state::AIM;

        else
            current_state = e_player_state::IDLE;

        UnCrouch();
    }
    else
    {
        if (current_state == e_player_state::AIM)
            current_state = e_player_state::CROUCH_AIM;

        else
            current_state = e_player_state::CROUCH;

        Crouch();
    }
}

void ACustom_player::Proning()
{
    switch (current_state)
    {
    case e_player_state::IDLE:
        current_state = e_player_state::PRONING;
        p_spring_arm_comp->SetRelativeLocation(FVector(0.f, 0.f, -50.f));
        break;

    case e_player_state::CROUCH:
    case e_player_state::CROUCH_WALK:
        current_state = e_player_state::PRONING;
        p_spring_arm_comp->SetRelativeLocation(FVector(0.f, 0.f, -50.f));
        UnCrouch();
        break;

    case e_player_state::AIM:
        current_state = e_player_state::PRONING_AIM;
        break;

    case e_player_state::PRONING:
    case e_player_state::PRONING_WALK:
        current_state = e_player_state::IDLE;
        p_spring_arm_comp->SetRelativeLocation(FVector(0.f, 0.f, 80.f));
        break;
    }
}

void ACustom_player::Begin_sprint()
{
    if (!GetVelocity().IsZero())
        current_state = e_player_state::SPRINT;
}

void ACustom_player::End_sprint()
{
    current_state                        = e_player_state::IDLE;
    m_sprint_multiplier                  = 1;
    GetCharacterMovement()->MaxWalkSpeed = 350.f;
}

void ACustom_player::Begin_shooting()
{
    //mp_weapon_manager->is_shooting = true;
}

void ACustom_player::End_shooting()
{
    //mp_weapon_manager->is_shooting = false;
}

void ACustom_player::Reload()
{
}

void ACustom_player::Aim()
{
    bool is_first_equipped  = false;
    bool is_second_equipped = false;
    int  code               = 0;

    //Verify_equipped_weapon(is_first_equipped, is_second_equipped);

    // 무기 장착하고 있지 않은 상태
    if (!is_first_equipped &&
        !is_second_equipped)
        return;

    switch (current_state)
    {
    // 기본자세 > 에임
    case e_player_state::IDLE:        current_state = e_player_state::AIM;          code = 1; break;
    case e_player_state::CROUCH:      current_state = e_player_state::CROUCH_AIM;   code = 1; break;
    case e_player_state::PRONING:     current_state = e_player_state::PRONING_AIM;  code = 1; break;

    // 에임 > 기본 자세
    case e_player_state::AIM:         current_state = e_player_state::IDLE;         code = 0; break;
    case e_player_state::CROUCH_AIM:  current_state = e_player_state::CROUCH;       code = 0; break;
    case e_player_state::PRONING_AIM: current_state = e_player_state::PRONING;      code = 0; break;
    }
    is_aiming = (bool)code;
    //Changing_aim_pose(code);
}

void ACustom_player::Change_shoot_mode()
{
    mp_weapon_manager->Change_shoot_mode();
}

void ACustom_player::Swap_scrolling_up()
{
    if (mp_weapon_manager->Scroll_select(1))
        mp_sound_manager->Play_player_sound(mp_audio_comp, e_player_sound_type::WEAPON_SWAP);
}

void ACustom_player::Swap_scrolling_down()
{
    if (mp_weapon_manager->Scroll_select(-1))
        mp_sound_manager->Play_player_sound(mp_audio_comp, e_player_sound_type::WEAPON_SWAP);
}

void ACustom_player::Equip_first_weapon()
{
    if (mp_weapon_manager->Check_if_weapon_available(e_current_weapon_type::FIRST))    
        mp_sound_manager->Play_player_sound(mp_audio_comp, e_player_sound_type::WEAPON_SWAP);
}

void ACustom_player::Equip_second_weapon()
{
    if (mp_weapon_manager->Check_if_weapon_available(e_current_weapon_type::SECOND))
        mp_sound_manager->Play_player_sound(mp_audio_comp, e_player_sound_type::WEAPON_SWAP);
}

void ACustom_player::Equip_third_weapon()
{
    if (mp_weapon_manager->Check_if_weapon_available(e_current_weapon_type::PISTOL))
        mp_sound_manager->Play_player_sound(mp_audio_comp, e_player_sound_type::WEAPON_SWAP);
}

void ACustom_player::Equip_fourth_weapon()
{
    if (mp_weapon_manager->Check_if_weapon_available(e_current_weapon_type::MELEE))
        mp_sound_manager->Play_player_sound(mp_audio_comp, e_player_sound_type::WEAPON_SWAP);
}

void ACustom_player::Equip_fifth_weapon()
{
    if (mp_weapon_manager->Check_if_weapon_available(e_current_weapon_type::THROWABLE))
        mp_sound_manager->Play_player_sound(mp_audio_comp, e_player_sound_type::WEAPON_SWAP);
}

void ACustom_player::Set_item_UI(bool _is_player_near)
{
    // 현재 무기가 있을 시
    if (m_collided_weapon)
    {
        Cast<ABase_interaction>(m_collided_weapon)->is_player_near = _is_player_near;

        if (!_is_player_near)
            m_collided_weapon = nullptr;
    }
}

void ACustom_player::Exit_from_vehicle(FVector _exit_location)
{
    // 플레이어 충돌체 관련
    auto capsule_component = GetCapsuleComponent();
    capsule_component->SetMobility(EComponentMobility::Movable);
    capsule_component->SetCollisionProfileName("Pawn");
    capsule_component->SetEnableGravity(true);

    // 플레이어 위치 및 카메라 위치
    current_seat_type = e_seat_type::NONE;
    is_in_vehicle     = false;
    SetActorLocation(_exit_location);
    p_spring_arm_comp->SetRelativeLocation(AGlobal::Get()->player_spring_arm_location);
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void ACustom_player::Open_inventory()
{
    if (!m_is_inventory_opened)
    {
        dele_open_inventory.ExecuteIfBound();
        m_is_inventory_opened = true;
    }
    else
    {
        p_spring_arm_comp->SetRelativeRotation(AGlobal::Get()->player_spring_arm_rotation);
        dele_close_inventory.ExecuteIfBound();
        m_is_inventory_opened = false;
    }
}