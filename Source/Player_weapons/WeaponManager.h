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
#include "PUBG_UE4/MyEnum.h"
#include "WeaponEnum.h"
#include "WeaponManager.generated.h"

class ABaseInteraction;
class ACoreWeapon;
class ACoreMeleeWeapon;
class ACoreThrowableWeapon;
class AWeaponManager;
class USceneComponent;

UCLASS()
class PLAYER_WEAPONS_API AWeaponManager : public AActor
{
	GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, Category = Scene_comp) USceneComponent* SceneComp = nullptr;

    const float	mkReloadTime	   = 2.f;
    float		mCurrentReloadTime = 0.f;
    float		mShootTime		   = 0.25f;
    float		mCurrentShootTime  = 0.f;
    bool		mbReloading		   = false;
    bool		mbInteracting	   = false;
    bool		mbChangedShootType = true;
	bool		mbThrowingGrenade  = false;

public:
	/** \brief 현재 착용 중인 무기 */
	UPROPERTY() ACoreWeapon*		  pFirstGun  = nullptr;
    UPROPERTY() ACoreWeapon*		  pSecondGun = nullptr;
    UPROPERTY() ACoreWeapon*		  pPistol	 = nullptr;
    UPROPERTY() ACoreMeleeWeapon*	  pMelee	 = nullptr;
    UPROPERTY() ACoreThrowableWeapon* pThrowable = nullptr;

    ECurrentWeaponType CurrentWeaponType = ECurrentWeaponType::NONE;
	FVector			   GrenadePathPredictPos;
	bool			   bArrWeaponEquipped[5]{ false };
	bool			   bShooting = false;

public:	
	AWeaponManager();

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
	void UpdateCurrentWeaponArr();
	
	/**
	 * \brief 소리 재생을 해주는 함수
	 * \param _sound_type 소리종류
	 */
    void PlaySound(EWeaponSoundType _sound_type);

    /**
     * \brief 배열 내에 원소 찾음
	 * \param _direction 방향
	 * \param _start_index 시작 위치
	 * \return e_current_weapon_type 현재 무기 타입
     */
    ECurrentWeaponType GetWeaponIndex(FString _direction, int _start_index);

    /**
     * \brief 무기를 플레이어 메시에 부착
	 * \param _p_tmp_weapon 무기 종류
	 * \param _socket_name 소켓 이름
	 * \param _should_check 무기 중복 여부 체크
     */
    void Attach(ABaseInteraction* _p_tmp_weapon, FString _socket_name, bool _should_check = true);

	/**
	 * \brief 무기 버린 후 초기화
	 * \param _p_weapon 맵에다가 버릴 무기
	 * \param _new_pos 새로운 위치
	 */
    void ResetAfterDetaching(ABaseInteraction* _p_weapon, FTransform _new_pos);

	void CheckForGrenadePath();

public:
	/**
	 * \brief 무기 착용
	 * \param _p_weapon 착용할 무기 
	 * \param _should_check 무기 중복 여부 체크
	 */
	void Equip(AActor* _p_weapon, bool _should_check = true);

	/** \brief 발사 */
	void Shoot();

	/** \brief 재장전 */
	void Reload();

	/**
	 * \brief 마우스 휠 통해 무기 교체
	 * \param _Pos 현재 착용 중인 무기의 위치
	 */
	bool ScrollSelect(FString _Pos);

	/**
	 * \brief 무기 교체
	 * \param _current_weapon 현재 착용 중인 무기
	 * \param _new_weapon 새로운 무기
	 * \param _socket_name 메시 소켓 명칭
	 */
	void SwapWorld(ABaseInteraction* _current_weapon, AActor* _new_weapon, FString _socket_name);

	bool Swap(ABaseInteraction* _p_old_weapon, ABaseInteraction* _p_new_weapon = nullptr, ECurrentWeaponType _weapon_type = ECurrentWeaponType::NONE);

	/** \brief 착용 중인 무기를 체크함 */
	void ChangeShootMode();

	// 미구현 상태
	void ChangeAimPose(int);

	/**
	 * \brief 재장전 중인지 확인
	 * \param _TranscurredShootTime 현재 재장전으로부터 초과한 시간
	 */
	void CheckIfReloading(float _TranscurredShootTime);

	/**
	 * \brief 연사 하고있는 중인지 체크
	 * \param _TranscurredShootTime 발사 시간 간격
	 */
	void CheckContinouslyShooting(float _TranscurredShootTime);

	/**
	 * \brief 무기 변경 가능한지 확인
	 * \param _WeaponType 무기 종류
	 */
	bool IsWeaponAvailable(ECurrentWeaponType _WeaponType);

	// ------- 부착 관련 함수 -------

	/**
	 * \brief 총기 최대 총알 개수를 가지고 옴
	 * \param _WeaponType 무기 종류
	 * \return 총알 최대 개수
	 */
    int GetMaxBulletCount(ECurrentWeaponType _WeaponType);

    /**
     * \brief 현재 착용 중인 무기를 갖고옴
     * \param _WeaponType 무기 종류
     * \return 현재 착용 중인 무기
     */
	ABaseInteraction* GetWeaponByIndex(ECurrentWeaponType _WeaponType);

	// 무기의 인덱스를 구함
	ECurrentWeaponType GetWeaponIndex(ABaseInteraction* _pWeapon);

	// 무기의 종류를 구함
	int GetWeaponType(ABaseInteraction* _pWeapon);

	// ------- UI 관련 함수 -------

	/**
	 * \brief 무기를 맵에다가 버림
	 * \param _WeaponType 버릴 무기 종류
	 */
	void Drop(ECurrentWeaponType _WeaponType);

	void SetNull(ECurrentWeaponType _WeaponType);

	void SetMeshToPlayerUI(TArray<AActor*> _pArrActor, USkeletalMeshComponent* _pSkeletalMeshComp);

	bool IsDuplicated(ABaseInteraction* _p_weapon, ECurrentWeaponType _weapon_type);
};