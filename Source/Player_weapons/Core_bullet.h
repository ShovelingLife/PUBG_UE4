// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PUBG_UE4/Data_table_manager.h"
#include "Weapon_enum.h"
#include "Core_bullet.generated.h"

UCLASS()
class PLAYER_WEAPONS_API ACore_bullet : public AActor
{
	GENERATED_BODY()
	
protected:
    Fs_weapon_data m_weapon_data;
    e_weapon_type  m_weapon_type;
    const float	   mk_bullet_speed = 2.f;
    const float	   mk_life_time = 3.f;
    float		   m_current_life_time = 0.f;
    float		   m_current_speed = 0.f;

public:
    UPROPERTY(VisibleAnywhere, Category = Scene_comp)
        class USceneComponent* scene_comp;

    UPROPERTY(VisibleAnywhere, Category = Collider)
        class UCapsuleComponent* collider;


    UPROPERTY(VisibleAnywhere, Category = Mesh)
        class UStaticMeshComponent* mesh;

    UPROPERTY(VisibleAnywhere, Category = Bullet)
        class UProjectileMovementComponent* projectile_movement_component;

public:
    // Sets default values for this actor's properties
    ACore_bullet();


protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UFUNCTION()
        void OnHit(class UPrimitiveComponent* _my_comp, AActor* _other, class UPrimitiveComponent* _other_comp, FVector _normal_impulse, const FHitResult& _hit);

public:
    // Called every frame
    virtual void Tick(float) override;

private:
    void Init_mesh();

    void Init_collider();

    void Init_projectile_movement_component();

    void Load_csv();

protected:
    void Init(e_weapon_type);
};
