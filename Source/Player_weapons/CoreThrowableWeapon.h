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
#include "PUBG_UE4/MyEnum.h"
#include "PUBG_UE4/OtherWeaponData.h"
#include "CoreThrowableWeapon.generated.h"

DECLARE_DELEGATE(FDeleExplosionEvent)

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UCustomGameInstance;

UCLASS()
class PLAYER_WEAPONS_API ACoreThrowableWeapon : public ABaseInteraction
{
	GENERATED_BODY()

private:
    FTimerHandle mWaitHandle;
    bool mbPlayed = false;
    bool mbExploded = false;

public:
    FVector GrenadeEndPos;

protected:
    FDeleExplosionEvent mCallBack;

public:
    UPROPERTY(EditAnywhere, category = Collider) class USphereComponent* SphereComp;
    UPROPERTY(VisibleAnywhere, Category = ProjectileMovementComp) UProjectileMovementComponent* ProjectileMovementComp = nullptr;    
    UPROPERTY(VisibleAnywhere) UParticleSystemComponent* GrenadeParticleComp;
    FsOtherWeaponData    WeaponData;
	EThrowableWeaponType CurrentWeaponType = EThrowableWeaponType::MAX;

public:
    virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

    UFUNCTION() void BeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION() void EndOverlap(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    virtual void BeginDestroy() override;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

protected:
    /** \brief 파티클 시스템 생성 */
    virtual void InitParticleSystem(FString Path = "") override;

    /**
      * \brief 투척류 무기 초기화
      * \param _weapon_type 무기 종류
     */
    void Init(EThrowableWeaponType WeaponType);

    /** \brief 이동 컴포넌트 초기화 */
    void InitProjectileMovementComp();

    /** \brief 메쉬 초기화 */
    void InitMesh();

    void InitSphereComp();

    /** \brief 플레이어와 투척류 간 거리 계산= */
    bool IsPlayerInRadius();

public:
    void Throw(FVector Velocity);
};