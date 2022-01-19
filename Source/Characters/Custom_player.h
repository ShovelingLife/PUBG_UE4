#pragma once

#include "CoreMinimal.h"
#include "PUBG_UE4/My_enum.h"
#include "GameFramework/Character.h"
#include "Custom_player.generated.h"

class ASound_manager;
class AWeapon_manager;
class UMy_anim_instance;
class ACore_weapon;
class ACore_vehicle;

UCLASS()
class CHARACTERS_API ACustom_player : public ACharacter
{
    GENERATED_BODY()

private:
    // ���� �������� ������Ʈ��
    ASound_manager*  mp_sound_manager;
    AWeapon_manager* mp_weapon_manager;
    AActor*          m_collided_weapon  = nullptr;
    ACore_vehicle*   m_collided_vehicle = nullptr;

    // ����� ����
    UPROPERTY(VisibleAnywhere, Category = Audio)
        class UAudioComponent* mp_audio_comp;

    // ��ƼŬ ����
    UPROPERTY(VisibleAnywhere, Category = Particle)
        class UParticleSystemComponent* mp_particle;

    // �̵� ����
    FVector     m_direction_up_down    = FVector::ZeroVector;
    FVector     m_direction_left_right = FVector::ZeroVector;
    float       m_sprint_time          = 0.f;
    float       m_max_sprint_time      = 0.5f;
    float       m_sprint_multiplier    = 1.f;
    bool        m_is_moving            = false;
    bool        m_is_interacting       = false;

 // �÷��̾� ������Ʈ �� ���� ����
public:
    UPROPERTY(VisibleAnywhere, Category = Camera)
        class USpringArmComponent* p_spring_arm_comp = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Camera)
        class UCameraComponent* p_camera_comp = nullptr;

    const float k_max_health = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
        float current_health = 100;

    UPROPERTY(VisibleAnywhere, Category = Oxygen)
        float current_oxygen = 1.f;

    UPROPERTY(VisibleAnywhere, Category = State)
        bool  is_sprinting = false;

    e_player_state current_state;
    bool           is_animation_playing  = false;
    bool           is_aiming             = false;
    bool           is_weapon_equipped    = false;
    bool           is_inventory_opened   = false;
    
    // ���� ���� ����
    e_seat_type    current_seat_type = e_seat_type::NONE;
    bool           is_in_vehicle     = false;

public:
    ACustom_player();

private: 
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// �ʱ�ȭ �Լ���
private:
    void Update_player_settings();

    void Init_camera_comp();

    void Init_mesh_comp();

    void Init_audio_comp();

    void Init_anim_instance();

    void Init_particle_system();

private:
    void Check_for_object();

    void Try_to_get_collided_component();

    void Begin_interact() { m_is_interacting = true; }

    void End_interact()   { m_is_interacting = false; }

    void Begin_shooting();

    void End_shooting();

    // 
    void Move_up_down(float);

    // 
    void Move_left_right(float);

    // 
    void Check_if_moving();

    // ���� �����ϴ� �Լ�
    void Check_if_is_vehicle_near();
    
    // �� �Ʒ� ī�޶� ��ȯ
    void Look_up(float);

    // �翷 ī�޶� ��ȯ
    void Turn(float);

    // ������
    void Custom_jump();

    // ���̰� ����
    void Custom_crouch();

    // ���帮�� ����
    void Proning();

    // �ٰ� ����
    void Begin_sprint();

    // �ٴٰ� ����
    void End_sprint();


    // ------- UI ���� -------

    void Open_inventory();


    // ------- ���� ���� -------
    
    void Reload();

    void Aim();

    void Change_shoot_mode();

    void Swap_scrolling_up();

    void Swap_scrolling_down();

    void Update_weapon_pos();

    void Equip_first_weapon();

    void Equip_second_weapon();

    void Equip_third_weapon();

    void Equip_fourth_weapon();

    void Equip_fifth_weapon();

    void Set_item_UI(bool);

public:
    void Exit_from_vehicle(FVector);

    AWeapon_manager* Get_weapon_manager() { return mp_weapon_manager; }
};