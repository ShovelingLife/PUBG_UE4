#include "Custom_player.h"
#include "Core_vehicle.h"
#include "Player_weapons/Core_weapon.h"
#include "Player_weapons/Core_bullet.h"
#include "AI_PUBG/AI_character.h"
#include "PUBG_UE4/Global.h"
#include "PUBG_UE4/UI_manager.h"
#include "PUBG_UE4/Sound_manager.h" 
#include "UI_PUBG/Inventory_manager.h"
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

    Init_player_settings();
    Init_camera_settings();
    Init_mesh_settings();
    Init_animation_settings();
    Init_particle_system();
    Init_managers();
}

void ACustom_player::OnOverlapBegin(class UPrimitiveComponent* _overlapped_comp, class AActor* _other_actor, class UPrimitiveComponent* _other_comp, int32 _other_body_index, bool _is_from_sweep, const FHitResult& _sweep_result)
{
    m_collided_weapon     = Cast<ACore_weapon>(_other_actor);
    is_detected_collision = true;
}

void ACustom_player::OnOverlapEnd(class UPrimitiveComponent* _overlapped_comp, class AActor* _other_actor, class UPrimitiveComponent* _other_comp, int32 _other_body_index)
{
    is_detected_collision = false;
}

void ACustom_player::BeginPlay()
{
    Super::BeginPlay();
    mp_UI_manager = GetWorld()->SpawnActor<AUI_manager>(AUI_manager::StaticClass(), FTransform::Identity);
}

void ACustom_player::Tick(float _delta_time)
{
    Super::Tick(_delta_time);
    Check_if_moving();
    Check_if_reloading(_delta_time);
    Check_if_is_vehicle_near();
    //Play_walk_sound();
    Update_weapon_pos();

    if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(EKeys::LeftMouseButton))
        Check_continously_shooting(_delta_time);

    if (is_detected_collision)
        Try_to_get_collided_component();
}

void ACustom_player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(InputComponent);

    InputComponent->BindAxis(FName(TEXT("Up_down")), this, &ACustom_player::Move_up_down);
    InputComponent->BindAxis(FName(TEXT("Left_right")), this, &ACustom_player::Move_left_right);
    InputComponent->BindAxis(FName(TEXT("Look_up")), this, &ACustom_player::Look_up);
    InputComponent->BindAxis(FName(TEXT("Turn")), this, &ACustom_player::Turn);

    InputComponent->BindAction(FName(TEXT("Jump")), IE_Pressed, this, &ACustom_player::Custom_jump);
    InputComponent->BindAction(FName(TEXT("Crouch")), IE_Pressed, this, &ACustom_player::Custom_crouch);
    InputComponent->BindAction(FName(TEXT("Sprint")), IE_Pressed, this, &ACustom_player::Begin_sprint);
    InputComponent->BindAction(FName(TEXT("Sprint")), IE_Released, this, &ACustom_player::End_sprint);
    InputComponent->BindAction(FName(TEXT("Aim")), IE_Pressed, this, &ACustom_player::Aim);
    InputComponent->BindAction(FName(TEXT("Equip_first_weapon")), IE_Pressed, this, &ACustom_player::Equip_first_weapon);
    InputComponent->BindAction(FName(TEXT("Equip_second_weapon")), IE_Pressed, this, &ACustom_player::Equip_second_weapon);
    InputComponent->BindAction(FName(TEXT("Change_shoot_mode")), IE_Pressed, this, &ACustom_player::Change_shoot_mode);
    InputComponent->BindAction(FName(TEXT("Shoot")), IE_Pressed, this, &ACustom_player::Begin_shooting);
    InputComponent->BindAction(FName(TEXT("Shoot")), IE_Released, this, &ACustom_player::End_shooting);
    InputComponent->BindAction(FName(TEXT("Reload")), IE_Pressed, this, &ACustom_player::Reload);
    InputComponent->BindAction(FName(TEXT("Proning")), IE_Pressed, this, &ACustom_player::Proning);
    InputComponent->BindAction(FName(TEXT("Interact")), IE_Pressed, this, &ACustom_player::Begin_interact);
    InputComponent->BindAction(FName(TEXT("Interact")), IE_Released, this, &ACustom_player::End_interact);
    InputComponent->BindAction(FName(TEXT("Swap_weapon")), IE_Pressed, this, &ACustom_player::Swap_weapon);
    InputComponent->BindAction(FName(TEXT("Inventory")), IE_Pressed, this, &ACustom_player::Open_inventory);
}

