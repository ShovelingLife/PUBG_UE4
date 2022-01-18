#pragma once

#include "CoreMinimal.h"
#include "PUBG_UE4/Base_interaction.h"
#include "Weapon_enum.h"
#include "PUBG_UE4/Data_table_manager.h"
#include "Core_weapon.generated.h"

class UAudioComponent;
class USoundBase;
class ACore_bullet;

UCLASS()
class PLAYER_WEAPONS_API ACore_weapon : public ABase_interaction
{
	GENERATED_BODY()
	
public:
    UPROPERTY(VisibleAnywhere, Category = Sound)
        UAudioComponent* p_audio_comp = nullptr;

    // 총알 관련
    UPROPERTY(VisibleAnywhere, Category = Bullet)
        ACore_bullet* p_bullet;

    class ACore_gun_mag* p_gun_mag = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Particle)
        class UParticleSystemComponent* p_gun_particle = nullptr;

    e_weapon_type  weapon_type;
    Fs_weapon_data weapon_data;
    bool		   is_equipped = false;


protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void NotifyActorBeginOverlap(AActor*) override;

    virtual void NotifyActorEndOverlap(AActor*) override;

    // Called every frame
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

    // 사운드 초기화
    void Init_audio();

    void Init_particle_system();

    void Update_particle_system();
};