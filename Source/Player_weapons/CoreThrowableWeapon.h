#pragma once

#include "CoreMinimal.h"
#include "PUBG_UE4/WeaponEnum.h"
#include "PUBG_UE4/BaseInteraction.h"
#include "PUBG_UE4/MyEnum.h"
#include "PUBG_UE4/OtherWeaponData.h"
#include "CoreThrowableWeapon.generated.h"

DECLARE_DELEGATE(FDeleExplosionEvent)

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
    FTimerHandle mWaitHandle;
    bool mbPlayed = false;

protected:
    FDeleExplosionEvent mExplosionEvent;

public:
    UPROPERTY(EditAnywhere, category = Collider) 
    USphereComponent* SphereComp;

    UPROPERTY(VisibleAnywhere, Category = ProjectileMovementComp) 
    UProjectileMovementComponent* ProjectileMovementComp = nullptr;   

    UPROPERTY(VisibleAnywhere) 
    UParticleSystemComponent* GrenadeParticleComp;

    UPROPERTY(EditAnywhere) 
    URadialForceComponent* RadialForceComp;

    FsOtherWeaponData    WeaponData;
    EThrowableWeaponType WeaponType = THROWABLE_MAX;

    FVector GrenadeEndPos;
    bool bThrowed = false;

public:
    ACoreThrowableWeapon();

    ACoreThrowableWeapon(EThrowableWeaponType WeaponType);

    // 대입 연산자
    FORCEINLINE	ACoreThrowableWeapon& operator=(ACoreThrowableWeapon& Src)
    {
        using namespace std;

        // swap 함수 사용함으로서 복사 오버헤드 방지
        swap(this->StaticMeshComp, Src.StaticMeshComp);
        swap(this->WeaponData, Src.WeaponData);
        return *this;
    }

public:
    virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

    UFUNCTION() void BeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION() void EndOverlap(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    virtual void BeginDestroy() override;

    virtual void ClickEvent() final;

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    /** \brief 파티클 시스템 생성 */
    virtual void InitParticleSystem(FString Path = "") override;

protected:
    // ------- 초기화 함수 모음 -------

    /** \brief 이동 컴포넌트 초기화 */
    void InitProjectileMovementComp();

    /** \brief 메쉬 초기화 */
    void InitMesh();

    void InitSphereComp();

    void InitRadialForce();

    /** \brief 함수 포인터에 이벤트 바인딩 */
    void BindExplosionFunc();

    // 그 외 함수

    /** \brief 플레이어와 투척류 간 거리 계산= */
    bool IsPlayerInRadius();

public:
    // 투척류 기본 설정
    void Setup(ACoreThrowableWeapon* OtherWeapon);

    void Throw(FVector Velocity);
};