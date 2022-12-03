#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponBulletData.generated.h"

USTRUCT(BlueprintType)
struct FsWeaponBulletData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = WeaponBulletData) 
    FString Type = "";

    UPROPERTY(BlueprintReadWrite, Category = WeaponBulletData) 
    FString MeshType = "";

    UPROPERTY(BlueprintReadWrite, Category = WeaponBulletData) 
    int Count = 0;

    UPROPERTY(BlueprintReadWrite, Category = WeaponBulletData) 
    FString Description = "";
};