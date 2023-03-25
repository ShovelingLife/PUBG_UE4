#include "CoreBackpack.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "PUBG_UE4/DataTableManager.h"
#include "Kismet/GameplayStatics.h"

// 가방 오브젝트 초기화
ACoreBackpack::ACoreBackpack(EBackpackType BackpackType) : Super()
{
    Data = ADataTableManager::GetUtilityData((int)BackpackType);

    // 메쉬 레벨 순 2 > 4 > 7 
    FString meshPath    = "/Game/2_Meshes/Backpacks_Bags/Meshes/SM_Backpack_0";
    FString backpackStr = "Backpack";
    float   newZ = 0.f;
    int     idx  = 0;

    switch (BackpackType)
    {
    case EBackpackType::FIRST:  backpackStr += "LV1"; newZ = 20.f; idx = 2; break;
    case EBackpackType::SECOND: backpackStr += "LV2"; newZ = 25.f; idx = 4; break;
    case EBackpackType::THIRD:  backpackStr += "LV3"; newZ = 29.f; idx = 7; break;
    }
    InitStaticMesh(meshPath + FString::FromInt(idx));
    ObjectType = backpackStr;

    // 디폴트 z축 20
    FVector newPos = StaticMeshComp->GetRelativeLocation();
    newPos.Z += newZ;
    StaticMeshComp->AddRelativeLocation(newPos);

    // 저장 용량 25 > 50 > 100 순
    ChangeMeshSettings();
}