#pragma once
#include "CoreMinimal.h"
#include "Vehicle_data.h"
#include "Weapon_data.h"
#include "Item_data.h"
#include "GameFramework/Actor.h"
#include "Data_table_manager.generated.h"

UCLASS()
class PUBG_UE4_API AData_table_manager : public AActor
{
    GENERATED_BODY()

private:
    // 무기 데이터테이블 관련
    UPROPERTY(VisibleAnywhere)
    class UDataTable* mp_weapon_data_table;

    Fs_weapon_data    m_arr_weapon_data[MAX_WEAPON_COUNT];
    const FString	  mk_weapon_mesh_path       = "/Game/AdvanceWeaponPack/Mesh/Weapon/";
    const FString     mk_bullet_bp_path         = "/Game/Blueprints/Weapons/Ammo/BP_";
    const FString     mk_rendertarget_mesh_path = "/Game/Materials/Weapon_UI/UI_material_";
    const FString     mk_weapon_sound_path      = "/Game/SFX/Gun_sounds/";
    int               m_current_weapon_count    = 0;

    // 차량 데이터 테이블 관련
    UPROPERTY(VisibleAnywhere)
    class UDataTable* mp_vehicle_data_table;

    Fs_vehicle_data   m_arr_vehicle_data[MAX_VEHICLE_COUNT];
    const FString     mk_vehicle_mesh_path = "/Game/Characters/Vehicles/Skeletons/";
    const FString     mk_anim_instance_path = "/Game/Blueprints/Vehicles/";
    const FString     mk_vehicle_sound_path   = "";
    int               m_current_vehicle_count = 0;

private:
    void Init_weapon_data();

    void Init_vehicle_data();

public:
    // Sets default values for this actor's properties
    AData_table_manager();

public:
    Fs_weapon_data  Get_weapon_data(int _index)  { return m_arr_weapon_data[_index]; }

    Fs_vehicle_data Get_vehicle_data(int _index) { return m_arr_vehicle_data[_index]; }

    auto            Get_player_vehicle_pos(void) { return m_arr_vehicle_data[0].arr_player_seat_pos[0]; }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
};