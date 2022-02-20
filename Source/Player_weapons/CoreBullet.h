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

public:
    UPROPERTY(VisibleAnywhere, Category = Scene_comp) USceneComponent*              SceneComp;
    UPROPERTY(VisibleAnywhere, Category = Collider)   UCapsuleComponent*            ColliderComp;
    UPROPERTY(VisibleAnywhere, Category = Mesh)       UStaticMeshComponent*         MeshComp;
    UPROPERTY(VisibleAnywhere, Category = Bullet)     UProjectileMovementComponent* ProjectileMovementComp;

public:
    ACoreBullet();


protected:
    virtual void BeginPlay() override;

    /**
      * \brief 충돌 시 오브젝트 소멸 (UFUNCTION)
     */
    UFUNCTION()
    void OnHit(class UPrimitiveComponent* _my_comp, AActor* _other, class UPrimitiveComponent* _other_comp, FVector _normal_impulse, const FHitResult& _hit);

public:
    virtual void Tick(float) override;

private:
    /**
      * \brief 메시 컴포넌트 초기화
     */
    void InitMesh();

    /**
      * \brief 충돌체 컴포넌트 초기화
     */
    void InitCollider();

    /**
      * \brief 총알 컴포넌트 초기화
     */
    void InitProjectileMovementComp();

protected:
    /**
      * \brief 총알 오브젝트 초기화
      * \param _Index 무기 종류
     */
    void Init(EWeaponType _Index);
};
