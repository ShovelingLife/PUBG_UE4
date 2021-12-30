#pragma once

#include "CoreMinimal.h"
#include "PUBG_UE4/My_enum.h"
#include "GameFramework/Character.h"
#include "Custom_player.generated.h"
class UMy_anim_instance;
class ACore_weapon;
class ACore_vehicle;
class UPlayer_UI;
class AInventory_manager;

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
    // ���� �������� ������Ʈ��
    ACore_weapon*  m_collided_weapon = nullptr;
    ACore_weapon*  m_first_weapon    = nullptr;
    ACore_weapon*  m_second_weapon   = nullptr;
    ACore_vehicle* m_collided_vehicle = nullptr;

    // ����� ����
    UPROPERTY(VisibleAnywhere, Category = Audio)
        class UAudioComponent* mp_audio;

    USoundBase* mp_walk_sound;
    USoundBase* mp_gun_farm_sound;
    USoundBase* mp_gun_swap_sound;

    // ��ƼŬ ����
    UPROPERTY(VisibleAnywhere, Category = Particle)
        class UParticleSystemComponent* mp_particle;

    // UI ����
    TSubclassOf<UUserWidget>  m_user_widget;
    UPlayer_UI* mp_user_ui;

    // �κ��丮 ����
    TSubclassOf<AInventory_manager>  m_inventory_manager_subclass;
    AInventory_manager* mp_inventory_manager;

    // �̵� ����
    FVector     m_direction_up_down{ 0.f };
    FVector     m_direction_left_right{ 0.f };
    float       m_sprint_transition_time = 0.f;
    float       m_max_sprint_transition_time = 0.5f;
    float       m_sprint_multiplier = 1.f;
    bool        m_is_moving = false;

    // ���� ����
    const float mk_reload_time          = 2.f;
    FString     m_gun_type_str          = "";
    float       m_current_reload_time   = 0.f;
    float       m_shoot_time            = 0.25f;
    float       m_current_shoot_time    = 0.f;
    bool        m_is_reloading          = false;
    bool        m_is_sprinting          = false;
    bool        m_is_interacting        = false;
    bool        m_is_changed_shoot_type = true;
    bool        m_is_shooting           = false;

    // 기�? 변??
public:
    UPROPERTY(VisibleAnywhere, Category = Camera)
        class USpringArmComponent* p_spring_arm = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Camera)
        class UCameraComponent* p_camera = nullptr;

    // Current HP
    const float k_max_health = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
        float current_health = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Oxygen)
        float current_oxygen = 1.f;

    e_player_state current_state;
    bool           is_detected_collision = false;
    bool           is_animation_playing  = false;
    bool           is_aiming             = false;
    bool           is_weapon_equipped    = false;

    e_seat_type current_seat_pos = e_seat_type::NONE;

public:
    // Sets default values for this character's properties
    ACustom_player();

    // 기본 ?�폴???�수
private:
    // 충돌 ?�작
    UFUNCTION()
        void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // 충돌 ??
    UFUNCTION()
        void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // 초기???�수
private:
    void Init_player_settings();

    void Init_audio();

    void Init_camera_settings();

    void Init_mesh_settings();

    void Init_animation_settings();

    void Init_UI();

    void Init_particle_system();

private:

    void Try_to_get_collided_component();

    void Play_swap_sound();

    // ?�동 관???�수
private:
    void Begin_interact() { m_is_interacting = true; }

    void End_interact() { m_is_interacting = false; }

    void Begin_shooting() { m_is_shooting = true; }

    void End_shooting() { if (m_is_changed_shoot_type) m_is_shooting = false; }

    // 
    void Move_up_down(float);

    // 
    void Move_left_right(float);

    // 
    void Check_if_moving();

    void Play_walk_sound();

    void Check_continously_shooting(float);

    // 카메?��? ???�래�??�환
    void Look_up(float);

    // 카메?��? ?�쪽 ?�른쪽으�??�환
    void Turn(float);

    // ?�프
    void Custom_jump();

    // ?�임
    void Custom_crouch();

    // ?�드�?
    void Proning();

    // ?�리�??�작??
    void Begin_sprint();

    //?�리�?멈춤
    void End_sprint();

    // ?�임 맞추�?

    // ------- UI ���� -------

    void Update_UI(float);

    void Update_oxygen_bar_UI(float);

    void Update_weapon_slot_UI();

    void Update_aim_UI();

    void Update_bullet_count_UI();

    void Change_shoot_mode();

    void Open_inventory();

    // ------- ���� ���� -------

    void Aim();

    void Shoot();

    void Reload();

    void Check_if_reloading(float);

    void Changing_aim_pose(int);

    void Update_weapon_pos();

    void Swap_weapon();

    void Verify_equipped_weapon(bool&, bool&);

    void Select_weapon(e_equipped_weapon_type);

    void Equip_first_weapon();

    void Equip_second_weapon();
};