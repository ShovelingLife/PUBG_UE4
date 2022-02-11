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
#include "PUBG_UE4/Data_table_manager.h"
#include "Weapon_enum.h"
#include "Core_bullet.generated.h"

class UCapsuleComponent;
class USceneComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class PLAYER_WEAPONS_API ACore_bullet : public AActor
{
	GENERATED_BODY()
	
protected:
    Fs_weapon_data m_weapon_data;
    const float	   mk_bullet_speed     = 2.f;
    const float	   mk_life_time        = 3.f;
    float		   m_current_life_time = 0.f;
    float		   m_current_speed     = 0.f;

public:
    UPROPERTY(VisibleAnywhere, Category = Scene_comp) USceneComponent*              p_scene_comp;
    UPROPERTY(VisibleAnywhere, Category = Collider)   UCapsuleComponent*            p_collider_comp;
    UPROPERTY(VisibleAnywhere, Category = Mesh)       UStaticMeshComponent*         p_mesh_comp;
    UPROPERTY(VisibleAnywhere, Category = Bullet)     UProjectileMovementComponent* p_projectile_movement_comp;

public:
    ACore_bullet();


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
    void Init_mesh();

    /**
      * \brief 충돌체 컴포넌트 초기화
     */
    void Init_collider();

    /**
      * \brief 총알 컴포넌트 초기화
     */
    void Init_projectile_movement_component();

protected:
    /**
      * \brief 총알 오브젝트 초기화
      * \param _index 무기 종류
     */
    void Init(e_weapon_type _index);
};
