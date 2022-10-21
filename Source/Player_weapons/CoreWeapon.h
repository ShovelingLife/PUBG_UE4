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
class USoundBase;
//class ACoreBullet;
class UParticleSystemComponent;
class UCameraComponent;

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
    FTimerHandle mTimerHandle;

    const float	mkReloadTime = 2.f;
    float mCurrentReloadTime = 0.f;
    float mCurrentShootTime  = 0.f;
    float mNextShootTime = 0.f;
    int	  mBurstCount		 = 0;
    bool  mbReloading		 = false;
	bool  mbChangedShootType = true;

public:
    UPROPERTY(VisibleAnywhere, Category = Camera) UCameraComponent* CameraComp;

    // 총알 관련
    UPROPERTY(VisibleAnywhere, Category = Bullet) TSubclassOf< ACoreBullet > BP_Bullet;
    // class ACore_gun_mag* p_gun_mag = nullptr;
    FsWeaponData  WeaponData;
    EGunType      WeaponType = EGunType::GUN_MAX;
    EGunShootType ShootType  = EGunShootType::SINGLE;

    // 부속품 관련 변수들
    UPROPERTY(VisibleAnywhere) ACoreBarrel* CurrentBarrel = nullptr;
    UPROPERTY(VisibleAnywhere) ACoreForend* CurrentForend = nullptr;
    UPROPERTY(VisibleAnywhere) ACoreGrip*   CurrentGrip   = nullptr;
    UPROPERTY(VisibleAnywhere) ACoreSight*  CurrentSight  = nullptr;
    UPROPERTY(VisibleAnywhere) ACoreStock*  CurrentStock  = nullptr;

    bool bInInventory = false;
    bool  bShooting = false;

public:
    ACoreWeapon();

public:
    virtual void ClickEvent() final;

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
    void Init(EGunType WeaponType);

    /** \brief 탄알 초기화 */
    void InitBullet();

    /** \brief 파티클 시스템 갱신 */
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
    void Reload();

    void ChangeShootMode();

    EGunShootType GetNextShootType() const;

    EGunShootType GetMaxShootType() const;

    FString GetShootTypeStr() const;
};