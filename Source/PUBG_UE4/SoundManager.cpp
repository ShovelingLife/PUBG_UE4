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
    static ConstructorHelpers::FObjectFinder<USoundWave> EQUIP_SOUND(*(mkSoundPath + "Gun_pickup"));

    if (EQUIP_SOUND.Succeeded())
        pWeaponPickupSound = EQUIP_SOUND.Object;

    // 무기 교체 사운드
    static ConstructorHelpers::FObjectFinder<USoundWave> SWAP_SOUND(*(mkSoundPath + "Gun_swap"));

    if (SWAP_SOUND.Succeeded())
        pWeaponSwapSound = SWAP_SOUND.Object;

    // 플레이어 상태 사운드
    const FString effectSoundPath = "/Game/SFX/EffectSounds/";
    static ConstructorHelpers::FObjectFinder<USoundWave> BLIND_SOUND(*(effectSoundPath + "EarRing"));

    if (BLIND_SOUND.Succeeded())
        arrPlayerStateSound.Add(BLIND_SOUND.Object);
}

void ASoundManager::InitWeaponAudio()
{
    // 총알 비어있는 사운드
    static ConstructorHelpers::FObjectFinder<USoundWave> EMPTY_AMMO_SOUND(*(mkSoundPath + "Empty_shot"));

    if (EMPTY_AMMO_SOUND.Succeeded())
        pEmptyAmmoSound = EMPTY_AMMO_SOUND.Object;

    // 재장전 사운드
    static ConstructorHelpers::FObjectFinder<USoundWave> RELOAD_AMMO_SOUND(*(mkSoundPath + "Gun_reload"));

    if (RELOAD_AMMO_SOUND.Succeeded())
        pReloadSound = RELOAD_AMMO_SOUND.Object;

    // 총알 발사 사운드
    for (int i = 0; i < ADataTableManager::ArrWeaponData.Num(); i++)
    {
        ConstructorHelpers::FObjectFinder<USoundWave> SHOT_SOUND(*(mkSoundPath + ADataTableManager::ArrWeaponData[i].Type + "_shot"));

        if (SHOT_SOUND.Succeeded())
            pArrShotSound.Add(SHOT_SOUND.Object);
    }
    // 투척류 사운드
    const FString k_grenadeSoundPath = "/Game/SFX/ExplosiveSounds/";

    auto claymoreSound = ConstructorHelpers::FObjectFinder<USoundWave>(*(k_grenadeSoundPath + "Claymore"));

    if (claymoreSound.Succeeded())
        pClaymoreSound = claymoreSound.Object;

    auto fragmentationSound = ConstructorHelpers::FObjectFinder<USoundWave>(*(k_grenadeSoundPath + "Fragmentation"));

    if (fragmentationSound.Succeeded())
        pFragmentationSound = fragmentationSound.Object;

    auto illuminationSound = ConstructorHelpers::FObjectFinder<USoundWave>(*(k_grenadeSoundPath + "Illumination"));

    if (illuminationSound.Succeeded())
        pIlluminationSound = illuminationSound.Object;

    auto molotovSound = ConstructorHelpers::FObjectFinder<USoundWave>(*(k_grenadeSoundPath + "Molotov"));

    if (molotovSound.Succeeded())
        pMolotovSound = molotovSound.Object;

    auto smokeSound = ConstructorHelpers::FObjectFinder<USoundWave>(*(k_grenadeSoundPath + "Smoke"));

    if (smokeSound.Succeeded())
        pSmokeSound = smokeSound.Object;

    auto stickSound = ConstructorHelpers::FObjectFinder<USoundWave>(*(k_grenadeSoundPath + "Stick"));

    if (stickSound.Succeeded())
        pStickSound = stickSound.Object;
    
}

void ASoundManager::PlayPlayerSound(UAudioComponent* AudioComp, EPlayerSoundType SoundType)
{
    USoundBase* tmpSound = nullptr;

    switch(SoundType)
    {
    case EPlayerSoundType::ITEM_FARM:    tmpSound = pItemFarmSound;     break;
    case EPlayerSoundType::WEAPON_EQUIP: tmpSound = pWeaponPickupSound; break;
    case EPlayerSoundType::WEAPON_SWAP:  tmpSound = pWeaponSwapSound;   break;
    }
    if (AudioComp &&
        tmpSound)
    {
        AudioComp->SetSound(tmpSound);
        AudioComp->Play();
    }
}

void ASoundManager::PlayPlayerEffectSound(int Index)
{
    if (Index >= arrPlayerStateSound.Num())
        return;

    UGameplayStatics::PlaySound2D(GetWorld(), arrPlayerStateSound[Index]);
}

void ASoundManager::PlayGunSound(UAudioComponent* AudioComp, EWeaponSoundType SoundType, int WeaponIndex)
{
    USoundBase* tmpSound = nullptr;

    switch (SoundType)
    {
    case EWeaponSoundType::EMPTY_AMMO: tmpSound = pEmptyAmmoSound;             break;
    case EWeaponSoundType::SHOT:       tmpSound = pArrShotSound[WeaponIndex]; break;
    case EWeaponSoundType::RELOAD:     tmpSound = pReloadSound;                break;
    }
    if (AudioComp &&
        tmpSound)
    {
        AudioComp->SetSound(tmpSound);
        AudioComp->Play();
    }
}

USoundBase* ASoundManager::GetExplosiveSoundBase(int Type)
{
    switch (Type)
    {
    case 0: case 1: return pFragmentationSound;
    case 3: case 4: return pSmokeSound;
    case 2: return pIlluminationSound;
    case 5: return pStickSound;
    case 6: return pMolotovSound;
    case 7: return pClaymoreSound;
    }
    return nullptr;
}
