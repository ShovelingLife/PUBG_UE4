#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PUBG_UE4/My_enum.h"
#include "Weapon_enum.h"
#include "Weapon_manager.generated.h"

class ABase_interaction;
class ACore_weapon;
class ACore_melee_weapon;
class ACore_throwable_weapon;

UCLASS()
class PLAYER_WEAPONS_API AWeapon_manager : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = Scene_comp)
	class USceneComponent* mp_scene_comp = nullptr;

    const float	mk_reload_time			= 2.f;
    float		m_current_reload_time	= 0.f;
    float		m_shoot_time			= 0.25f;
    float		m_current_shoot_time	= 0.f;
    bool		m_is_reloading		    = false;
    bool		m_is_interacting	    = false;
    bool		m_is_changed_shoot_type = true;
    bool		m_is_shooting			= false;

public:
	ACore_weapon*			p_first_gun			= nullptr;
    ACore_weapon*			p_second_gun		= nullptr;
    ACore_weapon*			p_pistol			= nullptr;
    ACore_melee_weapon*		p_melee				= nullptr;
    ACore_throwable_weapon* p_throwable			= nullptr;
    e_current_weapon_type	current_weapon_type = e_current_weapon_type::NONE;
	bool					arr_is_weapon_equipped[5]{ false };
	bool					is_shooting			= false;

public:	
	// Sets default values for this actor's properties
	AWeapon_manager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// 착용 중인 무기 체크
    void Check_for_equipped_weapon();

	// 현재 장착 중인 무기
	void Update_current_equipped_weapon();

	// 사운드 재생
    void Play_sound(e_weapon_sound_type);

	// 배열 내에 원소 찾음 / 방향 / 시작 위치
    e_current_weapon_type Find_weapon_index(FString, int);

	// 부착
    void Attach_weapon(ABase_interaction*, FString);

	// 탈착 후 위치 초기화
    void Reset_weapon_after_detaching(ABase_interaction*, FTransform);

public:
	// 장착
	void Equip(AActor*);

	// 발사
	void Shoot();

	// 장전
	void Reload();

	// 마우스 휠
	bool Scroll_select(int);

	// 교체 현재 무기 / 맵 무기 / 소켓 이름
	void Swap(ABase_interaction*, AActor*, FString);

	void Change_shoot_mode();

	void Change_aim_pose(int);

	// 장전 중인지 확인
	void Check_if_reloading(float);

	// 연사 / 연발 확인
	void Check_continously_shooting(float);

	// 무기 교체가 가능한지 확인
	bool Check_if_weapon_available(e_current_weapon_type);

	// ------- 부착 관련 함수 -------

    int Get_weapon_max_bullet_count(e_current_weapon_type);

    // 무기 종류를 가져옴
    ACore_weapon* Get_weapon_type(e_current_weapon_type);

	// ------- UI 관련 함수 -------

	// 무기를 맵에다가 버림
	void Drop(ABase_interaction*);
};