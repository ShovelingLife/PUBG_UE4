#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponBulletData.generated.h"

USTRUCT()
struct FsWeaponBulletData : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, Category = WeaponData) FString Type = "";
    UPROPERTY(BlueprintReadWrite, Category = WeaponData) FString MeshType = "";
    //UPROPERTY(BlueprintReadWrite, Category = WeaponData) EAmmoBoxType Code = EAmmoBoxType::NONE;
};