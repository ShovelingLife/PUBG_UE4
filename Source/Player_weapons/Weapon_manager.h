/**
 * \file Weapon_manager.h
 *
 * \brief 플레이어 무기 매니저 관련
 *
 * \ingroup Player_weapons
 *
 * \author ShovelingLife
 */
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
class USceneComponent;

UCLASS()
class PLAYER_WEAPONS_API AWeapon_manager : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = Scene_comp) USceneComponent* mp_scene_comp = nullptr;

    const float	mk_reload_time			= 2.f;
    float		m_current_reload_time	= 0.f;
    float		m_shoot_time			= 0.25f;
    float		m_current_shoot_time	= 0.f;
    bool		m_is_reloading		    = false;
    bool		m_is_interacting	    = false;
    bool		m_is_changed_shoot_type = true;
    bool		m_is_shooting			= false;

public:
	/** \brief 현재 착용 중인 무기 */
	UPROPERTY() ACore_weapon*			p_first_gun	 = nullptr;
    UPROPERTY() ACore_weapon*			p_second_gun = nullptr;
    UPROPERTY() ACore_weapon*			p_pistol	 = nullptr;
    UPROPERTY() ACore_melee_weapon*		p_melee		 = nullptr;
    UPROPERTY() ACore_throwable_weapon* p_throwable	 = nullptr;

    e_current_weapon_type current_weapon_type = e_current_weapon_type::NONE;
	bool arr_is_weapon_equipped[5]{ false };

public:	
	AWeapon_manager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	/* \brief 착용 중인 무기를 체크함 */
	// 미구현상태
    void Check_for_equipped_weapon();
	
	/**
     * \brief 현재 장착 중인 무기에 따라 착용 여부 \n
	 * arr_is_weapon_equipped 배열 업데이트
	 */
	void Update_current_equipped_weapon();
	
	/**
	 * \brief 소리 재생을 해주는 함수
	 * \param _sound_type 소리종류
	 */
    void Play_sound(e_weapon_sound_type _sound_type);

    /**
     * \brief 배열 내에 원소 찾음
	 * \param _direction 방향
	 * \param _start_index 시작 위치
	 * \return e_current_weapon_type 현재 무기 타입
     */
    e_current_weapon_type Find_weapon_index(FString _direction, int _start_index);

    /**
     * \brief 무기를 플레이어 메시에 부착
	 * \param _p_tmp_weapon 무기 종류
	 * \param _socket_name 소켓 이름
     */
    void Attach_weapon(ABase_interaction* _p_tmp_weapon, FString _socket_name);

	/**
	 * \brief 무기 버린 후 초기화
	 * \param _weapon 맵에다가 버릴 무기
	 * \param _new_pos 새로운 위치
	 */
    void Reset_weapon_after_detaching(ABase_interaction* _weapon, FTransform _new_pos);

public:
	/**
	 * \brief 무기 착용
	 * \param _p_weapon 착용할 무기
	 */
	void Equip(AActor* _p_weapon);

	/** \brief 발사 */
	void Shoot();

	/** \brief 재장전 */
	void Reload();

	/**
	 * \brief 마우스 휠 통해 무기 교체
	 * \param _pos 현재 착용 중인 무기의 위치
	 * \return boolean 교체 여부
	 */
	bool Scroll_select(int _pos);

	/**
	 * \brief 무기 교체
	 * \param _current_weapon 현재 착용 중인 무기
	 * \param _new_weapon 새로운 무기
	 * \param _socket_name 메시 소켓 명칭
	 */
	void Swap(ABase_interaction* _current_weapon, AActor* _new_weapon, FString _socket_name);

	/**
	  * \brief 착용 중인 무기를 체크함 
	 */
	void Change_shoot_mode();

	// 미구현 상태
	void Change_aim_pose(int);

	/**
	 * \brief 재장전 중인지 확인
	 * \param _transcurred_reload_time 현재 재장전으로부터 초과한 시간
	 */
	void Check_if_reloading(float _transcurred_reload_time);

	/**
	 * \brief 연사 하고있는 중인지 체크
	 * \param _transcurred_shoot_time 발사 시간 간격
	 */
	void Check_continously_shooting(float _transcurred_shoot_time);

	/**
	 * \brief 무기 변경 가능한지 확인
	 * \param _weapon_type 무기 종류
	 * \return 변경 가능 여부
	 */
	bool Check_if_weapon_available(e_current_weapon_type _weapon_type);

	// ------- 부착 관련 함수 -------

	/**
	 * \brief 총기 최대 총알 개수를 가지고 옴
	 * \param _weapon_type 무기 종류
	 * \return 총알 최대 개수
	 */
    int Get_weapon_max_bullet_count(e_current_weapon_type _weapon_type);

    /**
     * \brief 현재 착용 중인 무기를 갖고옴
     * \param _weapon_type 무기 종류
     * \return 현재 착용 중인 무기
     */
	ABase_interaction* Get_weapon(e_current_weapon_type _weapon_type);

	// ------- UI 관련 함수 -------

	/**
	 * \brief 무기를 맵에다가 버림
	 * \param _p_weapon 버릴 무기
	 */
	void Drop(ABase_interaction* _p_weapon);
};