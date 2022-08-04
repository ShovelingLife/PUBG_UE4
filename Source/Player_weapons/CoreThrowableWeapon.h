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
#include "PUBG_UE4/WeaponEnum.h"
#include "PUBG_UE4/BaseInteraction.h"
#include "PUBG_UE4/MyEnum.h"
#include "PUBG_UE4/OtherWeaponData.h"
#include "CoreThrowableWeapon.generated.h"

DECLARE_DELEGATE(FDeleExplosionEvent)

class UCustomGameInstance;

class UProjectileMovementComponent;
class UParticleSystemComponent;
class URadialForceComponent;
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class PLAYER_WEAPONS_API ACoreThrowableWeapon : public ABaseInteraction
{
	GENERATED_BODY()

private:
    UPROPERTY(VisibleAnywhere) UCustomGameInstance* mpCustomGameInstance = nullptr;
    FTimerHandle mWaitHandle;
    bool mbPlayed = false;

public:
    FVector GrenadeEndPos;
    bool bThrowed = false;

protected:
    FDeleExplosionEvent mCallBack;

public:
    UPROPERTY(EditAnywhere, category = Collider) class USphereComponent* SphereComp;
    UPROPERTY(VisibleAnywhere, Category = ProjectileMovementComp) UProjectileMovementComponent* ProjectileMovementComp = nullptr;    
    UPROPERTY(VisibleAnywhere) UParticleSystemComponent* GrenadeParticleComp;
    UPROPERTY(EditAnywhere) URadialForceComponent* RadialForceComp;
    FsOtherWeaponData    WeaponData;
	EThrowableWeaponType WeaponType = THROWABLE_MAX;

public:
    ACoreThrowableWeapon();

    ACoreThrowableWeapon(EThrowableWeaponType WeaponType);

    // 대입 연산자
    FORCEINLINE	ACoreThrowableWeapon& operator=(ACoreThrowableWeapon& Src)
    {
        // swap 함수 사용함으로서 복사 오버헤드 방지
        std::swap(this->StaticMeshComp, Src.StaticMeshComp);
        std::swap(this->WeaponData, Src.WeaponData);
        return *this;
    }

public:
    virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

    UFUNCTION() void BeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION() void EndOverlap(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    virtual void BeginDestroy() override;

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    /** \brief 파티클 시스템 생성 */
    virtual void InitParticleSystem(FString Path = "") override;

    /** \brief 함수 포인터에 이벤트 바인딩 */
    void BindExplosionFunc();

    /** \brief 이동 컴포넌트 초기화 */
    void InitProjectileMovementComp();

    /** \brief 메쉬 초기화 */
    void InitMesh();

    void InitSphereComp();

    void InitRadialForce();

    /** \brief 플레이어와 투척류 간 거리 계산= */
    bool IsPlayerInRadius();

public:
    // 투척류 기본 설정
    void Setup(ACoreThrowableWeapon* OtherWeapon);

    void Throw(FVector Velocity);
};