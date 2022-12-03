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
#include "WeaponAttachmentData.h"
#include "WeaponBulletData.h"
#include "UtilityData.h"
#include "Global.h"
#include "GameFramework/Actor.h"
#include "DataTableManager.generated.h"

class UDataTable;

UCLASS()
class PUBG_UE4_API ADataTableManager : public AActor
{
    GENERATED_BODY()

public:
    /** \brief 게임 오브젝트 데이터 배열 (static) */
    static TArray<FsVehicleData>          ArrVehicleData;
    static TArray<FsWeaponData>           ArrWeaponData;
    static TArray<FsOtherWeaponData>      ArrOtherWeaponData;    
    static TArray<FsWeaponAttachmentData> ArrWeaponAttachmentData;
    static TArray<FsWeaponBulletData>     ArrWeaponBulletData;
    static TArray<FsUtilityData>          ArrUtilityData;

private:
    /** \brief 차량 데이터 관련 */
    const FString mkAnimInstancePath   = Global::BlueprintFolder + "Vehicles/";
    const FString mkVehicleMeshPath    = Global::MeshFolder + "Vehicles/Skeletons/";
    const FString mkVehicleSoundPath   = "";
    int           mCurrentVehicleCount = 0;

    /** * \brief 무기 데이터 관련 */
    const FString mkBulletBP_path        = Global::BlueprintFolder + "Items/Weapons/Ammo/BP_";
    const FString mkWeaponMeshPath       = Global::MeshFolder + "Weapons/Mesh/Guns/";
    const FString mkRendertargetMeshPath = "/Game/Materials/Weapon_UI/UI_material_";
    const FString mkWeaponSoundPath      = "/Game/SFX/Gun_sounds/";

    /** \brief 기타 무기 (투척류/근접) 데이터 관련 */
    const FString mkOtherWeaponMeshPath = Global::MeshFolder + "Weapons/Mesh/";

    /** \brief 무기 부속품 데이터 관련 */
    const FString mkWeaponAttachmentMeshPath = Global::MeshFolder + "Weapons/Mesh/Attachment/";

    /** \brief 총알 상자 데이터 관련 */
    const FString mkAmmoBoxMeshPath = "/Game/UI/AmmoBoxIcon/AmmoBox/";

public:
    static FsVehicleData GetVehicleData(int Index) { return ArrVehicleData.IsEmpty() ? FsVehicleData() : ArrVehicleData[Index]; }

    static FsWeaponData GetWeaponData(int Index) { return (ArrWeaponData.IsEmpty() ? FsWeaponData() : ArrWeaponData[Index]); }

    static FsOtherWeaponData GetOtherWeaponData(int Index) { return (ArrOtherWeaponData.IsEmpty() ? FsOtherWeaponData() : ArrOtherWeaponData[Index]); }

    static FsWeaponAttachmentData GetWeaponAttachmentData(int Index) { return (ArrWeaponAttachmentData.IsEmpty() ? FsWeaponAttachmentData() : ArrWeaponAttachmentData[Index]); }

    static FsWeaponBulletData GetWeaponBulletData(int Index) { return (ArrWeaponBulletData.IsEmpty() ? FsWeaponBulletData() : ArrWeaponBulletData[Index]); }

    static FsUtilityData GetUtilityData(int Index) { return (ArrUtilityData.IsEmpty() ? FsUtilityData() : ArrUtilityData[Index]); }

public:
    ADataTableManager();

private:
    /** \brief 차량 데이터 초기화 */
    void InitVehicleDataArr();

    /** \brief 무기 데이터 초기화 */
    void InitWeaponDataArr();

    /** \brief 기타 무기 데이터 초기화 */ 
    void InitOtherWeaponDataArr();

    /** \brief 무기 부속품 데이터 초기화 */
    void InitWeaponAttachmentDataArr();

    /** \brief 무기 총알 상자 데이터 초기화 */
    void InitWeaponBulletDataArr();

    void InitUtilityDataArr();
};