void ACustom_player::Init_player_settings()
{
    RootComponent = GetCapsuleComponent();
    GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
    //GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

    // ?띾룄 ?쒗븳
    GetCharacterMovement()->MaxWalkSpeed = 350.f;
    GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACustom_player::OnOverlapBegin);
    GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ACustom_player::OnOverlapEnd);
}

void ACustom_player::Init_camera_settings()
{
    // 카메라 컴포넌트 초기화
    p_spring_arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring_arm"));
    p_camera     = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

    // 카메라를 부모 컴포넌트에 부착
    p_spring_arm->SetupAttachment(reinterpret_cast<USceneComponent*>(GetCapsuleComponent()));
    p_camera->SetupAttachment(p_spring_arm);

    // 카메라 설정
    p_spring_arm->TargetArmLength = AGlobal::player_spring_arm_length;
    p_spring_arm->SetRelativeRotation(AGlobal::player_spring_arm_rotation);
    p_spring_arm->SetWorldLocation(AGlobal::player_spring_arm_location);
    p_spring_arm->bUsePawnControlRotation = true;
}

void ACustom_player::Init_mesh_settings()
{
    // 메쉬 초기화
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/Characters/UE4_Mannequin/Mesh/SK_Mannequin"));

    if (SK_MANNEQUIN.Succeeded())
        GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);

    GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
}

void ACustom_player::Init_animation_settings()
{
    // 애니메이션 초기화
    static ConstructorHelpers::FClassFinder<UAnimInstance> BP_ANIM(TEXT("/Game/Blueprints/Animations/BP_Player_anim_instance"));

    if (BP_ANIM.Succeeded())
        GetMesh()->SetAnimInstanceClass(BP_ANIM.Class);
}

void ACustom_player::Init_particle_system()
{

}

void ACustom_player::Init_managers()
{
    auto BP_sound_manager = ConstructorHelpers::FClassFinder<AActor>(TEXT("Blueprint'/Game/Blueprints/Managers/BP_Sound_manager.BP_Sound_manager_C'"));

    if (BP_sound_manager.Succeeded())
        mp_sound_manager = Cast<ASound_manager>(BP_sound_manager.Class->GetDefaultObject());
}

