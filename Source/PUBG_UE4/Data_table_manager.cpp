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
    // CSV �ε�
    ConstructorHelpers::FObjectFinder<UDataTable> WEAPON_DATA_TABLE(TEXT("/Game/Data/WEAPON_DATA_TABLE"));

    if (WEAPON_DATA_TABLE.Succeeded())
        mp_weapon_data_table = WEAPON_DATA_TABLE.Object;

    if (!mp_weapon_data_table)
        return;

    // ��� �̸� ��������
    TArray<FName> row_name_arr = mp_weapon_data_table->GetRowNames();
    m_current_weapon_count = row_name_arr.Num();
    
    // ����� CSV�κ��� ������ �Ҵ�
    for (int i = 0; i < m_current_weapon_count; i++)
    {
        // row_name_arr �ȿ� ���� �� ��Ī
        auto p_row = mp_weapon_data_table->FindRow<Fs_weapon_data>(row_name_arr[i], row_name_arr[i].ToString());

        if (!p_row)
            break;

        Fs_weapon_data data = *p_row;
        m_arr_weapon_data[i].type                     = data.type;
        m_arr_weapon_data[i].mesh_path                = mk_weapon_mesh_path + data.mesh_path;
        m_arr_weapon_data[i].audio_path               = data.audio_path;
        m_arr_weapon_data[i].mag_mesh_path            = mk_weapon_mesh_path + data.type + data.mag_mesh_path;
        m_arr_weapon_data[i].bullet_mesh_path         = mk_weapon_mesh_path + "Ammunition/SM_Shell_" + data.bullet_mesh_path;
        m_arr_weapon_data[i].collider_size            = data.collider_size;
        m_arr_weapon_data[i].collider_pos             = data.collider_pos;
        m_arr_weapon_data[i].current_ammunition_count = data.current_ammunition_count;
        m_arr_weapon_data[i].max_bullet_count         = data.max_bullet_count;
        m_arr_weapon_data[i].damage                   = data.damage;
        m_arr_weapon_data[i].current_bullet_count     = data.max_bullet_count;
        m_arr_weapon_data[i].UI_material_bp_path      = mk_rendertarget_mesh_path + data.type;
        m_arr_weapon_data[i].bullet_bp_path           = mk_bullet_bp_path + data.type + "_bullet";
    }
}

void AData_table_manager::Init_vehicle_data()
{
    // CSV �ε�
    ConstructorHelpers::FObjectFinder<UDataTable> VEHICLE_DATA_TABLE(TEXT("/Game/Data/VEHICLE_DATA_TABLE"));

    if (VEHICLE_DATA_TABLE.Succeeded())
        mp_vehicle_data_table = VEHICLE_DATA_TABLE.Object;

    if (!mp_vehicle_data_table)
        return;

    // ��� �̸� ��������
    TArray<FName> row_name_arr = mp_vehicle_data_table->GetRowNames();
    m_current_vehicle_count    = row_name_arr.Num();

    // ����� CSV�κ��� ������ �Ҵ�
    for (int i = 0; i < m_current_vehicle_count; i++)
    {
        // row_name_arr �ȿ� ���� �� ��Ī
        auto p_row = mp_vehicle_data_table->FindRow<Fs_vehicle_data>(row_name_arr[i], row_name_arr[i].ToString());

        if (!p_row)
            break;

        Fs_vehicle_data data                = *p_row;
        m_arr_vehicle_data[i].type          = data.type;
        m_arr_vehicle_data[i].audio_path    = data.audio_path;
        m_arr_vehicle_data[i].mesh_path     = mk_vehicle_mesh_path + data.mesh_path;
        m_arr_vehicle_data[i].collider_pos  = data.collider_pos;
        m_arr_vehicle_data[i].collider_size = data.collider_size;
        m_arr_vehicle_data[i].durability    = data.durability;
        m_arr_vehicle_data[i].max_seater    = data.max_seater;
        m_arr_vehicle_data[i].speed         = data.speed;

        // �¼� ���� ��ġ
        m_arr_vehicle_data[i].arr_player_seat_pos[0].first  = data.seat_1_player_location;
        m_arr_vehicle_data[i].arr_player_seat_pos[0].second = data.seat_1_camera_location;
        m_arr_vehicle_data[i].arr_player_seat_pos[1].first  = data.seat_2_player_location;
        m_arr_vehicle_data[i].arr_player_seat_pos[1].second = data.seat_2_camera_location;
        m_arr_vehicle_data[i].arr_player_seat_pos[2].first  = data.seat_3_player_location;
        m_arr_vehicle_data[i].arr_player_seat_pos[2].second = data.seat_3_camera_location;
        m_arr_vehicle_data[i].arr_player_seat_pos[3].first  = data.seat_4_player_location;
        m_arr_vehicle_data[i].arr_player_seat_pos[3].second = data.seat_4_camera_location;
        //m_arr_vehicle_data[i].arr_player_seat_pos[0].Add(data.seat_1_camera_location, data.seat_1_player_location);
        //m_arr_vehicle_data[i].arr_player_seat_pos[1].Add(data.seat_2_camera_location, data.seat_2_player_location);
        //m_arr_vehicle_data[i].arr_player_seat_pos[2].Add(data.seat_3_camera_location, data.seat_3_player_location);
        //m_arr_vehicle_data[i].arr_player_seat_pos[3].Add(data.seat_4_camera_location, data.seat_4_player_location);
    }
}