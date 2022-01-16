#include "Sound_manager.h"
#include "Components/AudioComponent.h"
#include "Editor/EditorEngine.h"
#include "LevelEditor.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

// Sets default values
ASound_manager::ASound_manager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    Init_weapon_audio();
}

// Called when the game starts or when spawned
void ASound_manager::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ASound_manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASound_manager::Play_weapon_sound(UAudioComponent* _p_audio_comp, e_weapon_sound_type _sound_type, int _weapon_index)
{
    USoundBase* tmp_sound = nullptr;

    switch (_sound_type)
    {
    case e_weapon_sound_type::EMPTY_AMMO:   tmp_sound = p_empty_ammo_sound;              break;
    case e_weapon_sound_type::SHOT_SOUND:   tmp_sound = p_shot_sound_arr[_weapon_index]; break;
    case e_weapon_sound_type::RELOAD_SOUND: tmp_sound = p_reload_sound;                  break;
    case e_weapon_sound_type::EQUIP_SOUND:  tmp_sound = p_equip_sound;                   break;
    }
    _p_audio_comp->SetSound(tmp_sound);
    _p_audio_comp->Play();
}

void ASound_manager::Init_weapon_audio()
{
    // 사운드 적용
    static ConstructorHelpers::FObjectFinder<USoundWave> BP_EMPTY_AMMO_SOUND(*(mk_sound_path + "Empty_shot"));

    if (BP_EMPTY_AMMO_SOUND.Succeeded())
        p_empty_ammo_sound = Cast<USoundBase>(BP_EMPTY_AMMO_SOUND.Object);

    static ConstructorHelpers::FObjectFinder<USoundWave> BP_RELOAD_AMMO_SOUND(*(mk_sound_path + "Gun_reload"));

    if (BP_RELOAD_AMMO_SOUND.Succeeded())
        p_reload_sound = Cast<USoundBase>(BP_RELOAD_AMMO_SOUND.Object);

    static ConstructorHelpers::FObjectFinder<USoundWave> BP_SWAP_SOUND(*(mk_sound_path + "Gun_swap"));

    if (BP_SWAP_SOUND.Succeeded())
        p_equip_sound = Cast<USoundBase>(BP_SWAP_SOUND.Object);
    
    for (int i = 0; i < MAX_WEAPON_COUNT; i++)
    {
        ConstructorHelpers::FObjectFinder<USoundWave> BP_SHOT_SOUND(*(mk_sound_path + AData_table_manager::arr_weapon_data[i].type + "_shot"));

        if (BP_SHOT_SOUND.Succeeded())
            p_shot_sound_arr[i] = Cast<USoundBase>(BP_SHOT_SOUND.Object);
    }
}