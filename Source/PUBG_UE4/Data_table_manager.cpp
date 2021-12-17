// Fill out your copyright notice in the Description page of Project Settings.


#include "Data_table_manager.h"

// Sets default values
AData_table_manager::AData_table_manager()
{
    Init_weapon_data();
    Init_vehicle_data();
}

// Called when the game starts or when spawned
void AData_table_manager::BeginPlay()
{
    Super::BeginPlay();
}

void AData_table_manager::Init_weapon_data()
{
    // CSV 로드
    ConstructorHelpers::FObjectFinder<UDataTable> WEAPON_DATA_TABLE(TEXT("/Game/Data/WEAPON_DATA_TABLE"));

    if (WEAPON_DATA_TABLE.Succeeded())
        mp_weapon_data_table = WEAPON_DATA_TABLE.Object;

    if (!mp_weapon_data_table)
        return;

    // 모든 이름 가져오기
    TArray<FName> row_name_arr = mp_weapon_data_table->GetRowNames();
    m_current_weapon_count = row_name_arr.Num();

    // 갖고온 CSV로부터 데이터 할당
    for (int i = 0; i < m_current_weapon_count; i++)
    {
        // row_name_arr 안에 정보 및 명칭
        auto p_row = mp_weapon_data_table->FindRow<Fs_weapon_data>(row_name_arr[i], row_name_arr[i].ToString());

        if (!p_row)
            break;

        Fs_weapon_data data = *p_row;
        m_weapon_data_arr[i].type                     = data.type;
        m_weapon_data_arr[i].mesh_path                = mk_weapon_mesh_path + data.mesh_path;
        m_weapon_data_arr[i].audio_path               = data.audio_path;
        m_weapon_data_arr[i].mag_mesh_path            = mk_weapon_mesh_path + data.type + data.mag_mesh_path;
        m_weapon_data_arr[i].bullet_mesh_path         = mk_weapon_mesh_path + "Ammunition/SM_Shell_" + data.bullet_mesh_path;
        m_weapon_data_arr[i].collider_size            = data.collider_size;
        m_weapon_data_arr[i].collider_pos             = data.collider_pos;
        m_weapon_data_arr[i].current_ammunition_count = data.current_ammunition_count;
        m_weapon_data_arr[i].max_bullet_count         = data.max_bullet_count;
        m_weapon_data_arr[i].damage                   = data.damage;
        m_weapon_data_arr[i].current_bullet_count     = data.max_bullet_count;
        m_weapon_data_arr[i].UI_material_bp_path      = mk_rendertarget_mesh_path + data.type;
        m_weapon_data_arr[i].bullet_bp_path           = mk_bullet_bp_path + data.type + "_bullet";
    }
}

void AData_table_manager::Init_vehicle_data()
{
    // CSV 로드
    ConstructorHelpers::FObjectFinder<UDataTable> VEHICLE_DATA_TABLE(TEXT("/Game/Data/VEHICLE_DATA_TABLE"));

    if (VEHICLE_DATA_TABLE.Succeeded())
        mp_vehicle_data_table = VEHICLE_DATA_TABLE.Object;

    if (!mp_vehicle_data_table)
        return;

    // 모든 이름 가져오기
    TArray<FName> row_name_arr = mp_vehicle_data_table->GetRowNames();
    m_current_vehicle_count    = row_name_arr.Num();

    // 갖고온 CSV로부터 데이터 할당
    for (int i = 0; i < m_current_vehicle_count; i++)
    {
        // row_name_arr 안에 정보 및 명칭
        auto p_row = mp_vehicle_data_table->FindRow<Fs_vehicle_data>(row_name_arr[i], row_name_arr[i].ToString());

        if (!p_row)
            break;

        Fs_vehicle_data data                = *p_row;
        m_vehicle_data_arr[i].audio_path    = data.audio_path;
        m_vehicle_data_arr[i].mesh_path     = mk_vehicle_mesh_path + data.mesh_path;
        m_vehicle_data_arr[i].collider_pos  = data.collider_pos;
        m_vehicle_data_arr[i].collider_size = data.collider_size;
        m_vehicle_data_arr[i].durability    = data.durability;
        m_vehicle_data_arr[i].max_seater    = data.max_seater;
        m_vehicle_data_arr[i].speed         = data.speed;
    }
}