/**
 * \file Weapon_data.h
 *
 * \brief ���� ������
 *
 * \ingroup PUBG_UE4
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponData.generated.h"

 /**
   * \brief CSV�κ��� ������� ���� ���� ����ü
  */
USTRUCT()
struct FsWeaponData : public FTableRowBase
{
    GENERATED_BODY(BlueprintType)

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) FString Type           = "";
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) FString GroupType      = "";
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) FString BulletType     = "";
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) FString Description    = "";
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) FString MeshPath       = "";
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) FString IconPath       = "";
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) FString SlotIconPath   = "";
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) FString BulletMeshPath = "";
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) FString MagMeshPath    = "";
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) FVector ColliderSize   = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) FVector ColliderPos    = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) int     MaxBulletCount = 0;
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) int     Damage         = 0;

    FString BulletBP_path         = "";
    int     CurrentMaxBulletCount = 0;
    int     CurrentBulletCount    = 0;

public:
    FsWeaponData() = default;
};