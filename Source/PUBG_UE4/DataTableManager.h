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
    const FString mkVehicleMeshPath    = "/Game/Characters/Vehicles/Skeletons/";
    const FString mkAnimInstancePath   = "/Game/1_Blueprints/Vehicles/";
    const FString mkVehicleSoundPath   = "";
    int           mCurrentVehicleCount = 0;

    /** * \brief 무기 데이터 관련 */
    const FString mkWeaponMeshPath       = "/Game/AdvanceWeaponPack/Mesh/Weapon/";
    const FString mkBulletBP_path        = "/Game/1_Blueprints/Items/Weapons/Ammo/BP_";
    const FString mkRendertargetMeshPath = "/Game/Materials/Weapon_UI/UI_material_";
    const FString mkWeaponSoundPath      = "/Game/SFX/Gun_sounds/";

    /** \brief 기타 무기 (투척류/근접) 데이터 관련 */
    const FString mkOtherWeaponMeshPath = "/Game/Items/Weapons/";

    /** \brief 무기 부속품 데이터 관련 */
    const FString mkWeaponAttachmentMeshPath = "/Game/AdvanceWeaponPack/Mesh/Attachment/";

    /** \brief 총알 상자 데이터 관련 */
    const FString mkAmmoBoxMeshPath = "/Game/UI/AmmoBoxIcon/AmmoBox/";

public:
    static FsVehicleData GetVehicleData(int Index);

    static FsWeaponData GetWeaponData(int Index);

    static FsOtherWeaponData GetOtherWeaponData(int Index);

    static FsWeaponAttachmentData GetWeaponAttachmentData(int Index);

    static FsWeaponBulletData GetWeaponBulletData(int Index);

    static FsUtilityData GetUtilityData(int Index);

public:
    ADataTableManager();

private:
    /** \brief 차량 데이터 초기화 */
    void InitVehicleData();

    /** \brief 무기 데이터 초기화 */
    void InitWeaponData();

    /** \brief 기타 무기 데이터 초기화 */ 
    void InitOtherWeaponData();

    /** \brief 무기 부속품 데이터 초기화 */
    void InitWeaponAttachmentData();

    /** \brief 무기 총알 상자 데이터 초기화 */
    void InitWeaponBulletData();

    void InitUtilityData();
};