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
    const FString mkSoundPath = "/Game/SFX/Gun_sounds/";

public:
    /** \brief 무기 관련 사운드 */
    UPROPERTY(VisibleAnywhere, Category = WeaponSound) TArray<USoundBase*> pArrShotSound;
    UPROPERTY(VisibleAnywhere, Category = WeaponSound) USoundBase*         pEmptyAmmoSound = nullptr;
    UPROPERTY(VisibleAnywhere, Category = WeaponSound) USoundBase*         pReloadSound    = nullptr;

    UPROPERTY(VisibleAnywhere, Category = GrenadeSound) USoundBase* pClaymoreSound = nullptr;
    UPROPERTY(VisibleAnywhere, Category = GrenadeSound) USoundBase* pFragmentationSound = nullptr;
    UPROPERTY(VisibleAnywhere, Category = GrenadeSound) USoundBase* pIlluminationSound = nullptr;
    UPROPERTY(VisibleAnywhere, Category = GrenadeSound) USoundBase* pMolotovSound = nullptr;
    UPROPERTY(VisibleAnywhere, Category = GrenadeSound) USoundBase* pSmokeSound = nullptr;
    UPROPERTY(VisibleAnywhere, Category = GrenadeSound) USoundBase* pStickSound = nullptr;

    /** \brief 플레이어 사운드 관련 */
    UPROPERTY(VisibleAnywhere, Category = PlayerSound) USoundBase* pItemFarmSound     = nullptr; 
    UPROPERTY(VisibleAnywhere, Category = PlayerSound) USoundBase* pWeaponPickupSound = nullptr;
    UPROPERTY(VisibleAnywhere, Category = PlayerSound) USoundBase* pWeaponSwapSound   = nullptr;

    TArray<USoundBase*> arrPlayerStateSound;

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
    /** * \brief 플레이어 오디오 재생
      * \param AudioComp 오디오 컴포넌트 
      * \param SoundType  사운드 종류
      */
    void PlayPlayerSound(UAudioComponent* AudioComp, EPlayerSoundType SoundType);

    void PlayPlayerEffectSound(int);

    /**
      * \brief 무기 오디오 재생
      * \param 
      * \param 
      * \param 
      */
    void PlayGunSound(FVector Location, EWeaponSoundType SoundType, int WeaponIndex = 0);

    USoundBase* GetExplosiveSoundBase(int) const;
};