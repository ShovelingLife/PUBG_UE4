
#include "CoreBackpack.h"

ACoreBackpack::ACoreBackpack(EBackpackType BackpackType) : Super()
{
    // �޽� ���� �� 2 > 4 > 7 
    FString meshPath = "/Game/Backpacks_Bags/Meshes/SM_Backpack_0";
    meshPath += FString::FromInt((int)BackpackType);
    InitStaticMesh(meshPath);

    FString backpackStr = "Backpack";
    float newZ = 0.f;

    switch (BackpackType)
    {
    case EBackpackType::FIRST /*(2)*/: backpackStr += "LV1"; newZ = 20.f; break;
    case EBackpackType::SECOND/*(4)*/: backpackStr += "LV2"; newZ = 25.f; break;
    case EBackpackType::THIRD /*(7)*/: backpackStr += "LV3"; newZ = 29.f; break;
    }
    ObjectType = backpackStr;
    // ����Ʈ z�� 20
    FVector newPos = StaticMeshComp->GetRelativeLocation();
    newPos.Z += newZ;
    StaticMeshComp->AddRelativeLocation(newPos);

    // ���� �뷮 25 > 50 > 100 ��
    ChangeCollisionSettings();
}