/**
 * \file Base_interaction.h
 *
 * \brief 상호작용 가능한 모든 오브젝트들의 부모 클래스
 *
 * \ingroup PUBG_UE4
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponEnum.h"
#include "WeaponInterface.h"
#include "BaseInteraction.generated.h"

class UBoxComponent;
class UParticleSystemComponent;
class USceneComponent;
class USkeletalMeshComponent;
class UStaticMeshComponent;
class UTexture;
class UWidgetComponent;

UCLASS()
class PUBG_UE4_API ABaseInteraction : public AActor, public IWeaponInterface
{
	GENERATED_BODY()
	
using enum EGunType;
using enum EThrowableWeaponType;
using enum EMeleeWeaponType;

protected: 
    UPROPERTY(VisibleAnywhere, Category = Widget) UWidgetComponent* WidgetComp;

    float mCurrentTime = 0.f;

public:
    UPROPERTY(VisibleAnywhere, Category = Collider) UBoxComponent*            ColliderComp     = nullptr;
    UPROPERTY(VisibleAnywhere, Category = Mesh)     UStaticMeshComponent*     StaticMeshComp   = nullptr;
    UPROPERTY(VisibleAnywhere, Category = Mesh)     USkeletalMeshComponent*   SkeletalMeshComp = nullptr;
    UPROPERTY(VisibleAnywhere, Category = Particle) UParticleSystemComponent* ParticleComp     = nullptr;
    UPROPERTY(VisibleAnywhere, Category = Particle) UParticleSystem*          Particle         = nullptr;
    FString ObjectGroupType = "";
    FString ObjectType      = "";
    int     UI_index        = 0;
    bool    bPlayerNear     = false;

    UPROPERTY() UTexture* CurrentItemTex;

public:
    ABaseInteraction();

protected:
    virtual void BeginPlay() override;

    /**
      * \brief 플레이어가 접근했을 시 상호작용 UI 띄움
      * \param DeltaTime 프레임 초
     */
    virtual void Tick(float DeltaTime) override;

public:
    virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

    // 클릭 이벤트
    virtual void ClickEvent() override { };

protected:
    /** \brief 오디오 컴포넌트 초기화 */
    void InitComponents();

    /** \brief 상호작용 UI 초기화 */
    void InitInteractionUI();

    void AttachComponents();

    void SetCollisionSettingsForObjects();

    /**
      * \brief 스태틱 메시 컴포넌트 초기화 (무기,아이템)
      * \param Path 메시 경로
     */
    void InitStaticMesh(FString Path);

    /**
      * \brief 스켈레탈 메시 컴포넌트 초기화 (캐릭터,차량)
      * \param Path 메시 경로
     */
    void InitSkeletalMesh(FString Path);

    /**
      * \brief 파티클 시스템 초기화
      * \param Path 파티클 경로
     */
    virtual void InitParticleSystem(FString Path = "");

public:
    // ------- 게터 세터 -------
    UStaticMesh* GetStaticMesh() const;

    USkeletalMesh* GetSkeletalMesh() const;

    void SetStaticMesh(UStaticMesh* Mesh);

    void SetSkeletalMesh(USkeletalMesh* Mesh);

    // ------- 그 외 함수들 -------
    void DestroyComponentsForUI();

    void SetForDummyCharacter();

    void ChangeCollisionSettings(bool bTurned);

    void AttachToMesh(USceneComponent* RootComp, FString SocketName);

    void Detach(FTransform NewPos);
};