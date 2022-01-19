// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon_enum.h"
#include "PUBG_UE4/Base_interaction.h"
#include "PUBG_UE4/Other_weapon_data.h"
#include "Core_throwable_weapon.generated.h"

UCLASS()
class PLAYER_WEAPONS_API ACore_throwable_weapon : public ABase_interaction
{
	GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, Category = Particle)
        class UParticleSystemComponent* p_grenade_particle = nullptr;

public:
	Fs_other_weapon_data    weapon_data;
	e_throwable_weapon_type weapon_type;

public:
    ACore_throwable_weapon();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float) override;

protected:
    void Init(e_throwable_weapon_type);

    // 메쉬 초기화
    void Init_mesh();

    // 콜라이더 정보 갱신
    void Update_collider();

    void Init_particle_system();
};
