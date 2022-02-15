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
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Weapon_data.generated.h"

/**
  * \brief CSV로부터 가지고올 무기 정보 구조체
 */
USTRUCT()
struct Fs_weapon_data : public FTableRowBase
{
    GENERATED_BODY(BlueprintType)

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) FString type                  = "";
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) FString weapon_group_type     = "";
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) FString bullet_type           = "";
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) FString mesh_path             = "";
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) FString weapon_icon_path      = "";
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) FString weapon_slot_icon_path = "";
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) FString bullet_mesh_path      = "";
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) FString mag_mesh_path         = "";
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) FVector collider_size         = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) FVector collider_pos          = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) int     max_bullet_count      = 0;
    UPROPERTY(BlueprintReadWrite, Category = Weapon_data) int     damage                = 0;

    FString UI_material_bp_path      = "";
    FString bullet_bp_path           = "";
    int     current_max_bullet_count = 0;
    int     current_bullet_count     = 0;

public:
    Fs_weapon_data() = default;
};

UCLASS()
class PUBG_UE4_API AWeapon_data : public AActor
{
	GENERATED_BODY()
};
