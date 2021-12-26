// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core_farmable_item.h"
#include "Crate_box.generated.h"

/**
 * 
 */
UCLASS()
class FARMABLE_ITEMS_API ACrate_box : public ACore_farmable_item
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category = Mesh)
        class UStaticMeshComponent* mp_parachute_mesh = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Mesh)
		class UParticleSystemComponent* mp_particle_component = nullptr;

	float m_current_time			 = 0.f;
    const float mk_max_particle_time = 15.f;
    const float mk_max_spawn_time	 = 30.f;
	bool  m_is_landed				 = false;
	bool  m_is_particle_ended		 = false;

public:
	ACrate_box();

protected:
    // Called every frame
    virtual void Tick(float) override;

private:
	// 이펙트 초기화
	void Init_vfx();

	// 메쉬 초기화
	void Init_meshes();

	// 콜라이더 업데이트
	void Update_collider();

public:
	UFUNCTION()
	void Turn_off_after_landing(class UPrimitiveComponent* HitComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};