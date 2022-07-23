/**
 * \file Core_bullet.h
 *
 * \brief ��� �Ѿ˵��� �̷κ��� ��ӹ���
 *
 * \ingroup Player_weapons
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PUBG_UE4/DataTableManager.h"
#include "PUBG_UE4/WeaponEnum.h"
#include "CoreBullet.generated.h"

class UCapsuleComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UProjectileMovementComponent;
class URadialForceComponent;
class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class OBJECTS_API ACoreBullet : public AActor
{
    GENERATED_BODY()

protected:
    FsWeaponData mWeaponData;
    const float	 mkBulletSpeed = 2.f;
    const float	 mkLifeTime = 3.f;
    float		 mCurrentLifeTime = 0.f;
    float		 mCurrentSpeed = 0.f;
    bool mCollided = false;

public:
    UPROPERTY(VisibleAnywhere, Category = Mesh)   UStaticMeshComponent* MeshComp;
    UPROPERTY(VisibleAnywhere, Category = Bullet) UProjectileMovementComponent* ProjectileMovementComp;
    UPROPERTY(VisibleAnywhere) URadialForceComponent* RadialForceComp;

    // ����Ʈ ����
    UPROPERTY(VisibleAnywhere, Category = Effect) UNiagaraComponent* NiagaraComp;
    UPROPERTY(VisibleAnywhere, Category = Effect) UNiagaraSystem* TrailEffect;
    UPROPERTY(VisibleAnywhere, Category = Effect) UNiagaraSystem* ImpactEffect;

public:
    ACoreBullet();

protected:
    virtual void BeginPlay() override;

    /** \brief �浹 �� ������Ʈ �Ҹ� */
    virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

public:
    virtual void Tick(float) override;

private:
    /** \brief �޽� ������Ʈ �ʱ�ȭ */
    void InitMesh();

    /** \brief �Ѿ� ������Ʈ �ʱ�ȭ */
    void InitProjectileMovementComp();

    void InitVFX();

protected:
    /**
      * \brief �Ѿ� ������Ʈ �ʱ�ȭ
      * \param _Index ���� ����
     */
    void Init(EWeaponType _Index);
};