void ACustom_player::Check_if_moving()
{
    // ?湲?以?
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
        m_is_sprinting = false;
    }
    else
    {
        // ?숈씤 梨?嫄룰린
        if (current_state == e_player_state::CROUCH ||
            current_state == e_player_state::CROUCH_AIM)
            current_state = e_player_state::CROUCH_WALK;

        // ?롫뱶由?梨?媛湲?
        else if (current_state == e_player_state::PRONING)
            current_state = e_player_state::PRONING_WALK;

        else if (current_state == e_player_state::AIM)
            current_state = e_player_state::AIM_WALK;

        else
        {
            // ?먰봽 以?
            if (GetCharacterMovement()->IsFalling())
            {
                // ?곕뒗 以?
                if (m_sprint_multiplier > 1.f)
                {
                    if (current_oxygen > 0)
                        current_oxygen -= 0.001f;

                    m_is_sprinting = true;
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
                    if (current_oxygen > 0)
                        current_oxygen -= 0.001f;

                    else
                    {
                        current_state = e_player_state::IDLE;
                        m_sprint_multiplier = 1;
                        GetCharacterMovement()->MaxWalkSpeed = 350.f;
                        m_is_sprinting = false;
                        return;
                    }
                    m_is_sprinting = true;

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
            m_collided_vehicle->is_player_near = false;

        is_detected_collision = false;
        return;
    }
    if(hitted_actor->IsA(ACore_vehicle::StaticClass()))
    {
        m_collided_vehicle = Cast<ACore_vehicle>(hitted_actor);
        
        if (m_collided_vehicle)
            m_collided_vehicle->is_player_near = true;

        is_detected_collision = true;
    }
}

void ACustom_player::Check_continously_shooting(float _delta_time)
{
    if (!is_aiming)
        return;

    if (m_is_shooting)
    {
        m_current_shoot_time += _delta_time;

        if (m_current_shoot_time > m_shoot_time)
        {
            Shoot();
            m_current_shoot_time = 0.f;
            m_is_shooting = m_is_changed_shoot_type;
        }
    }
}

void ACustom_player::Custom_jump()
{
    if (current_state == e_player_state::CROUCH ||
        current_state == e_player_state::PRONING)
        return;

    Jump();
}

void ACustom_player::Move_up_down(float _value)
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
    AddControllerPitchInput(_value);
}

void ACustom_player::Turn(float _value)
{
    AddControllerYawInput(_value);
}

void ACustom_player::Custom_crouch()
{
    if (m_is_moving ||
        GetCharacterMovement()->IsFalling())
        return;

    p_spring_arm->SetRelativeLocation(FVector(0.f, 0.f, 80.f));

    // ?숈씤 媛믪쓣 媛?몄????좊떦
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

void ACustom_player::Begin_sprint()
{
    if (!GetVelocity().IsZero())
    {
        current_state = e_player_state::SPRINT;
        Select_weapon(e_equipped_weapon_type::NONE);
    }
}

void ACustom_player::End_sprint()
{
    current_state = e_player_state::IDLE;
    m_sprint_multiplier = 1;
    GetCharacterMovement()->MaxWalkSpeed = 350.f;
}

void ACustom_player::Aim()
{
    bool is_first_equipped = false;
    bool is_second_equipped = false;
    int code = 0;

    Verify_equipped_weapon(is_first_equipped, is_second_equipped);

    // 李⑹슜?섍퀬 ?덈뒗 臾닿린媛 ?놁쓣 ??
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
    Changing_aim_pose(code);
}

void ACustom_player::Shoot()
{
    ACore_weapon* tmp_weapon = nullptr;
    UWorld* p_world = GetWorld();
    bool          is_first_equipped = false;
    bool          is_second_equipped = false;

    if (!is_aiming ||
        m_is_reloading)
        return;

    Verify_equipped_weapon(is_first_equipped, is_second_equipped);

    // 첫번째 무기
    if (is_first_equipped)
        tmp_weapon = m_first_weapon;

    else
        tmp_weapon = m_second_weapon;

    if (tmp_weapon)
    {
        auto weapon_data = tmp_weapon->weapon_data;

        // 총알 부족
        if (weapon_data.current_bullet_count == 0)
        {
            if (weapon_data.current_ammunition_count == 0)
                tmp_weapon->Play_sound(e_weapon_sound_type::EMPTY_AMMO);

            else
                Reload();

            return;
        }
        // 사운드 적용 및 총알 1개 차감
        weapon_data.current_bullet_count--;
        tmp_weapon->Play_sound(e_weapon_sound_type::BULLET_SOUND);

        // 레이캐스트 적용
        APlayerCameraManager* camera_manager = UGameplayStatics::GetPlayerCameraManager(this, 0);
        FVector               begin_pos      = tmp_weapon->skeletal_mesh->GetSocketLocation("Muzzle_socket");
        FVector               forward_vec    = camera_manager->GetActorForwardVector() * 500;
        FHitResult            hit_result;
        FVector               end_pos = begin_pos + forward_vec;
        p_world->LineTraceSingleByObjectType(hit_result, begin_pos, end_pos, FCollisionObjectQueryParams(ECC_Pawn));
        //DrawDebugLine(p_world, begin_pos, end_pos, FColor::Red, true, 5.f, (uint8)0U, 1.f);
        FRotator bullet_rotation = UKismetMathLibrary::FindLookAtRotation(begin_pos, end_pos);
        //DrawDebugLine(p_world, begin_pos, end_pos, FColor::Red, true, 5.f, (uint8)0U, 1.f);
        //GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Cyan, bullet_rotation.ToString());
        AActor* bullet = nullptr;

        if (tmp_weapon->p_bullet)
        {
            // 총알 회전값 수정 요망
            auto test_rotation = FRotator::MakeFromEuler(FVector(end_pos.X, bullet_rotation.Pitch, bullet_rotation.Yaw));
            test_rotation.Yaw += 100.f;
            bullet = p_world->SpawnActor(tmp_weapon->p_bullet->GetClass(), &begin_pos, &bullet_rotation);
            //GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Cyan, test_rotation.ToString());
            /*ACore_bullet* core_bullet = Cast<ACore_bullet>(bullet);

            if (core_bullet)
                core_bullet->mesh->SetRelativeRotation(bullet_rotation);*/
        }
        //if(another_character)

        // 파티클 적용
        tmp_weapon->p_gun_particle->Activate(true);
    }
}

void ACustom_player::Reload()
{
    ACore_weapon* tmp_weapon = nullptr;
    int           result = 0;
    bool          is_first_equipped = false;
    bool          is_second_equipped = false;

    Verify_equipped_weapon(is_first_equipped, is_second_equipped);

    // 첫번째 무기 착용상태
    if (is_first_equipped)
        tmp_weapon = m_first_weapon;

    // 두번째 무기 착용 상태
    if (is_second_equipped)
        tmp_weapon = m_second_weapon;

    if (tmp_weapon)
    {
        auto weapon_data = tmp_weapon->weapon_data;

        // 허용 총알 개수가 똑같을 시
        if (weapon_data.current_bullet_count == weapon_data.max_bullet_count)
            return;

        // 중간에 장전
        else if (weapon_data.current_bullet_count > 0)
            result = weapon_data.max_bullet_count - weapon_data.current_bullet_count;

        // 전체 총알 소모 후 장전
        else
            result = tmp_weapon->weapon_data.max_bullet_count;

        weapon_data.current_ammunition_count -= result;
        weapon_data.current_bullet_count += result;
        tmp_weapon->Play_sound(e_weapon_sound_type::RELOAD_SOUND);
        m_is_reloading = true;
    }
}

void ACustom_player::Check_if_reloading(float _delta_time)
{
    // 재장전 중
    if (m_is_reloading)
    {
        m_current_reload_time += _delta_time;

        if (m_current_reload_time > mk_reload_time)
        {
            m_current_reload_time = 0.f;
            m_is_reloading = false;
        }
    }
}

void ACustom_player::Changing_aim_pose(int _type)
{
    bool is_first_equipped = false;
    bool is_second_equipped = false;

    Verify_equipped_weapon(is_first_equipped, is_second_equipped);

    // ?쒖엳???곹깭?먯꽌 ?먯엫
    if (_type == 1)
    {
        // ?꾩옱 李⑹슜??臾닿린 ?뺤씤
        if (m_first_weapon &&
            is_first_equipped)
            m_first_weapon->skeletal_mesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("hand_weapon_sock"));

        else
        {
            if (m_second_weapon &&
                is_second_equipped)
                m_second_weapon->skeletal_mesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("hand_weapon_sock"));
        }
    }
    // ?먯엫 ?곹깭?먯꽌 ?쒖엳??
    else if (_type == 0)
    {
        // ?꾩옱 李⑹슜??臾닿린 ?뺤씤
        if (m_first_weapon &&
            is_first_equipped)
            Select_weapon(e_equipped_weapon_type::FIRST);

        else
        {
            if (m_second_weapon &&
                is_second_equipped)
                Select_weapon(e_equipped_weapon_type::SECOND);
        }
    }
}

