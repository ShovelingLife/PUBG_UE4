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
#include "PUBG_UE4/BaseInteraction.h"
#include "Objects/CoreBullet.h"
#include "PUBG_UE4/WeaponEnum.h"
#include "PUBG_UE4/MyEnum.h"
#include "PUBG_UE4/DataTableManager.h"
#include "CoreWeapon.generated.h"

class UAudioComponent;
//class ACoreBullet;
class UCameraComponent;
class UGunRecoilComponent;
class UParticleSystemComponent;
class USoundBase;

// 부속품들
class ACoreBarrel;
class ACoreForend;
class ACoreGrip;
class ACoreSight;
class ACoreStock;
class UCustomGameInstance;

UCLASS()
class PLAYER_WEAPONS_API ACoreWeapon : public ABaseInteraction
{
    GENERATED_BODY()

using enum EGunShootType;
using enum EWeaponSoundType;

private:
    const float	mkReloadTime = 2.f; // 재장전 시간

    // 총기 속성 관련
    FTimerHandle mBurstTimerHandle; // 점사 타이머 핸들
    float mCurrentReloadTime = 0.f;
    float mCurrentShootTime  = 0.f;
    float mNextShootTime     = 0.f;
    int	  mBurstCount		 = 0;
    bool  mbReloading		 = false;
	bool  mbChangedShootType = true;

    // 총기 반동 관련

public:
    // 총알 관련
    UPROPERTY(VisibleAnywhere, Category = Bullet) 
    TSubclassOf<ACoreBullet> BP_Bullet;

    // class ACore_gun_mag* p_gun_mag = nullptr;
    FsWeaponData  WeaponData;
    EGunType      WeaponType = EGunType::GUN_MAX;
    EGunShootType ShootType  = EGunShootType::SINGLE;

    // 부속품 관련 변수들
    UPROPERTY(VisibleAnywhere) // 소음기
    ACoreBarrel* CurrentBarrel = nullptr;

    UPROPERTY(VisibleAnywhere) // 탄알집
    ACoreForend* CurrentForend = nullptr;

    UPROPERTY(VisibleAnywhere) // 손잡이
    ACoreGrip* CurrentGrip = nullptr;

    UPROPERTY(VisibleAnywhere) // 조준경
    ACoreSight* CurrentSight = nullptr;

    UPROPERTY(VisibleAnywhere) // 개머리판
    ACoreStock* CurrentStock = nullptr;

    UPROPERTY(EditAnywhere)
    UGunRecoilComponent* GunRecoilComponent = nullptr;

    bool bInInventory = false;
    bool bShooting    = false;

public:
    ACoreWeapon();

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    virtual void ResetSettings() final;

public:
    virtual void ClickEvent() final;

protected:
    /**
      * \brief 무기 초기화
      * \param 무기 종류
     */
    void Init(EGunType WeaponType);

    // 탄알 초기화
    void InitBullet();

    // 파티클 시스템 갱신
    void UpdateParticleSystem();

    void PlaySound(EWeaponSoundType SoundType);

    /**
     * \brief 재장전 중인지 확인
     * \param TranscurredTime 현재 재장전으로부터 초과한 시간
     */
    void CheckReloading(float DeltaTime);

    void FireBullet();

    void ResetBurstCount();

public:
    // 게터 모음
    EGunShootType GetNextShootType() const;

    EGunShootType GetMaxShootType() const;

    FString GetShootTypeStr() const;

    UTexture* GetAttachmentTex(int Idx) const;

    void Reload();

    void ChangeShootMode();
};