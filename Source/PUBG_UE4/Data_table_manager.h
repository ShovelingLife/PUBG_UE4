// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Data_table_manager.generated.h"
#define MAX_WEAPON_COUNT 6

// Fs_weapon_data
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

UCLASS()
class PUBG_UE4_API AData_table_manager : public AActor
{
	GENERATED_BODY()
	
private:
    // 데이터테이블 관련
    UPROPERTY(VisibleAnywhere)
    class UDataTable* mp_weapon_data_table;
    Fs_weapon_data    m_weapon_data_arr[MAX_WEAPON_COUNT];
    int               m_current_weapon_count = 0;

    // 경로 관련
    const FString	  mk_weapon_mesh_path       = "/Game/FPS_Weapon_Bundle/Weapons/Meshes/";
    const FString     mk_bullet_bp_path         = "/Game/Blueprints/Weapon/Ammo/BP_";
    const FString     mk_rendertarget_mesh_path = "/Game/Materials/Weapon_UI/UI_material_";
    const FString     mk_sound_path             = "/Game/SFX/Gun_sounds/";

private:
    void Init_weapon_data();

public:
    // Sets default values for this actor's properties
    AData_table_manager();

public:
    Fs_weapon_data Get_weapon_data(int _index) { return m_weapon_data_arr[_index]; }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
};