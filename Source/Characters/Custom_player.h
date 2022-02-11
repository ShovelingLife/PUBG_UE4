/**
 * \file Custom_player.h
 *
 * \brief 캐릭터 클래스 관련
 *
 * \ingroup Characters
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "PUBG_UE4/My_enum.h"
#include "GameFramework/Character.h"
#include "Custom_player.generated.h"

/**
 * \brief 델리게이트 모음 (인벤토리 열기/닫기)
 */
DECLARE_DELEGATE(FDele_open_inventory)
DECLARE_DELEGATE(FDele_close_inventory)

class ASound_manager;
class AWeapon_manager;
class UMy_anim_instance;
class ACore_weapon;
class ACore_vehicle;
class UAudioComponent;
class UParticleSystemComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class CHARACTERS_API ACustom_player : public ACharacter
{
    GENERATED_BODY()

private:
    /**
     * \brief 플레이어가 사용하는 변수
     */
    UPROPERTY() ASound_manager*  mp_sound_manager;
    UPROPERTY() AWeapon_manager* mp_weapon_manager;
    UPROPERTY() AActor*          mp_collided_weapon  = nullptr;
    UPROPERTY() ACore_vehicle*   mp_collided_vehicle = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Audio)    UAudioComponent*          mp_audio_comp;
    UPROPERTY(VisibleAnywhere, Category = Particle) UParticleSystemComponent* mp_particle;

    /**
     * \brief 이동 관련 변수
     */
    FVector m_direction_up_down    = FVector::ZeroVector;
    FVector m_direction_left_right = FVector::ZeroVector;
    float   m_sprint_time          = 0.f;
    float   m_max_sprint_time      = 0.5f;
    float   m_sprint_multiplier    = 1.f;
    bool    m_is_moving            = false;
    bool    m_is_interacting       = false;
    bool    m_is_inventory_opened  = false;

 // 플레이어 컴포넌트 및 상태 변수
public:
    FDele_open_inventory  dele_open_inventory;
    FDele_close_inventory dele_close_inventory;

    UPROPERTY(VisibleAnywhere, Category = Camera) USpringArmComponent* p_spring_arm_comp = nullptr;
    UPROPERTY(VisibleAnywhere, Category = Camera) UCameraComponent* p_camera_comp = nullptr;

    /**
     * \brief 플레이어 상태 관련 변수
     */
    e_player_state current_state;
    const float    k_max_health          = 100.f;
    float          current_health        = 100;
    float          current_oxygen        = 1.f;
    bool           is_sprinting          = false;
    bool           is_animation_playing  = false;
    bool           is_aiming             = false;
    bool           is_weapon_equipped    = false;
    
    /**
     * \brief 차량 관련 변수
     */
    e_seat_type    current_seat_type = e_seat_type::NONE;
    bool           is_in_vehicle     = false;

public:
    ACustom_player();

private: 
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// 초기화 함수들
private:
    /**
     * \brief 플레이어에 대한 정보 갱신
     */
    void Update_player_settings();

    /**
     * \brief 카메라 컴포넌트 초기화
     */
    void Init_camera_comp();

    /**
     * \brief 메시 컴포넌트 초기화
     */
    void Init_mesh_comp();

    /**
     * \brief 오디오 컴포넌트 초기화
     */
    void Init_audio_comp();

    /**
     * \brief 애님 인스턴스 초기화
     */
    void Init_anim_instance();

    /**
     * \brief 파티클 시스템 초기화
     */
    void Init_particle_system();

private:
    /**
     * \brief 플레이어 움직임 감지
     */
    void Check_if_moving();

    /**
     * \brief 충돌한 오브젝트 확인 (아이템) (레이캐스트 : 플레이어 발 밑)
     */
    void Check_for_object();

    /**
     * \brief 근처에 차량이 있는지 감지 (레이캐스트 : 플레이어 상체)
     */
    void Check_if_is_vehicle_near();

    /**
     * \brief 상호 작용 중일 시 (무기 줍기/차량 탑승)
     */
    void Try_to_interact();

    /**
     * \brief F키 눌렀을 시 상호 작용
     */
    void Begin_interact() { m_is_interacting = true; }

    /**
     * \brief F키를 뗐을 시 
     */
    void End_interact()   { m_is_interacting = false; }
  
    /**
     * \brief 앞 뒤로 움직임
     */
    void Move_forward_back(float);

    /**
     * \brief 좌 우로 움직임
     */
    void Move_left_right(float);

    /**
     * \brief 위 아래 카메라 전환
     * \param _value 카메라 회전 값
     */
    void Look_up(float _value);

    /**
     * \brief 좌우 카메라 전환
     * \param _value 카메라 회전 값
     */
    void Turn(float _value);

    /**
     * \brief 점프
     */
    void Custom_jump();

    /**
     * \brief 숙임
     */
    void Custom_crouch();

    /**
     * \brief 엎드림
     */
    void Proning();

    /**
     * \brief 뛰기 시작함
     */
    void Begin_sprint();

    /**
     * \brief 뛰다 멈춤
     */
    void End_sprint();


    // ------- UI 관련 -------

    /**
     * \brief 인벤토리 열음/닫음
     */
    void Open_inventory();


    // ------- 무기 관련 -------

    /**
     * \brief 사격
     */
    void Begin_shooting();

    /**
     * \brief 사격 중지
     */
    void End_shooting();

    /**
     * \brief 재장전
     */
    void Reload();

    /**
     * \brief 조준
     */
    void Aim();

    /**
     * \brief 격발 모드 변경
     */
    void Change_shoot_mode();

    /**
     * \brief 마우스 위로 스크롤
     */
    void Swap_scrolling_up();

    /**
     * \brief 마우스 아래로 스크롤
     */
    void Swap_scrolling_down();

    /**
     * \brief 첫번째 무기 장착
     */
    void Equip_first_weapon();

    /**
     * \brief 두번째 무기 장착
     */
    void Equip_second_weapon();

    /**
     * \brief 세번째 무기 장착
     */
    void Equip_third_weapon();

    /**
     * \brief 네번째 무기 장착
     */
    void Equip_fourth_weapon();

    /**
     * \brief 다섯번째 무기 장착
     */
    void Equip_fifth_weapon();

public:
    /**
     * \brief 플레이어가 차량에서 나감
     * \param _exit_location 나가는 위치
     */
    void Exit_from_vehicle(FVector _exit_location);

    /**
     * \brief 무기 매니저를 갖고옴
     * \return 무기 매니저
     */
    AWeapon_manager* Get_weapon_manager() { return mp_weapon_manager; }
};