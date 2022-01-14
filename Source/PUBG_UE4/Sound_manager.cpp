// Fill out your copyright notice in the Description page of Project Settings.


#include "Sound_manager.h"
#include "Editor/EditorEngine.h"
#include "LevelEditor.h"
#include "Kismet/GameplayStatics.h"

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

    // csv 매니저 초기화
    ConstructorHelpers::FClassFinder<AActor> DATA_TABLE_MANAGER(TEXT("Blueprint'/Game/Blueprints/Managers/BP_Data_table_manager.BP_Data_table_manager_C'"));
    AData_table_manager* p_data_table_manager = nullptr;

    if (DATA_TABLE_MANAGER.Succeeded())
        p_data_table_manager = Cast<AData_table_manager>(DATA_TABLE_MANAGER.Class->GetDefaultObject());
    
    if (!p_data_table_manager)
        return;

    // CSV로부터 오디오 경로 가져오기
    for (int i = 0; i < MAX_WEAPON_COUNT; i++)
    {
        ConstructorHelpers::FObjectFinder<USoundWave> BP_SHOT_SOUND(*(mk_sound_path + p_data_table_manager->Get_weapon_data(i).type + "_shot"));

        if (BP_SHOT_SOUND.Succeeded())
            p_shot_sound_arr[i] = Cast<USoundBase>(BP_SHOT_SOUND.Object);
    }
}