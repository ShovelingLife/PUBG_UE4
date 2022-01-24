#include "Sound_manager.h"
#include "Data_table_manager.h"
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
    Init_player_audio();
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

void ASound_manager::Init_player_audio()
{
    // 무기 장착 사운드
    static ConstructorHelpers::FObjectFinder<USoundWave> BP_EQUIP_SOUND(*(mk_sound_path + "Gun_pickup"));

    if (BP_EQUIP_SOUND.Succeeded())
        p_weapon_pickup_sound = Cast<USoundBase>(BP_EQUIP_SOUND.Object);

    // 무기 교체 사운드
    static ConstructorHelpers::FObjectFinder<USoundWave> BP_SWAP_SOUND(*(mk_sound_path + "Gun_swap"));

    if (BP_SWAP_SOUND.Succeeded())
        p_weapon_swap_sound = Cast<USoundBase>(BP_SWAP_SOUND.Object);
}

void ASound_manager::Init_weapon_audio()
{
    // 총알 비어있는 사운드
    static ConstructorHelpers::FObjectFinder<USoundWave> BP_EMPTY_AMMO_SOUND(*(mk_sound_path + "Empty_shot"));

    if (BP_EMPTY_AMMO_SOUND.Succeeded())
        p_empty_ammo_sound = Cast<USoundBase>(BP_EMPTY_AMMO_SOUND.Object);

    // 재장전 사운드
    static ConstructorHelpers::FObjectFinder<USoundWave> BP_RELOAD_AMMO_SOUND(*(mk_sound_path + "Gun_reload"));

    if (BP_RELOAD_AMMO_SOUND.Succeeded())
        p_reload_sound = Cast<USoundBase>(BP_RELOAD_AMMO_SOUND.Object);

    // 총알 발사 사운드
    for (int i = 0; i < AData_table_manager::arr_weapon_data.Num(); i++)
    {
        ConstructorHelpers::FObjectFinder<USoundWave> BP_SHOT_SOUND(*(mk_sound_path + AData_table_manager::arr_weapon_data[i].type + "_shot"));

        if (BP_SHOT_SOUND.Succeeded())
            p_arr_shot_sound.Add(Cast<USoundBase>(BP_SHOT_SOUND.Object));
    }
}

void ASound_manager::Play_weapon_sound(UAudioComponent* _p_audio_comp, e_weapon_sound_type _sound_type, int _weapon_index)
{
    USoundBase* tmp_sound = nullptr;

    switch (_sound_type)
    {
    case e_weapon_sound_type::EMPTY_AMMO: tmp_sound = p_empty_ammo_sound;              break;
    case e_weapon_sound_type::SHOT:       tmp_sound = p_arr_shot_sound[_weapon_index]; break;
    case e_weapon_sound_type::RELOAD:     tmp_sound = p_reload_sound;                  break;
    }
    _p_audio_comp->SetSound(tmp_sound);

    if (_p_audio_comp->Sound)
        _p_audio_comp->Play();
}

void ASound_manager::Play_player_sound(UAudioComponent* _p_audio_comp, e_player_sound_type _sound_type)
{
    USoundBase* tmp_sound = nullptr;

    switch(_sound_type)
    {
    case e_player_sound_type::ITEM_FARM:    tmp_sound = p_item_farm_sound;     break;
    case e_player_sound_type::WEAPON_EQUIP: tmp_sound = p_weapon_pickup_sound; break;
    case e_player_sound_type::WEAPON_SWAP:  tmp_sound = p_weapon_swap_sound;   break;
    }
    _p_audio_comp->SetSound(tmp_sound);
    
    if (_p_audio_comp->Sound)
        _p_audio_comp->Play();
}