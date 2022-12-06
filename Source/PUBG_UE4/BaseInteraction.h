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

class UInteractionComponent;

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
    float mCurrentTime = 0.f;

public:
    UPROPERTY(VisibleAnywhere, Category = Mesh)     
    UStaticMeshComponent* StaticMeshComp = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Mesh)     
    USkeletalMeshComponent* SkeletalMeshComp = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Particle) 
    UParticleSystemComponent* ParticleComp = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Particle) 
    UParticleSystem* Particle = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Collider)
    UBoxComponent* ColliderComp = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Interaction)
    UInteractionComponent* InteractionComp = nullptr;

    FString ObjectGroupType = "";

    // 위젯 컴포넌트 UI용
    FString ObjectType = "";
    int     ItemIdx = 0;

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

    void InitCollider();

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

    // UI일 시 기타 컴포넌트를 파괴
    void DestroyComponentsForUI();

    // 인벤토리 내 더미 캐릭터 UI 설정
    void SetForDummyCharacter();

    // 충돌체 설정 변경
    void ChangeCollisionSettings(bool bTurned = true);

    // 컴포넌트 충돌 관한 설정 변경
    void ChangeCollisionSettings(UPrimitiveComponent* MeshComp, bool bTurned);

    // 메쉬에게 부착 주로 플레이어에게 사용
    void AttachToMesh(USceneComponent* RootComp, FString SocketName);

    // 무기를 탈착
    void Detach(FVector NewPos);

    void TurnUI(bool bOnOff = true);

    // 현재 오브젝트가 찾고자하는 타입인지 확인
    bool IsGroupType(FString Type) { return ObjectGroupType == Type; }
};