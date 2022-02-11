#include "Data_table_manager.h"

TArray<Fs_weapon_data>       AData_table_manager::arr_weapon_data;
TArray<Fs_other_weapon_data> AData_table_manager::arr_other_weapon_data;
TArray<Fs_vehicle_data>      AData_table_manager::arr_vehicle_data;

// Sets default values
AData_table_manager::AData_table_manager()
{
    Init_weapon_data();
    Init_other_weapon_data();
    Init_vehicle_data();
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
    TArray<FName> arr_row_name = mp_weapon_data_table->GetRowNames();
    
    // 갖고온 CSV로부터 데이터 할당
    for (int i = 0; i < arr_row_name.Num(); i++)
    {
        // row_name_arr 안에 정보 및 명칭
        auto p_row = mp_weapon_data_table->FindRow<Fs_weapon_data>(arr_row_name[i], arr_row_name[i].ToString());

        if (!p_row)
            break;

        Fs_weapon_data data      = *p_row;
        data.mesh_path           = mk_weapon_mesh_path + data.weapon_group_type + "/SK_" + data.mesh_path;
        data.mag_mesh_path       = mk_weapon_mesh_path + data.type + data.mag_mesh_path;
        data.bullet_mesh_path    = mk_weapon_mesh_path + "Ammunition/SM_Shell_" + data.bullet_mesh_path;
        data.UI_material_bp_path = mk_rendertarget_mesh_path + data.type;
        data.bullet_bp_path      = mk_bullet_bp_path + data.type + "_bullet";
        arr_weapon_data.Add(data);
    }
}

void AData_table_manager::Init_other_weapon_data()
{
    // CSV 로드
    ConstructorHelpers::FObjectFinder<UDataTable> OTHER_WEAPON_DATA_TABLE(TEXT("/Game/Data/OTHER_WEAPON_DATA_TABLE"));

    if (OTHER_WEAPON_DATA_TABLE.Succeeded())
        mp_other_weapon_data_table = OTHER_WEAPON_DATA_TABLE.Object;

    if (!mp_other_weapon_data_table)
        return;

    // 모든 이름 가져오기
    TArray<FName> arr_row_name = mp_other_weapon_data_table->GetRowNames();

    // 갖고온 CSV로부터 데이터 할당
    for (int i = 0; i < arr_row_name.Num(); i++)
    {
        // row_name_arr 안에 정보 및 명칭
        auto p_row = mp_other_weapon_data_table->FindRow<Fs_other_weapon_data>(arr_row_name[i], arr_row_name[i].ToString());

        if (!p_row)
            break;

        Fs_other_weapon_data data = *p_row;
        data.mesh_path            = mk_other_weapon_mesh_path + data.weapon_group + "/Meshes/SM_" + data.type;
        arr_other_weapon_data.Add(data);
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
    TArray<FName> arr_row_name = mp_vehicle_data_table->GetRowNames();
    m_current_vehicle_count    = arr_row_name.Num();

    // 갖고온 CSV로부터 데이터 할당
    for (int i = 0; i < m_current_vehicle_count; i++)
    {
        // row_name_arr 안에 정보 및 명칭
        auto p_row = mp_vehicle_data_table->FindRow<Fs_vehicle_data>(arr_row_name[i], arr_row_name[i].ToString());

        if (!p_row)
            break;

        Fs_vehicle_data data    = *p_row;
        data.mesh_path          = mk_vehicle_mesh_path + data.mesh_path;
        data.anim_instance_path = mk_anim_instance_path + data.type + "/BP_AnimInst_" + data.type;

        // 좌석 정보 배치
        data.arr_player_seat_pos[0] = { data.seat_1_player_location, data.seat_1_camera_location };
        data.arr_player_seat_pos[1] = { data.seat_2_player_location, data.seat_2_camera_location };
        data.arr_player_seat_pos[2] = { data.seat_3_player_location, data.seat_3_camera_location };
        data.arr_player_seat_pos[3] = { data.seat_4_player_location, data.seat_4_camera_location };
        arr_vehicle_data.Add(data);
    }
}