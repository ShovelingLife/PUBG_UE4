#pragma once
#include "CoreMinimal.h"
#include "Vehicle_data.h"
#include "Other_weapon_data.h"
#include "Weapon_data.h"
#include "GameFramework/Actor.h"
#include "Data_table_manager.generated.h"

class UDataTable;

UCLASS()
class PUBG_UE4_API AData_table_manager : public AActor
{
    GENERATED_BODY()
        
public:
    static TArray<Fs_weapon_data>       arr_weapon_data;
    static TArray<Fs_other_weapon_data> arr_other_weapon_data;
    static TArray<Fs_vehicle_data>      arr_vehicle_data;

private:
    // ------- ���� ������ ���� -------

    UPROPERTY(VisibleAnywhere) UDataTable* mp_weapon_data_table;

    const FString mk_weapon_mesh_path       = "/Game/AdvanceWeaponPack/Mesh/Weapon/";
    const FString mk_bullet_bp_path         = "/Game/Blueprints/Weapons/Ammo/BP_";
    const FString mk_rendertarget_mesh_path = "/Game/Materials/Weapon_UI/UI_material_";
    const FString mk_weapon_sound_path      = "/Game/SFX/Gun_sounds/";


    // ------- ��Ÿ ���� (��ô��/����) ������ ���� -------

    UPROPERTY(VisibleAnywhere) UDataTable* mp_other_weapon_data_table;

    const FString mk_other_weapon_mesh_path = "/Game/Items/Weapons/";


    // ------- ���� ������ ���� -------

    UPROPERTY(VisibleAnywhere) UDataTable* mp_vehicle_data_table;

    const FString mk_vehicle_mesh_path    = "/Game/Characters/Vehicles/Skeletons/";
    const FString mk_anim_instance_path   = "/Game/Blueprints/Vehicles/";
    const FString mk_vehicle_sound_path   = "";
    int           m_current_vehicle_count = 0;

public:
    AData_table_manager();

private:
    // ���� ������ �ʱ�ȭ
    void Init_weapon_data();

    // ��Ÿ ���� ������ �ʱ�ȭ
    void Init_other_weapon_data();

    // ���� ������ �ʱ�ȭ
    void Init_vehicle_data();
};