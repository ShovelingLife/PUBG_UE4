// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data_table_manager.h"
#include "Sound_manager.generated.h"

UCLASS()
class PUBG_UE4_API ASound_manager : public AActor
{
	GENERATED_BODY()

private:
    const FString     mk_sound_path = "/Game/SFX/Gun_sounds/";

public:
    // 무기 관련 사운드
    UPROPERTY(VisibleAnywhere, Category = Sound)
        USoundBase* p_shot_sound_arr[MAX_WEAPON_COUNT];

    UPROPERTY(VisibleAnywhere, Category = Sound)
        USoundBase* p_empty_ammo_sound = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Sound)
        USoundBase* p_reload_sound = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Sound)
        USoundBase* p_equip_sound = nullptr;

private:
    void Init_weapon_audio();

public:	
	// Sets default values for this actor's properties
	ASound_manager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
