/**
 * \file Sound_manager.h
 *
 * \brief 사운드 매니저
 *
 * \ingroup PUBG_UE4
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data_table_manager.h"
#include "My_enum.h"
#include "Sound_manager.generated.h"

class USoundBase;
class UAudioComponent;

UCLASS()
class PUBG_UE4_API ASound_manager : public AActor
{
	GENERATED_BODY()

private:
    const FString     mk_sound_path = "/Game/SFX/Gun_sounds/";

public:
    /**
      * \brief 무기 관련 사운드
     */
    UPROPERTY(VisibleAnywhere, Category = Weapon_sound) TArray<USoundBase*> p_arr_shot_sound;
    UPROPERTY(VisibleAnywhere, Category = Weapon_sound) USoundBase*         p_empty_ammo_sound = nullptr;
    UPROPERTY(VisibleAnywhere, Category = Player_sound) USoundBase*         p_reload_sound = nullptr;

    /**
      * \brief 플레이어 무기 사운드 관련
     */
    UPROPERTY(VisibleAnywhere, Category = Player_sound) USoundBase* p_item_farm_sound = nullptr; 
    UPROPERTY(VisibleAnywhere, Category = Player_sound) USoundBase* p_weapon_pickup_sound = nullptr;
    UPROPERTY(VisibleAnywhere, Category = Player_sound) USoundBase* p_weapon_swap_sound = nullptr;

private:
    /**
      * \brief 무기 오디오 초기화
     */
    void Init_weapon_audio();

    /**
      * \brief 플레이어 오디오 초기화
     */
    void Init_player_audio();

public:	
	ASound_manager();

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

public:
    /**
      * \brief 무기 오디오 재생
      * \param _p_audio_comp 오디오 컴포넌트 \param _sound_type 사운드 종류 \param 사운드 인덱스
     */
    void Play_weapon_sound(UAudioComponent* _p_audio_comp, e_weapon_sound_type _sound_type, int _weapon_index = 0);

    /**
      * \brief 플레이어 오디오 재생
      * \param _p_audio_comp 오디오 컴포넌트 \param _sound_type 사운드 종류
     */
    void Play_player_sound(UAudioComponent* _p_audio_comp, e_player_sound_type _sound_type);
};