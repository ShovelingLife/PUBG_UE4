#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Weapon_data.generated.h"

#define MAX_WEAPON_COUNT 13

// CSV로부터 가지고올 무기 정보 구조체
USTRUCT(BlueprintType)
struct Fs_weapon_data : public FTableRowBase
{
    GENERATED_BODY()

        UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        FString type = "";

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        FString weapon_group_type = "";

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        FString mesh_path = "";

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        FString weapon_icon_path = "";

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        FString weapon_slot_icon_path = "";

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        FString bullet_mesh_path = "";

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        FString mag_mesh_path = "";

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        FVector collider_size = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        FVector collider_pos = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        int     max_bullet_count = 0;

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        int     damage = 0;

    FString UI_material_bp_path = "";
    FString bullet_bp_path = "";
    int     current_bullet_count = 0;
};

UCLASS()
class PUBG_UE4_API AWeapon_data : public AActor
{
	GENERATED_BODY()
};