void ACustom_player::Proning()
{
    switch (current_state)
    {
    case e_player_state::IDLE:
        current_state = e_player_state::PRONING;
        p_spring_arm->SetRelativeLocation(FVector(0.f, 0.f, -50.f));
        break;

    case e_player_state::CROUCH:
    case e_player_state::CROUCH_WALK:
        current_state = e_player_state::PRONING;
        p_spring_arm->SetRelativeLocation(FVector(0.f, 0.f, -50.f));
        UnCrouch();
        break;

    case e_player_state::AIM:
        current_state = e_player_state::PRONING_AIM;
        break;

    case e_player_state::PRONING:
    case e_player_state::PRONING_WALK:
        current_state = e_player_state::IDLE;
        p_spring_arm->SetRelativeLocation(FVector(0.f, 0.f, 80.f));
        break;
    }
}

void ACustom_player::Try_to_get_collided_component()
{
    if (m_is_interacting)
    {
        // 臾닿린硫?
        if (m_collided_weapon)
        {
            // ?꾩옱 ?μ갑?섎뒗 臾닿린媛 ?놁쑝硫?
            if (!m_first_weapon)
            {
                m_first_weapon = m_collided_weapon;
                Select_weapon(e_equipped_weapon_type::FIRST);
                Change_shoot_mode();
            }
            // 理쒖냼 臾닿린 1媛??μ갑 以?
            else
            {
                // ?먮쾲吏?臾닿린媛 ?놁쓣 ??
                if (!m_second_weapon)
                {
                    // 泥ル쾲吏?臾닿린
                    m_first_weapon->skeletal_mesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("first_back_weapon_sock"));
                    m_second_weapon = m_collided_weapon;

                    // ?먮쾲吏?臾닿린
                    Select_weapon(e_equipped_weapon_type::SECOND);
                }
                else // 援먯껜
                    Select_weapon(e_equipped_weapon_type::SWAP);
            }
        }
        // 
        else if (m_collided_vehicle)
        {
            //  차량 탑승 상태
            if (m_collided_vehicle->Check_available_seat(this))
            {
                m_collided_vehicle->is_player_near = false;
                is_in_vehicle                      = true;
            }
            m_collided_vehicle = nullptr;
        }
    }
}

