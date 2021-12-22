#pragma once

#include "CoreMinimal.h"
#include "PUBG_UE4/Custom_enum.h"
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
    // 현재 쓰여지는 오브젝트들
    ACore_weapon*  m_collided_weapon = nullptr;
    ACore_weapon*  m_first_weapon    = nullptr;
    ACore_weapon*  m_second_weapon   = nullptr;
    ACore_vehicle* m_current_vehicle = nullptr;

    // 오디오 관련
    UPROPERTY(VisibleAnywhere, Category = Audio)
        class UAudioComponent* mp_audio;

    USoundBase* mp_walk_sound;
    USoundBase* mp_gun_farm_sound;
    USoundBase* mp_gun_swap_sound;

    // 파티클 관련
    UPROPERTY(VisibleAnywhere, Category = Particle)
        class UParticleSystemComponent* mp_particle;

    // UI 관련
    TSubclassOf<UUserWidget>  m_user_widget;
    UPlayer_UI* mp_user_ui;

    // 인벤토리 관련
    TSubclassOf<AInventory_manager>  m_inventory_manager_subclass;
    AInventory_manager* mp_inventory_manager;

    // 이동 관련
    FVector     m_direction_up_down{ 0.f };
    FVector     m_direction_left_right{ 0.f };
    float       m_sprint_transition_time = 0.f;
    float       m_max_sprint_transition_time = 0.5f;
    float       m_sprint_multiplier = 1.f;
    bool        m_is_moving = false;

    // ?곹샇?묒슜 愿�??
    const float mk_reload_time = 2.f;
    FString     m_gun_type_str = "";
    float       m_current_reload_time = 0.f;
    float       m_shoot_time = 0.25f;
    float       m_current_shoot_time = 0.f;
    bool        m_is_reloading = false;
    bool        m_is_sprinting = false;
    bool        m_is_interacting = false;
    bool        m_is_changed_shoot_type = true;
    bool        m_is_shooting = false;

    // 湲고? 蹂�??
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

public:
    // Sets default values for this character's properties
    ACustom_player();

    // 湲곕낯 ?뷀뤃???⑥닔
private:
    // 異⑸룎 ?쒖옉
    UFUNCTION()
        void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // 異⑸룎 ??
    UFUNCTION()
        void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // 珥덇린???⑥닔
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

    // ?대룞 愿�???⑥닔
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

    // 移대찓?쇰? ???꾨옒濡??꾪솚
    void Look_up(float);

    // 移대찓?쇰? ?쇱そ ?ㅻⅨ履쎌쑝濡??꾪솚
    void Turn(float);

    // ?먰봽
    void Custom_jump();

    // ?숈엫
    void Custom_crouch();

    // ?롫뱶由?
    void Proning();

    // ?щ━湲??쒖옉??
    void Begin_sprint();

    //?щ━湲?硫덉땄
    void End_sprint();

    // ?먯엫 留욎텛湲?

    // ------- UI 관련 -------

    void Update_UI(float);

    void Update_oxygen_bar_UI(float);

    void Update_weapon_slot_UI();

    void Update_aim_UI();

    void Update_bullet_count_UI();

    void Change_shoot_mode();

    void Open_inventory();

    // ------- 무기 관련 -------

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