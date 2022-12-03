/**
 * \file Weapon_data.h
 *
 * \brief 무기 데이터
 *
 * \ingroup PUBG_UE4
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponData.generated.h"

 /** \brief CSV로부터 가지고올 무기 정보 구조체 */
USTRUCT(BlueprintType)
struct FsWeaponData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = WeaponData) 
    FString Type = "";

    UPROPERTY(BlueprintReadWrite, Category = WeaponData) 
    FString GroupType = "";

    UPROPERTY(BlueprintReadWrite, Category = WeaponData) 
    FString BulletType = "";

    UPROPERTY(BlueprintReadWrite, Category = WeaponData) 
    FString Description = "";

    UPROPERTY(BlueprintReadWrite, Category = WeaponData) 
    FString MeshPath = "";

    UPROPERTY(BlueprintReadWrite, Category = WeaponData) 
    FString IconPath = "";

    UPROPERTY(BlueprintReadWrite, Category = WeaponData) 
    FString SlotIconPath = "";

    UPROPERTY(BlueprintReadWrite, Category = WeaponData) 
    FString BulletMeshPath = "";

    UPROPERTY(BlueprintReadWrite, Category = WeaponData) 
    FString MagMeshPath = "";

    UPROPERTY(BlueprintReadWrite, Category = WeaponData) 
    FVector ColliderSize = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = WeaponData) 
    FVector ColliderPos = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = WeaponData) 
    int BulletLimitCount = 0;

    UPROPERTY(BlueprintReadWrite, Category = WeaponData) 
    int Damage = 0;

    UPROPERTY(BlueprintReadWrite, Category = WeaponData) 
    bool bUnderlined = false;

    FString BulletBP_path  = "";
    int MaxBulletCount = 0;
    int BulletCount    = 0;

public:
    FsWeaponData() = default;
};