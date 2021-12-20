#pragma once

#include "CoreMinimal.h"
#include "PUBG_UE4/Base_interaction.h"
#include "PUBG_UE4/Custom_enum.h"
#include "PUBG_UE4/Data_table_manager.h"
#include "Core_weapon.generated.h"

class UAudioComponent;
class USoundBase;
class ACore_bullet;
class UMaterial;

UCLASS()
class PLAYER_WEAPONS_API ACore_weapon : public ABase_interaction
{
	GENERATED_BODY()
	
protected:
    // ------- ���� ���� -------
    UPROPERTY(VisibleAnywhere, Category = Sound)
    UAudioComponent* mp_audio = nullptr;

    e_weapon_type m_weapon_type;
    bool		  m_is_gun = true;

public:
    // �Ѿ� ����
    UPROPERTY(VisibleAnywhere, Category = Bullet)
        ACore_bullet* p_bullet;

    class ACore_gun_mag* p_gun_mag = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Particle)
        class UParticleSystemComponent* p_gun_particle = nullptr;

    UPROPERTY(VisibleAnywhere, Category = UI)
        UMaterial* p_render_target_ui_mesh = nullptr;

    Fs_weapon_data weapon_data;
    bool		  is_equipped = false;


protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void NotifyActorBeginOverlap(AActor*) override;

    virtual void NotifyActorEndOverlap(AActor*) override;

public:
    // Sets default values for this actor's properties
    ACore_weapon();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

protected:
    void Init(e_weapon_type);

    // 메시 ?�정
    void Init_mesh();

    // 충돌박스 ?�정
    void Update_collider();

    // 총알 ?�정
    void Init_bullet();

    // ?�운???�정
    void Init_audio();

    // ?�더 ?��?UI ?�정
    void Init_UI_material();

    void Init_particle_system();

    void Update_particle_system();

public:
    // ��ȣ �ۿ�
    virtual void Interact() override;

    void Play_sound(e_weapon_sound_type);
};