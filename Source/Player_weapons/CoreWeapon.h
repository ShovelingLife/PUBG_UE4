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
#include "WeaponEnum.h"
#include "PUBG_UE4/DataTableManager.h"
#include "CoreWeapon.generated.h"

class UAudioComponent;
class USoundBase;
class ACoreBullet;
class UParticleSystemComponent;

UCLASS()
class PLAYER_WEAPONS_API ACoreWeapon : public ABaseInteraction
{
	GENERATED_BODY()
	
public:
    // 총알 관련
    UPROPERTY(VisibleAnywhere, Category = Bullet) TSubclassOf< ACoreBullet > BP_Bullet;

    // class ACore_gun_mag* p_gun_mag = nullptr;
    FsWeaponData  WeaponData;
    EWeaponType   WeaponType = EWeaponType::MAX;
    EGunShootType ShootType  = EGunShootType::SINGLE;

public:
    ACoreWeapon();

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
    void Init(EWeaponType WeaponType);

    /** \brief 메쉬 초기화 */
    void InitMesh();

    /** \brief 탄알 초기화 */
    void InitBullet();

    /** \brief 콜라이더 정보 갱신 */
    void UpdateCollider();

    /** \brief 파티클 시스템 갱신 */
    void UpdateParticleSystem();
};