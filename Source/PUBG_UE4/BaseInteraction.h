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
#include "BaseInteraction.generated.h"

class UAudioComponent;
class UBoxComponent;
class USceneComponent;
class USkeletalMeshComponent;
class UStaticMeshComponent;
class UWidgetComponent;
class UParticleSystemComponent;

UCLASS()
class PUBG_UE4_API ABaseInteraction : public AActor
{
	GENERATED_BODY()
	
protected: 
    UPROPERTY(VisibleAnywhere, Category = Widget) UWidgetComponent* WidgetComp;

    float mCurrentTime = 0.f;

public:
    UPROPERTY(VisibleAnywhere, Category = Collider) UBoxComponent*            ColliderComp     = nullptr;
    UPROPERTY(VisibleAnywhere, Category = Mesh)     UStaticMeshComponent*     StaticMeshComp   = nullptr;
    UPROPERTY(VisibleAnywhere, Category = Mesh)     USkeletalMeshComponent*   SkeletalMeshComp = nullptr;
    UPROPERTY(VisibleAnywhere, Category = Sound)    UAudioComponent*          AudioComp        = nullptr;
    UPROPERTY(VisibleAnywhere, Category = Particle) UParticleSystemComponent* ParticleComp     = nullptr;

    FString ObjectGroupType = "";
    FString ObjectType      = "";
    int     UI_index        = 0;
    bool    bPlayerNear     = false;

public:
    ABaseInteraction();

protected:
    virtual void BeginPlay() override;

    /**
      * \brief 플레이어가 접근했을 시 상호작용 UI 띄움
      * \param _delta_time 프레임 초
     */
    virtual void Tick(float _delta_time) override;

protected:
    /**
      * \brief 스태틱 메시 컴포넌트 초기화 (무기,아이템)
      * \param _path 메시 경로
     */
    void InitStaticMesh(FString _path);

    /**
      * \brief 스켈레탈 메시 컴포넌트 초기화 (캐릭터,차량)
      * \param _path 메시 경로
     */
    void InitSkeletalMesh(FString _path);

    /** \brief 오디오 컴포넌트 초기화 */
    void InitComponents();

    /** \brief 상호작용 UI 초기화 */
    void InitInteractionUI();

    /**
      * \brief 파티클 시스템 초기화
      * \param FString 파티클 경로
     */
    void InitParticleSystem(FString _Path);

    void AttachComponents();

    void SetCollisionSettingsForObjects();

public:
    void DestroyComponentsForUI();
};