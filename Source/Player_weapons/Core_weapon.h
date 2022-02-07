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

    class ACore_gun_mag* p_gun_mag = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Particle) UParticleSystemComponent* p_gun_particle = nullptr;

    Fs_weapon_data   weapon_data;
    e_weapon_type    weapon_type    = e_weapon_type::MAX;
    e_gun_shoot_type gun_shoot_type = e_gun_shoot_type::SINGLE;

protected:
    virtual void BeginPlay() override;

    virtual void NotifyActorBeginOverlap(AActor*) override;

    virtual void NotifyActorEndOverlap(AActor*) override;

    virtual void Tick(float DeltaTime) override;

public:
    ACore_weapon();

protected:
    void Init(e_weapon_type);

    // 메쉬 초기화
    void Init_mesh();

    // 콜라이더 정보 갱신
    void Update_collider();

    // 탄알 초기화
    void Init_bullet();

    void Init_particle_system();

    void Update_particle_system();
};