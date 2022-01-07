#include "Global.h"
#include "Custom_game_instance.h"
#include "Data_table_manager.h"
#include "UI_manager.h"
#include "Sound_manager.h"

AGlobal* AGlobal::mp_global;

AGlobal::AGlobal()
{
    PrimaryActorTick.bCanEverTick = true;
    Init();
    Init_game_instance();
    Init_data_table_manager();
    Init_UI_manager();
    Init_sound_manager();
}

void AGlobal::BeginPlay()
{
    Super::BeginPlay();
    Spawn_managers();
}

void AGlobal::Tick(float _delta_time)
{
    Super::Tick(_delta_time);
}

void AGlobal::Spawn_managers()
{
    // UI 매니저
    mp_UI_manager = GetWorld()->SpawnActor<AUI_manager>(AUI_manager::StaticClass(), FTransform::Identity);

    // 사운드 매니저
    mp_sound_manager = GetWorld()->SpawnActor<ASound_manager>(ASound_manager::StaticClass(), FTransform::Identity);
}

void AGlobal::Init()
{
    // 포인터 초기화
    ConstructorHelpers::FClassFinder<AActor> GLOBAL_BP(TEXT("Blueprint'/Game/Blueprints/Managers/BP_Global.BP_Global_C'"));

    if (GLOBAL_BP.Succeeded())
        AGlobal::mp_global = Cast<AGlobal>(GLOBAL_BP.Class->GetDefaultObject());

    player_spring_arm_rotation = FRotator(-20.f, 0.f, 0.f);
    player_spring_arm_location = FVector(0.f, 0.f, 80.f);
    player_spring_arm_length   = 150.f;
}

void AGlobal::Init_data_table_manager()
{
    // 데이터테이블 초기화
    ConstructorHelpers::FClassFinder<AActor> DATA_TABLE_MANAGER_BP(TEXT("Blueprint'/Game/Blueprints/Managers/BP_Data_table_manager.BP_Data_table_manager_C'"));

    if (DATA_TABLE_MANAGER_BP.Succeeded())
        mp_data_table_manager = Cast<AData_table_manager>(DATA_TABLE_MANAGER_BP.Class->GetDefaultObject());
}

void AGlobal::Init_UI_manager()
{
    // UI 매니저 초기화
    ConstructorHelpers::FClassFinder<AActor> UI_MANAGER_BP(TEXT("Blueprint'/Game/Blueprints/Managers/BP_UI_manager.BP_UI_manager_C'"));

    if (UI_MANAGER_BP.Succeeded())
        mp_UI_manager = Cast<AUI_manager>(UI_MANAGER_BP.Class->GetDefaultObject());
}

void AGlobal::Init_sound_manager()
{
    // 사운드 매니저 초기화
    ConstructorHelpers::FClassFinder<AActor> SOUND_MANAGER_BP(TEXT("Blueprint'/Game/Blueprints/Managers/BP_Sound_manager.BP_Sound_manager_C'"));

    if (SOUND_MANAGER_BP.Succeeded())
        mp_sound_manager = Cast<ASound_manager>(SOUND_MANAGER_BP.Class->GetDefaultObject());
}

void AGlobal::Init_game_instance()
{
    // 데이터테이블 초기화
    ConstructorHelpers::FClassFinder<UCustom_game_instance> GAME_INSTANCE_BP(TEXT("Blueprint'/Game/Blueprints/Managers/BP_Custom_game_instance.BP_Custom_game_instance_C'"));

    if (GAME_INSTANCE_BP.Succeeded())
        mp_game_instance = Cast<UCustom_game_instance>(GAME_INSTANCE_BP.Class->GetDefaultObject());
}

AData_table_manager* AGlobal::Get_data_table_manager()
{
    if (mp_global &&
        !mp_global->mp_data_table_manager)
        mp_global->Init_data_table_manager();

    return mp_global->mp_data_table_manager;
}

UCustom_game_instance* AGlobal::Get_custom_game_inst()
{
    if (mp_global &&
        !mp_global->mp_game_instance)
        mp_global->Init_game_instance();

    return mp_global->mp_game_instance;
}

AUI_manager* AGlobal::Get_UI_manager()
{
    if (mp_global &&
        !mp_global->mp_UI_manager)
        mp_global->Init_UI_manager();

    return mp_global->mp_UI_manager;
}

ASound_manager* AGlobal::Get_sound_manager()
{
    if (mp_global&&
        !mp_global->mp_sound_manager)
        mp_global->Init_sound_manager();

    return mp_global->mp_sound_manager;
}