#pragma once

#include "CoreMinimal.h"
#include "PUBG_UE4/My_enum.h"
#include "GameFramework/Character.h"
#include "Custom_player.generated.h"

class UMy_anim_instance;
class ACore_weapon;
class ACore_vehicle;

enum class e_equipped_weapon_type
{
    FIRST,
    SECOND,
    SWAP,
    NONE
};

UCLASS()
class CHARACTERS_API ACustom_player : public ACharacter
{
    GENERATED_BODY()

private:
    // 현재 쓰여지는 오브젝트들
    ACore_weapon*  m_collided_weapon  = nullptr;
    ACore_weapon*  m_first_weapon     = nullptr;
    ACore_weapon*  m_second_weapon    = nullptr;
    ACore_vehicle* m_collided_vehicle = nullptr;

    // 오디오 관련
    UPROPERTY(VisibleAnywhere, Category = Audio)
        class UAudioComponent* mp_audio;

    // 파티클 관련
    UPROPERTY(VisibleAnywhere, Category = Particle)
        class UParticleSystemComponent* mp_particle;

    // 이동 관련
    FVector     m_direction_up_down     = FVector::ZeroVector;
    FVector     m_direction_left_right  = FVector::ZeroVector;
    float       m_sprint_time           = 0.f;
    float       m_max_sprint_time       = 0.5f;
    float       m_sprint_multiplier     = 1.f;
    bool        m_is_moving             = false;

    // 무기 관련
    const float mk_reload_time          = 2.f;
    FString     m_gun_type_str          = "";
    float       m_current_reload_time   = 0.f;
    float       m_shoot_time            = 0.25f;
    float       m_current_shoot_time    = 0.f;
    bool        m_is_reloading          = false;
    bool        m_is_interacting        = false;
    bool        m_is_changed_shoot_type = true;
    bool        m_is_shooting           = false;
    bool        m_is_inventory_opened   = false;

 // 플레이어 컴포넌트 및 상태 변수
public:
    UPROPERTY(VisibleAnywhere, Category = Camera)
        class USpringArmComponent* p_spring_arm = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Camera)
        class UCameraComponent* p_camera = nullptr;

    // Current HP
    const float k_max_health = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
        float current_health = 100;

    e_player_state current_state;
    bool           is_detected_collision = false;
    bool           is_animation_playing  = false;
    bool           is_aiming             = false;
    bool           is_weapon_equipped    = false;
    
    // 차량 관련 변수
    e_seat_type current_seat_type = e_seat_type::NONE;
    bool        is_in_vehicle     = false;

public:
    ACustom_player();

private:    
    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    //UFUNCTION()
    //void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// 초기화 함수들
private:
    void Init_collider_settings();

    void Init_player_settings();

    void Init_camera_settings();

    void Init_mesh_settings();

    void Init_animation_settings();

    void Init_particle_system();

private:

    void Try_to_get_collided_component();

    void Begin_interact() { m_is_interacting = true; }

    void End_interact()   { m_is_interacting = false; }

    void Begin_shooting() { m_is_shooting = true; }

    void End_shooting()   { if (m_is_changed_shoot_type) m_is_shooting = false; }

    // 
    void Move_up_down(float);

    // 
    void Move_left_right(float);

    // 
    void Check_if_moving();

    // 차량 감지하는 함수
    void Check_if_is_vehicle_near();
   
    void Check_continously_shooting(float);

    // 위 아래 카메라 전환
    void Look_up(float);

    // 양옆 카메라 전환
    void Turn(float);

    // 점프중
    void Custom_jump();

    // 숙이고 있음
    void Custom_crouch();

    // 엎드리고 있음
    void Proning();

    // 뛰고 있음
    void Begin_sprint();

    // 뛰다가 멈춤
    void End_sprint();


    // ------- UI 관련 -------

    void Open_inventory();


    // ------- 무기 관련 -------

    void Aim();

    void Shoot();

    void Reload();

    void Check_if_reloading(float);

    void Changing_aim_pose(int);

    void Update_weapon_pos();

    void Swap_weapon();

    void Change_shoot_mode();

    void Verify_equipped_weapon(bool&, bool&);

    void Select_weapon(e_equipped_weapon_type);

    void Equip_first_weapon();

    void Equip_second_weapon();

    // 첫번째 무기가 장착 되어있음
    bool Is_first_weapon_equipped();

    // 두번째 무기가 장착 되어있음
    bool Is_second_weapon_equipped();

    // 첫번째 무기를 부착
    void Attach_first_weapon(FString);

    // 두번째 무기를 부착
    void Attach_second_weapon(FString);

public:
    void Exit_from_vehicle(FVector);
};