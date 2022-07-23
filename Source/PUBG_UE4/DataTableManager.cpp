#include "DataTableManager.h"

TArray<FsVehicleData>          ADataTableManager::ArrVehicleData;
TArray<FsWeaponData>           ADataTableManager::ArrWeaponData;
TArray<FsOtherWeaponData>      ADataTableManager::ArrOtherWeaponData;
TArray<FsWeaponAttachmentData> ADataTableManager::ArrWeaponAttachmentData;
TArray<FsWeaponBulletData> ADataTableManager::ArrWeaponBulletData;

// Sets default values
ADataTableManager::ADataTableManager()
{
    InitVehicleData();
    InitWeaponData();
    InitOtherWeaponData();
    InitWeaponAttachmentData();
    InitWeaponBulletData();
}

void ADataTableManager::InitVehicleData()
{
    // CSV 로드
    ConstructorHelpers::FObjectFinder<UDataTable> VEHICLE_DATA_TABLE(TEXT("/Game/Data/VEHICLE_DATA_TABLE"));
    UDataTable* pWeaponDataTable = nullptr;

    if (VEHICLE_DATA_TABLE.Succeeded())
        pWeaponDataTable = VEHICLE_DATA_TABLE.Object;

    if (!pWeaponDataTable)
        return;

    // 모든 이름 가져오기
    TArray<FName> arrRowName = pWeaponDataTable->GetRowNames();

    // 갖고온 CSV로부터 데이터 할당
    for (int i = 0; i < arrRowName.Num(); i++)
    {
        // row_name_arr 안에 정보 및 명칭
        auto p_row = pWeaponDataTable->FindRow<FsVehicleData>(arrRowName[i], arrRowName[i].ToString());

        if (!p_row)
            break;

        FsVehicleData data = *p_row;
        data.MeshPath = mkVehicleMeshPath + data.MeshPath;
        data.AnimInstancePath = mkAnimInstancePath + data.Type + "/BP_AnimInst_" + data.Type;        

        // 좌석 정보 배치
        data.ArrPlayerSeatPos[0] = { data.SeatLocation1, data.SeatCameraLocation1 };
        data.ArrPlayerSeatPos[1] = { data.SeatLocation2, data.SeatCameraLocation2 };
        data.ArrPlayerSeatPos[2] = { data.SeatLocation3, data.SeatCameraLocation3 };
        data.ArrPlayerSeatPos[3] = { data.SeatLocation4, data.SeatCameraLocation4 };
        ArrVehicleData.Add(data);
    }
}

void ADataTableManager::InitWeaponData()
{
    // CSV 로드
    ConstructorHelpers::FObjectFinder<UDataTable> WEAPON_DATA_TABLE(TEXT("/Game/Data/WEAPON_DATA_TABLE"));
    UDataTable* pWeaponDataTable = nullptr;

    if (WEAPON_DATA_TABLE.Succeeded())
        pWeaponDataTable = WEAPON_DATA_TABLE.Object;

    if (!pWeaponDataTable)
        return;

    // 모든 이름 가져오기
    TArray<FName> arrRowName = pWeaponDataTable->GetRowNames();
    
    // 갖고온 CSV로부터 데이터 할당
    for (int i = 0; i < arrRowName.Num(); i++)
    {
        // row_name_arr 안에 정보 및 명칭
        auto p_row = pWeaponDataTable->FindRow<FsWeaponData>(arrRowName[i], arrRowName[i].ToString());

        if (!p_row)
            break;

        FsWeaponData data   = *p_row;
        auto weaponType = data.Type;
        data.MeshPath       = mkWeaponMeshPath + data.GroupType + "/SK_" + data.MeshPath;
        data.MagMeshPath    = mkWeaponMeshPath + weaponType + data.MagMeshPath;
        data.BulletMeshPath = "/Game/AmmoPack/Models/" + data.BulletMeshPath;
        FString bulletPath = "";

        if (weaponType == "Ayakashi" ||
            weaponType == "Crossbow" ||
            weaponType == "DoubleBarrel" ||
            weaponType == "Karos")
            bulletPath = "Bullet";

        else
            bulletPath = "_Bullet";

        data.BulletBP_path  = mkBulletBP_path + data.Type + bulletPath;        
        ArrWeaponData.Add(data);
    }
}

