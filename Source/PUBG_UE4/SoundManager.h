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
#include "MyEnum.h"
#include "SoundManager.generated.h"

class USoundBase;
class UAudioComponent;

UCLASS()
class PUBG_UE4_API ASoundManager : public AActor
{
	GENERATED_BODY()

private:
    const FString     mkSoundPath = "/Game/SFX/Gun_sounds/";

public:
    /**
      * \brief 무기 관련 사운드
     */
    UPROPERTY(VisibleAnywhere, Category = Weapon_sound) TArray<USoundBase*> pArrShotSound;
    UPROPERTY(VisibleAnywhere, Category = Weapon_sound) USoundBase*         pEmptyAmmoSound = nullptr;
    UPROPERTY(VisibleAnywhere, Category = Player_sound) USoundBase*         pReloadSound    = nullptr;

    /**
      * \brief 플레이어 무기 사운드 관련
     */
    UPROPERTY(VisibleAnywhere, Category = Player_sound) USoundBase* pItemFarmSound     = nullptr; 
    UPROPERTY(VisibleAnywhere, Category = Player_sound) USoundBase* pWeaponPickupSound = nullptr;
    UPROPERTY(VisibleAnywhere, Category = Player_sound) USoundBase* pWeaponSwapSound   = nullptr;

private:
    /**
      * \brief 무기 오디오 초기화
     */
    void InitWeaponAudio();

    /**
      * \brief 플레이어 오디오 초기화
     */
    void InitPlayerAudio();

public:	
	ASoundManager();

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

public:
    /**
      * \brief 무기 오디오 재생
      * \param _p_audio_comp 오디오 컴포넌트 \param _sound_type 사운드 종류 \param 사운드 인덱스
     */
    void PlayWeaponSound(UAudioComponent* _p_audio_comp, EWeaponSoundType _sound_type, int _weapon_index = 0);

    /**
      * \brief 플레이어 오디오 재생
      * \param _p_audio_comp 오디오 컴포넌트 \param _sound_type 사운드 종류
     */
    void PlayPlayerSound(UAudioComponent* _p_audio_comp, EPlayerSoundType _sound_type);
};