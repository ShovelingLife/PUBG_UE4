// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Data_table_manager.generated.h"
#define MAX_WEAPON_COUNT 6
#define MAX_VEHICLE_COUNT 6

// CSV�κ��� ������� ���� ���� ����ü
USTRUCT(BlueprintType)
struct Fs_weapon_data : public FTableRowBase
{
    GENERATED_BODY()

        UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        FString type = "";

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        FString mesh_path = "";

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        FString audio_path = "";

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        FString bullet_mesh_path = "";

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        FString mag_mesh_path = "";

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        FVector collider_size = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        FVector collider_pos = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        int     current_ammunition_count = 0;

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        int     max_bullet_count = 0;

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        int     damage = 0;

    FString UI_material_bp_path = "";
    FString bullet_bp_path = "";
    int     current_bullet_count = 0;
};

// CSV�κ��� ������� ���� ���� ����ü
USTRUCT(BlueprintType)
struct Fs_vehicle_data : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_name)
    FString type = "";

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_path)
        FString mesh_path = "";

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_path)
        FString audio_path = "";

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_collider)
        FVector collider_size = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_collider)
        FVector collider_pos = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_interaction_collider)
        FVector interaction_collider_size = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_interaction_collider)
        FVector interaction_collider_pos = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_camera)
        FVector camera_location = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_camera)
        FVector camera_rotation = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_camera)
        float spring_arm_length = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data_variable)
        int     max_seater = 0;

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        int     durability = 0;

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        int     speed = 0;
};

UCLASS()
class PUBG_UE4_API AData_table_manager : public AActor
{
    GENERATED_BODY()

private:
    // ���� ���������̺� ����
    UPROPERTY(VisibleAnywhere)
    class UDataTable* mp_weapon_data_table;

    Fs_weapon_data    m_weapon_data_arr[MAX_WEAPON_COUNT];
    const FString	  mk_weapon_mesh_path       = "/Game/Items/Weapons/Meshes/";
    const FString     mk_bullet_bp_path         = "/Game/Blueprints/Weapons/Ammo/BP_";
    const FString     mk_rendertarget_mesh_path = "/Game/Materials/Weapon_UI/UI_material_";
    const FString     mk_weapon_sound_path      = "/Game/SFX/Gun_sounds/";
    int               m_current_weapon_count    = 0;

    // ���� ������ ���̺� ����
    UPROPERTY(VisibleAnywhere)
    class UDataTable* mp_vehicle_data_table;

    Fs_vehicle_data   m_vehicle_data_arr[MAX_VEHICLE_COUNT];
    const FString     mk_vehicle_mesh_path    = "/Game/Characters/Vehicles/Skeletons/";
    const FString     mk_vehicle_sound_path   = "";
    int               m_current_vehicle_count = 0;

private:
    void Init_weapon_data();

    void Init_vehicle_data();

public:
    // Sets default values for this actor's properties
    AData_table_manager();

public:
    Fs_weapon_data Get_weapon_data(int _index) { return m_weapon_data_arr[_index]; }

    Fs_vehicle_data Get_vehicle_data(int _index) { return m_vehicle_data_arr[_index]; }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
};