void ADataTableManager::InitOtherWeaponData()
{
    // CSV 로드
    ConstructorHelpers::FObjectFinder<UDataTable> OTHER_WEAPON_DATA_TABLE(TEXT("/Game/Data/OTHER_WEAPON_DATA_TABLE"));
    UDataTable* pWeaponDataTable = nullptr;

    if (OTHER_WEAPON_DATA_TABLE.Succeeded())
        pWeaponDataTable = OTHER_WEAPON_DATA_TABLE.Object;

    if (!pWeaponDataTable)
        return;

    // 모든 이름 가져오기
    TArray<FName> arrRowName = pWeaponDataTable->GetRowNames();

    // 갖고온 CSV로부터 데이터 할당
    for (int i = 0; i < arrRowName.Num(); i++)
    {
        // row_name_arr 안에 정보 및 명칭
        auto p_row = pWeaponDataTable->FindRow<FsOtherWeaponData>(arrRowName[i], arrRowName[i].ToString());

        if (!p_row)
            break;

        auto data     = *p_row;
        data.MeshPath = mkOtherWeaponMeshPath + data.GroupType + "/Meshes/SM_" + data.Type;
        ArrOtherWeaponData.Add(data);
    }
}

void ADataTableManager::InitWeaponAttachmentData()
{
    // CSV 로드
    ConstructorHelpers::FObjectFinder<UDataTable> WEAPON_ATTACHMENT_DATA_TABLE(TEXT("/Game/Data/WEAPON_ATTACHMENT_DATA_TABLE"));
    UDataTable* pWeaponDataTable = nullptr;

    if (WEAPON_ATTACHMENT_DATA_TABLE.Succeeded())
        pWeaponDataTable = WEAPON_ATTACHMENT_DATA_TABLE.Object;

    if (!pWeaponDataTable)
        return;

    // 모든 이름 가져오기
    TArray<FName> arrRowName = pWeaponDataTable->GetRowNames();

    // 갖고온 CSV로부터 데이터 할당
    for (int i = 0; i < arrRowName.Num(); i++)
    {
        // row_name_arr 안에 정보 및 명칭
        auto p_row = pWeaponDataTable->FindRow<FsWeaponAttachmentData>(arrRowName[i], arrRowName[i].ToString());

        if (!p_row)
            break;

        FsWeaponAttachmentData data = *p_row;
        FString groupType = data.GroupType;

        if (groupType == "Scope" ||
            groupType == "IRS")
            groupType = "Sight";

        if (groupType == "StockA" ||
            groupType == "StockB")
            groupType = "Stock";

        data.MeshPath = mkWeaponAttachmentMeshPath + groupType + "/SM_" + data.Type + "_" + data.GroupType;
        data.Description = "Attachable in : " + data.WeaponMatchType;
        ArrWeaponAttachmentData.Add(data);
    }
}

void ADataTableManager::InitWeaponBulletData()
{
    // CSV 로드
    ConstructorHelpers::FObjectFinder<UDataTable> WEAPON_ATTACHMENT_DATA_TABLE(TEXT("/Game/Data/WEAPON_BULLET_DATA_TABLE"));
    UDataTable* pWeaponDataTable = nullptr;

    if (WEAPON_ATTACHMENT_DATA_TABLE.Succeeded())
        pWeaponDataTable = WEAPON_ATTACHMENT_DATA_TABLE.Object;

    if (!pWeaponDataTable)
        return;

    // 모든 이름 가져오기
    TArray<FName> arrRowName = pWeaponDataTable->GetRowNames();

    // 갖고온 CSV로부터 데이터 할당
    for (int i = 0; i < arrRowName.Num(); i++)
    {
        // row_name_arr 안에 정보 및 명칭
        auto p_row = pWeaponDataTable->FindRow<FsWeaponBulletData>(arrRowName[i], arrRowName[i].ToString());

        if (!p_row)
            break;

        ArrWeaponBulletData.Add(*p_row);
    }
}