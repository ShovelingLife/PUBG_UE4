/**
 * \file Core_throwable_weapon.h
 *
 * \brief 모든 투척류 무기들은 이로부터 상속받음
 *
 * \ingroup Player_weapons
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "WeaponEnum.h"
#include "PUBG_UE4/BaseInteraction.h"
#include "PUBG_UE4/OtherWeaponData.h"
#include "CoreThrowableWeapon.generated.h"

UCLASS()
class PLAYER_WEAPONS_API ACoreThrowableWeapon : public ABaseInteraction
{
	GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, Category = Particle) class UParticleSystemComponent* ParticleComp = nullptr;

public:
	FsOtherWeaponData WeaponData;
	EThrowableWeaponType WeaponType;

public:
    ACoreThrowableWeapon();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float) override;

protected:
    /**
      * \brief 투척류 무기 초기화
      * \param _weapon_type 무기 종류
     */
    void Init(EThrowableWeaponType _weapon_type);

    /**
      * \brief 메쉬 초기화
     */
    void InitMesh();

    /**
      * \brief 콜라이더 정보 갱신
     */
    void Update_collider();

    /**
      * \brief 파티클 시스템 초기화
     */
    void InitParticleSystem();
};