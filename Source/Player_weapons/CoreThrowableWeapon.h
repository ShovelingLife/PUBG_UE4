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
#include "InterfaceExplodeFunc.h"
#include "PUBG_UE4/BaseInteraction.h"
#include "PUBG_UE4/MyEnum.h"
#include "PUBG_UE4/OtherWeaponData.h"
#include "CoreThrowableWeapon.generated.h"

class UCapsuleComponent;
class UProjectileMovementComponent;

UCLASS()
class PLAYER_WEAPONS_API ACoreThrowableWeapon : public ABaseInteraction, public IInterfaceExplodeFunc
{
	GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, Category = Collider) UCapsuleComponent* GrenadeColliderComp = nullptr;
    UPROPERTY(VisibleAnywhere, Category = ProjectileMovementComp) UProjectileMovementComponent* ProjectileMovementComp = nullptr;
	FsOtherWeaponData    WeaponData;
	EThrowableWeaponType CurrentWeaponType = EThrowableWeaponType::MAX;
    bool bTouchedFloor = false;
    bool bThrowed = false;

public:
    ACoreThrowableWeapon();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
    virtual void BeginDestroy() override;

    virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
    /**
      * \brief 투척류 무기 초기화
      * \param _weapon_type 무기 종류
     */
    void Init(EThrowableWeaponType WeaponType);

    /** \brief 이동 컴포넌트 초기화 */
    void InitProjectileMovementComp();

    /** \brief 메쉬 초기화 */
    void InitMesh();

    /** \brief 콜라이더 정보 갱신 */
    void UpdateCollider();

    /** \brief 파티클 시스템 갱신 */
    void UpdateParticleSystem();

    /** \brief 플레이어와 투척류 간 거리 계산= */
    bool IsPlayerInRadius();

    virtual void Explode() override;

public:
    void Throw(FVector Velocity);
};