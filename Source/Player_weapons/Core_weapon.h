/**
 * \file Core_weapon.h
 *
 * \brief 모든 총기 무기들은 이로부터 상속받음
 *
 * \ingroup Player_weapons
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "PUBG_UE4/Base_interaction.h"
#include "Weapon_enum.h"
#include "PUBG_UE4/Data_table_manager.h"
#include "Core_weapon.generated.h"

class UAudioComponent;
class USoundBase;
class ACore_bullet;
class UParticleSystemComponent;

UCLASS()
class PLAYER_WEAPONS_API ACore_weapon : public ABase_interaction
{
	GENERATED_BODY()
	
public:
    // 총알 관련
    UPROPERTY(VisibleAnywhere, Category = Bullet) ACore_bullet* p_bullet;

    // class ACore_gun_mag* p_gun_mag = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Particle) UParticleSystemComponent* p_gun_particle = nullptr;

    Fs_weapon_data   weapon_data;
    e_weapon_type    weapon_type    = e_weapon_type::MAX;
    e_gun_shoot_type gun_shoot_type = e_gun_shoot_type::SINGLE;

public:
    ACore_weapon();

protected:
    virtual void BeginPlay() override;

    virtual void NotifyActorBeginOverlap(AActor*) override;

    virtual void NotifyActorEndOverlap(AActor*) override;

    virtual void Tick(float DeltaTime) override;


protected:
    /**
      * \brief 무기 초기화
      * \param 무기 종류
     */
    void Init(e_weapon_type _weapon_type);

    /**
      * \brief 메쉬 초기화
     */
    void Init_mesh();

    /**
      * \brief 콜라이더 정보 갱신
     */
    void Update_collider();

    /**
      * \brief 탄알 초기화
     */
    void Init_bullet();

    /**
      * \brief 파티클 시스템 초기화
     */
    void Init_particle_system();

    /**
      * \brief 파티클 시스템 갱신
     */
    void Update_particle_system();
};