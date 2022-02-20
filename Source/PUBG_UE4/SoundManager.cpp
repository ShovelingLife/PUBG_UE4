#include "SoundManager.h"
#include "DataTableManager.h"
#include "Components/AudioComponent.h"
#include "Editor/EditorEngine.h"
#include "LevelEditor.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

ASoundManager::ASoundManager()
{
    PrimaryActorTick.bCanEverTick = true;
    InitPlayerAudio();
    InitWeaponAudio();
}

void ASoundManager::BeginPlay()
{
    Super::BeginPlay();
}

void ASoundManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoundManager::InitPlayerAudio()
{
    // 무기 장착 사운드
    static ConstructorHelpers::FObjectFinder<USoundWave> BP_EQUIP_SOUND(*(mkSoundPath + "Gun_pickup"));

    if (BP_EQUIP_SOUND.Succeeded())
        pWeaponPickupSound = Cast<USoundBase>(BP_EQUIP_SOUND.Object);

    // 무기 교체 사운드
    static ConstructorHelpers::FObjectFinder<USoundWave> BP_SWAP_SOUND(*(mkSoundPath + "Gun_swap"));

    if (BP_SWAP_SOUND.Succeeded())
        pWeaponSwapSound = Cast<USoundBase>(BP_SWAP_SOUND.Object);
}

void ASoundManager::InitWeaponAudio()
{
    // 총알 비어있는 사운드
    static ConstructorHelpers::FObjectFinder<USoundWave> BP_EMPTY_AMMO_SOUND(*(mkSoundPath + "Empty_shot"));

    if (BP_EMPTY_AMMO_SOUND.Succeeded())
        pEmptyAmmoSound = Cast<USoundBase>(BP_EMPTY_AMMO_SOUND.Object);

    // 재장전 사운드
    static ConstructorHelpers::FObjectFinder<USoundWave> BP_RELOAD_AMMO_SOUND(*(mkSoundPath + "Gun_reload"));

    if (BP_RELOAD_AMMO_SOUND.Succeeded())
        pReloadSound = Cast<USoundBase>(BP_RELOAD_AMMO_SOUND.Object);

    // 총알 발사 사운드
    for (int i = 0; i < ADataTableManager::ArrWeaponData.Num(); i++)
    {
        ConstructorHelpers::FObjectFinder<USoundWave> BP_SHOT_SOUND(*(mkSoundPath + ADataTableManager::ArrWeaponData[i].Type + "_shot"));

        if (BP_SHOT_SOUND.Succeeded())
            pArrShotSound.Add(Cast<USoundBase>(BP_SHOT_SOUND.Object));
    }
}

void ASoundManager::PlayWeaponSound(UAudioComponent* _pAudioComp, EWeaponSoundType _SoundType, int _WeaponIndex)
{
    USoundBase* tmp_sound = nullptr;

    if (!_pAudioComp)
        return;

    switch (_SoundType)
    {
    case EWeaponSoundType::EMPTY_AMMO: tmp_sound = pEmptyAmmoSound;              break;
    case EWeaponSoundType::SHOT:       tmp_sound = pArrShotSound[_WeaponIndex]; break;
    case EWeaponSoundType::RELOAD:     tmp_sound = pReloadSound;                  break;
    }
    _pAudioComp->SetSound(tmp_sound);

    if (_pAudioComp->Sound)
        _pAudioComp->Play();
}

void ASoundManager::PlayPlayerSound(UAudioComponent* _pAudioComp, EPlayerSoundType _SoundType)
{
    USoundBase* tmp_sound = nullptr;

    if (!_pAudioComp)
        return;

    switch(_SoundType)
    {
    case EPlayerSoundType::ITEM_FARM:    tmp_sound = pItemFarmSound;     break;
    case EPlayerSoundType::WEAPON_EQUIP: tmp_sound = pWeaponPickupSound; break;
    case EPlayerSoundType::WEAPON_SWAP:  tmp_sound = pWeaponSwapSound;   break;
    }
    _pAudioComp->SetSound(tmp_sound);
    
    if (_pAudioComp->Sound)
        _pAudioComp->Play();
}