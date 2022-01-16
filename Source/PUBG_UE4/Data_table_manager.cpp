#include "Data_table_manager.h"

Fs_weapon_data  AData_table_manager::arr_weapon_data[MAX_WEAPON_COUNT];
Fs_vehicle_data AData_table_manager::arr_vehicle_data[MAX_VEHICLE_COUNT];

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

        Fs_weapon_data data                   = *p_row;
        //auto tmp_weapon_data                  = *(arr_weapon_data + i);
        arr_weapon_data[i].type                  = data.type;
        arr_weapon_data[i].mesh_path             = mk_weapon_mesh_path + data.weapon_group_type + "/SK_" + data.mesh_path;
        arr_weapon_data[i].weapon_icon_path      = data.weapon_icon_path;
        arr_weapon_data[i].weapon_slot_icon_path = data.weapon_slot_icon_path;
        arr_weapon_data[i].mag_mesh_path         = mk_weapon_mesh_path + data.type + data.mag_mesh_path;
        arr_weapon_data[i].bullet_mesh_path      = mk_weapon_mesh_path + "Ammunition/SM_Shell_" + data.bullet_mesh_path;
        arr_weapon_data[i].weapon_group_type     = data.weapon_group_type;
        arr_weapon_data[i].collider_size         = data.collider_size;
        arr_weapon_data[i].collider_pos          = data.collider_pos;
        arr_weapon_data[i].max_bullet_count      = data.max_bullet_count;
        arr_weapon_data[i].damage                = data.damage;
        arr_weapon_data[i].current_bullet_count  = data.max_bullet_count;
        arr_weapon_data[i].UI_material_bp_path   = mk_rendertarget_mesh_path + data.type;
        arr_weapon_data[i].bullet_bp_path        = mk_bullet_bp_path + data.type + "_bullet";
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
        auto tmp_vehicle_data               = *(arr_vehicle_data + i);
        tmp_vehicle_data.type               = data.type;
        tmp_vehicle_data.audio_path         = data.audio_path;
        tmp_vehicle_data.mesh_path          = mk_vehicle_mesh_path + data.mesh_path;
        tmp_vehicle_data.anim_instance_path = mk_anim_instance_path + data.type + "/BP_AnimInst_" + data.type;
        tmp_vehicle_data.collider_pos       = data.collider_pos;
        tmp_vehicle_data.collider_size      = data.collider_size;
        tmp_vehicle_data.durability         = data.durability;
        tmp_vehicle_data.max_seater         = data.max_seater;
        tmp_vehicle_data.speed              = data.speed;

        // 좌석 정보 배치
        tmp_vehicle_data.arr_player_seat_pos[0] = { data.seat_1_player_location, data.seat_1_camera_location };
        tmp_vehicle_data.arr_player_seat_pos[1] = { data.seat_2_player_location, data.seat_2_camera_location };
        tmp_vehicle_data.arr_player_seat_pos[2] = { data.seat_3_player_location, data.seat_3_camera_location };
        tmp_vehicle_data.arr_player_seat_pos[3] = { data.seat_4_player_location, data.seat_4_camera_location };
    }
}