void ACustom_player::Update_weapon_pos()
{
    switch (current_state)
    {
    case e_player_state::WALK:
    case e_player_state::SPRINT:
    case e_player_state::JUMP:
    case e_player_state::PRONING:

        // ?먯씠 ?꾨땺 ???ㅼ젙
        if (m_first_weapon)
            m_first_weapon->skeletal_mesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("first_back_weapon_sock"));

        if (m_second_weapon)
            m_second_weapon->skeletal_mesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("second_back_weapon_sock"));

        break;
    }
    // ?꾩옱 臾닿린媛 泥ル쾲吏?臾닿린媛 ?꾨땶 寃쎌슦
    if (Is_first_weapon_equipped())
        m_first_weapon->skeletal_mesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("first_back_weapon_sock"));

    // ?꾩옱 臾닿린媛 ?먮쾲吏?臾닿린媛 ?꾨땶 寃쎌슦
    if (Is_second_weapon_equipped())
        m_second_weapon->skeletal_mesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("second_back_weapon_sock"));
}

void ACustom_player::Swap_weapon()
{
    if (Is_first_weapon_equipped())
        Select_weapon(e_equipped_weapon_type::FIRST);
    
    // 두번쨰 무기가 있을 시 교체
    if (Is_second_weapon_equipped())
        Select_weapon(e_equipped_weapon_type::SECOND);
}

void ACustom_player::Change_shoot_mode()
{
}

void ACustom_player::Verify_equipped_weapon(bool& _first_weapon, bool& _second_weapon)
{
    if (m_first_weapon)
        _first_weapon = m_first_weapon->is_equipped;

    if (m_second_weapon)
        _second_weapon = m_second_weapon->is_equipped;
}

void ACustom_player::Select_weapon(e_equipped_weapon_type _type)
{
    ACore_weapon* tmp_weapon = nullptr;

    switch (_type)
    {
    case e_equipped_weapon_type::FIRST: // 첫번째 무기 착용

        if (!m_first_weapon)
            return;

        if (m_second_weapon)
            m_second_weapon->is_equipped = false;

        tmp_weapon = m_first_weapon;
        m_first_weapon->is_equipped = true;
        break;

    case e_equipped_weapon_type::SECOND: // 두번째 무기 착용

        if (!m_second_weapon)
            return;

        tmp_weapon = m_second_weapon;

        m_first_weapon->is_equipped = false;
        m_second_weapon->is_equipped = true;
        break;

    case e_equipped_weapon_type::SWAP: // 교체

        if (!m_second_weapon)
            return;

        tmp_weapon = m_collided_weapon;

        // 맵에 배치된 무기랑 교체
        m_second_weapon->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
        m_second_weapon->SetActorTransform(m_second_weapon->GetTransform());
        m_second_weapon->p_widget_component->SetVisibility(true);
        m_second_weapon = nullptr;
        break;

    case e_equipped_weapon_type::NONE: break;
    }
    // 장착
    if (tmp_weapon)
    {
        tmp_weapon->skeletal_mesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("front_weapon_sock"));
        tmp_weapon->p_widget_component->SetVisibility(false);
    }
    //Play_swap_sound();
    m_collided_weapon = nullptr;
}

void ACustom_player::Equip_first_weapon()
{
    if (m_first_weapon &&
        !m_first_weapon->is_equipped)
        Select_weapon(e_equipped_weapon_type::FIRST);
}

void ACustom_player::Equip_second_weapon()
{
    if (m_second_weapon &&
        !m_second_weapon->is_equipped)
        Select_weapon(e_equipped_weapon_type::SECOND);
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
    p_spring_arm->SetRelativeLocation(AGlobal::player_spring_arm_location);
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void ACustom_player::Open_inventory()
{
    bool is_opened_inventory = mp_UI_manager->p_inventory_manager->is_opened;
    mp_UI_manager->p_inventory_manager->is_opened = (is_opened_inventory == true) ? false : true;
}

bool ACustom_player::Is_first_weapon_equipped()
{
    return (m_first_weapon && !m_first_weapon->is_equipped); 
}

bool ACustom_player::Is_second_weapon_equipped()
{
    return (m_second_weapon && !m_second_weapon->is_equipped);
}