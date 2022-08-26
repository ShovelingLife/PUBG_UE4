
#include "CoreBackpack.h"

ACoreBackpack::ACoreBackpack(EBackpackType BackpackType) : ABaseInteraction()
{
    // 메쉬 레벨 순 2 > 4 > 7 
    FString meshPath = "/Game/Backpacks_Bags/Meshes/SM_Backpack_0";
    meshPath += FString::FromInt((int)BackpackType);
    InitStaticMesh(meshPath);

    FString backpackStr = "Backpack";

    switch (BackpackType)
    {
    case EBackpackType::FIRST: 
        backpackStr += "LV1";
        break;

    case EBackpackType::SECOND: 
        backpackStr += "LV2";
        break;

    case EBackpackType::THIRD: 
        backpackStr += "LV3";
        break;
    }
    ObjectType = backpackStr;

    // 저장 용량 25 > 50 > 100 순
    ChangeCollisionSettings();
}