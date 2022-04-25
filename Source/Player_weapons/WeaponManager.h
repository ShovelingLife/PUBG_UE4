﻿/**
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
class USplineComponent;
class USplineMeshComponent;
class UStaticMesh;
class UMaterial;

UCLASS()
class PLAYER_WEAPONS_API AWeaponManager : public AActor
{
	GENERATED_BODY()

private:
	// 투척류 관련
    UPROPERTY(EditAnywhere, Category = GrenadeVariable) TSubclassOf<AActor> BP_GrenadeEndPoint = nullptr;
	UPROPERTY(EditAnywhere, Category = GrenadeVariable) AActor* GrenadeEndPoint = nullptr;
    UPROPERTY(EditAnywhere, Category = GrenadeVariable) USplineComponent* SplineComp = nullptr;
    FVector mGrenadeVelocity;
    bool  mbThrowingGrenade = false;

	// 총기 관련
    const float	mkReloadTime	   = 2.f;
    float		mCurrentReloadTime = 0.f;
    float		mShootTime		   = 0.25f;
    float		mCurrentShootTime  = 0.f;
    bool		mbReloading		   = false;
	bool		mbChangedShootType = true;

public:
	/** \brief 현재 착용 중인 무기 */
	UPROPERTY() ACoreWeapon*		  pFirstGun  = nullptr;
    UPROPERTY() ACoreWeapon*		  pSecondGun = nullptr;
    UPROPERTY() ACoreWeapon*		  pPistol	 = nullptr;
    UPROPERTY() ACoreMeleeWeapon*	  pMelee	 = nullptr;
    UPROPERTY() ACoreThrowableWeapon* pThrowable = nullptr;

    ECurrentWeaponType CurrentWeaponType = ECurrentWeaponType::NONE;
    float GrenadeDirection;
	bool			   bArrWeaponEquipped[5]{ false };
	bool			   bShooting = false;

	// 투척류 관련
	UPROPERTY(EditAnywhere, Category = GrenadeVariable) UStaticMesh* PathMesh;
    UPROPERTY(EditAnywhere, Category = GrenadeVariable) UMaterial*	 PathMat;
    UPROPERTY(EditAnywhere, Category = GrenadeVariable) TArray<USplineMeshComponent*> arrSplineMeshComp;
	UPROPERTY(EditAnywhere, Category = GrenadeVariable) float GrenadeSpeed = 0.f;

public:	
	AWeaponManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	/* \brief 착용 중인 무기를 체크함 */
	// 미구현상태
    void CheckForEquippedWeapon();

    void InitGrenadePath();

    bool IsAmmoInsufficient(int BulletCount);

	/**
     * \brief 현재 장착 중인 무기에 따라 착용 여부 \n
	 * arr_is_weapon_equipped 배열 업데이트
	 */
	void UpdateCurrentWeaponArr();
	
	/**
	 * \brief 소리 재생을 해주는 함수
	 * \param SoundType 소리종류
	 */
    void PlaySound(EWeaponSoundType SoundType);

    /**
     * \brief 배열 내에 원소 찾음
	 * \param Direction 방향
	 * \param StartIndex 시작 위치
	 * \return e_current_weapon_type 현재 무기 타입
     */
    ECurrentWeaponType GetWeaponIndex(FString Direction, int StartIndex);

    /**
     * \brief 무기를 플레이어 메시에 부착
	 * \param pNewWeapon 무기 종류
	 * \param SocketName 소켓 이름
	 * \param bCheck 무기 중복 여부 체크
     */
    void Attach(ABaseInteraction* pNewWeapon, FString SocketName, bool bCheck = true);

	/**
	 * \brief 무기 버린 후 초기화
	 * \param pWeapon 맵에다가 버릴 무기
	 * \param NewPos 새로운 위치
	 */
    void ResetAfterDetaching(ABaseInteraction* pWeapon, FTransform NewPos);

	void PredictGrenadePath();

public:
	/**
	 * \brief 무기 착용
	 * \param pWeapon 착용할 무기 
	 * \param bCheck 무기 중복 여부 체크
	 */
	void Equip(AActor* pWeapon, bool bCheck = true);

	/** \brief 발사 */
	void Shoot();

	/** \brief 재장전 */
	void Reload();

	void ThrowGrenade();

	void ResetGrenadePath();

	void UpdateGrenadePath();

	/**
	 * \brief 마우스 휠 통해 무기 교체
	 * \param Pos 현재 착용 중인 무기의 위치
	 */
	bool ScrollSelect(FString Pos);

	/**
	 * \brief 무기 교체
	 * \param CurrentWeapon 현재 착용 중인 무기
	 * \param NewWeapon 새로운 무기
	 * \param SocketName 메시 소켓 명칭
	 */
	void SwapWorld(ABaseInteraction* pNewWeapon, AActor* pCurrentWeapon, FString SocketName);

	// Exit Code = -1 Error / 0 Succeded / 1 Is Melee or Is Throwable
	int Swap(ABaseInteraction* pCurrentWeapon, ABaseInteraction* pNewWeapon = nullptr, ECurrentWeaponType WeaponType = ECurrentWeaponType::NONE);

	/** \brief 착용 중인 무기를 체크함 */
	void ChangeShootMode();

	// 미구현 상태
	void ChangeAimPose(int);

	/**
	 * \brief 재장전 중인지 확인
	 * \param TranscurredTime 현재 재장전으로부터 초과한 시간
	 */
	void CheckReloading(float TranscurredTime);

	/**
	 * \brief 연사 하고있는 중인지 체크
	 * \param TranscurredTime 발사 시간 간격
	 */
	void CheckContinouslyShooting(float TranscurredTime);

	/**
	 * \brief 무기 변경 가능한지 확인
	 * \param WeaponType 무기 종류
	 */
	bool IsWeaponAvailable(ECurrentWeaponType WeaponType);

	// ------- 부착 관련 함수 -------

    /**
     * \brief 현재 착용 중인 무기를 갖고옴
     * \param WeaponType 무기 종류
     * \return 현재 착용 중인 무기
     */
	ABaseInteraction* GetWeaponByIndex(ECurrentWeaponType WeaponType);

	// 무기의 인덱스를 구함
	ECurrentWeaponType GetWeaponIndex(ABaseInteraction* pWeapon);

	// 무기의 종류를 구함
	int GetWeaponType(ABaseInteraction* pWeapon);

	// ------- UI 관련 함수 -------

	/**
	 * \brief 무기를 맵에다가 버림
	 * \param WeaponType 버릴 무기 종류
	 */
	void Drop(ECurrentWeaponType WeaponType);

	void SetNull(ECurrentWeaponType WeaponType);

	void SetMeshToPlayerUI(TArray<AActor*> pArrActor, USkeletalMeshComponent* SkeletalMeshComp);

	bool IsWrong(ABaseInteraction* pWeapon, ECurrentWeaponType WeaponType, bool bFromWeaponSlot);
};