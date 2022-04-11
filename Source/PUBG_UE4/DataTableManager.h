/**
 * \file Data_table_manager.h
 *
 * \brief 데이터 테이블 매니저
 *
 * \ingroup PUBG_UE4
 *
 * \author ShovelingLife
 */
#pragma once
#include "CoreMinimal.h"
#include "VehicleData.h"
#include "OtherWeaponData.h"
#include "WeaponData.h"
#include "GameFramework/Actor.h"
#include "DataTableManager.generated.h"

class UDataTable;

UCLASS()
class PUBG_UE4_API ADataTableManager : public AActor
{
    GENERATED_BODY()
        
public:
    /**
      * \brief 게임 오브젝트 데이터 배열 (static)
     */
    static TArray<FsWeaponData>      ArrWeaponData;
    static TArray<FsOtherWeaponData> ArrOtherWeaponData;
    static TArray<FsVehicleData>     ArrVehicleData;

private:
    /**
      * \brief 무기 데이터 관련
     */
    UPROPERTY(VisibleAnywhere) UDataTable* mpWeaponDataTable;

    const FString mkWeaponMeshPath       = "/Game/AdvanceWeaponPack/Mesh/Weapon/";
    const FString mkBulletBP_path        = "/Game/Blueprints/Weapons/Ammo/BP_";
    const FString mkRendertargetMeshPath = "/Game/Materials/Weapon_UI/UI_material_";
    const FString mkWeaponSoundPath      = "/Game/SFX/Gun_sounds/";

    /**
      * \brief 기타 무기 (투척류/근접) 데이터 관련
     */
    UPROPERTY(VisibleAnywhere) UDataTable* mpOtherWeaponDataTable;

    const FString mkOtherWeaponMeshPath = "/Game/Items/Weapons/";

    /**
      * \brief 차량 데이터 관련
     */
    UPROPERTY(VisibleAnywhere) UDataTable* mpVehicleDataTable;

    const FString mkVehicleMeshPath    = "/Game/Characters/Vehicles/Skeletons/";
    const FString mkAnimInstancePath   = "/Game/Blueprints/Vehicles/";
    const FString mkVehicleSoundPath   = "";
    int           mCurrentVehicleCount = 0;

public:
    ADataTableManager();

private:
    /** \brief 무기 데이터 초기화 */
    void InitWeaponData();

    /** \brief 기타 무기 데이터 초기화 */ 
    void InitOtherWeaponData();

    /** \brief 차량 데이터 초기화 */
    void InitVehicleData();
};