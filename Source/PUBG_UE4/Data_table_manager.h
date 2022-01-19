#pragma once
#include "CoreMinimal.h"
#include "Vehicle_data.h"
#include "Other_weapon_data.h"
#include "Weapon_data.h"
#include "Item_data.h"
#include "GameFramework/Actor.h"
#include "Data_table_manager.generated.h"

UCLASS()
class PUBG_UE4_API AData_table_manager : public AActor
{
    GENERATED_BODY()

private:
    // ------- ���� ������ ���� -------

    UPROPERTY(VisibleAnywhere)
    class UDataTable* mp_weapon_data_table;

    const FString	  mk_weapon_mesh_path       = "/Game/AdvanceWeaponPack/Mesh/Weapon/";
    const FString     mk_bullet_bp_path         = "/Game/Blueprints/Weapons/Ammo/BP_";
    const FString     mk_rendertarget_mesh_path = "/Game/Materials/Weapon_UI/UI_material_";
    const FString     mk_weapon_sound_path      = "/Game/SFX/Gun_sounds/";


    // ------- ��Ÿ ���� (��ô��/����) ������ ���� -------

    UPROPERTY(VisibleAnywhere)
    class UDataTable* mp_other_weapon_data_table;

    const FString	  mk_other_weapon_mesh_path = "/Game/Items/Weapons/";

    // ------- ���� ������ ���� -------

    UPROPERTY(VisibleAnywhere)
    class UDataTable* mp_vehicle_data_table;

    const FString     mk_vehicle_mesh_path    = "/Game/Characters/Vehicles/Skeletons/";
    const FString     mk_anim_instance_path   = "/Game/Blueprints/Vehicles/";
    const FString     mk_vehicle_sound_path   = "";
    int               m_current_vehicle_count = 0;

public:
    static Fs_weapon_data       arr_weapon_data[MAX_WEAPON_COUNT];
    static Fs_vehicle_data      arr_vehicle_data[MAX_VEHICLE_COUNT];
    static Fs_other_weapon_data arr_other_weapon_data[MAX_OTHER_WEAPON_COUNT];

private:
    // ���� ������ �ʱ�ȭ
    void Init_weapon_data();

    // ��Ÿ ���� ������ �ʱ�ȭ
    void Init_another_weapon_data();

    // ���� ������ �ʱ�ȭ
    void Init_vehicle_data();

public:
    // Sets default values for this actor's properties
    AData_table_manager();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
};