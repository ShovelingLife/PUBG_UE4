/**
 * \file Core_bullet.h
 *
 * \brief 모든 총알들은 이로부터 상속받음
 *
 * \ingroup Player_weapons
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PUBG_UE4/DataTableManager.h"
#include "WeaponEnum.h"
#include "CoreBullet.generated.h"

class UCapsuleComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class USceneComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class PLAYER_WEAPONS_API ACoreBullet : public AActor
{
	GENERATED_BODY()
	
protected:
    FsWeaponData mWeaponData;
    const float	 mkBulletSpeed    = 2.f;
    const float	 mkLifeTime       = 3.f;
    float		 mCurrentLifeTime = 0.f;
    float		 mCurrentSpeed    = 0.f;
    bool mCollided = false;

public:
    UPROPERTY(VisibleAnywhere, Category = Mesh)       UStaticMeshComponent*         MeshComp;
    UPROPERTY(VisibleAnywhere, Category = Bullet)     UProjectileMovementComponent* ProjectileMovementComp;

    // 이펙트 관련
    UPROPERTY(VisibleAnywhere, Category = Effect) UNiagaraComponent* NiagaraComp;
    UPROPERTY(VisibleAnywhere, Category = Effect) UNiagaraSystem* TrailEffect;
    UPROPERTY(VisibleAnywhere, Category = Effect) UNiagaraSystem* ImpactEffect;

public:
    ACoreBullet();

protected:
    virtual void BeginPlay() override;

    /** \brief 충돌 시 오브젝트 소멸 */
    virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

public:
    virtual void Tick(float) override;

private:
    /** \brief 메시 컴포넌트 초기화 */
    void InitMesh();

    /** \brief 총알 컴포넌트 초기화 */
    void InitProjectileMovementComp();

    void InitVFX();

protected:
    /**
      * \brief 총알 오브젝트 초기화
      * \param _Index 무기 종류
     */
    void Init(EWeaponType _Index);
};
