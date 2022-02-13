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
#include "Base_interaction.generated.h"

class UAudioComponent;
class UBoxComponent;
class USceneComponent;
class USkeletalMeshComponent;
class UStaticMeshComponent;
class UWidgetComponent;

UCLASS()
class PUBG_UE4_API ABase_interaction : public AActor
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(VisibleAnywhere, Category = Collider) UBoxComponent* m_box_collider = nullptr;    
    UPROPERTY(VisibleAnywhere, Category = Widget) UWidgetComponent* mp_interaction_widget_comp;

    float   m_current_time = 0.f;

public:
    UPROPERTY(VisibleAnywhere, Category = Mesh)  UStaticMeshComponent*   p_static_mesh_comp   = nullptr;
    UPROPERTY(VisibleAnywhere, Category = Mesh)  USkeletalMeshComponent* p_skeletal_mesh_comp = nullptr;
    UPROPERTY(VisibleAnywhere, Category = Sound) UAudioComponent*        p_audio_comp         = nullptr;

    FString object_group_type = "";
    FString object_type       = "";
    bool    is_player_near = false;

public:
    ABase_interaction();

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
      * \param _path 메시 경로 _name 변수 명칭 
     */
    void Init_static_mesh(FString _path, FName _name);

    /**
      * \brief 스켈레탈 메시 컴포넌트 초기화 (캐릭터,차량)
      * \param _path 메시 경로 _name 변수 명칭
     */
    void Init_skeletal_mesh(FString _path, FName _name);

    /**
      * \brief 오디오 컴포넌트 초기화
     */
    void Init_audio();

    /**
      * \brief 상호작용 UI 초기화
     */
    void Init_interaction_UI();
};