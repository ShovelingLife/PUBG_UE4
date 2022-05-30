#pragma once

#include "CoreMinimal.h"
#include "FarmableItemEnum.generated.h"

UENUM()
enum class EWeaponAttachmentType
{
    // 소음기
    COMPENSATOR_BARREL,
    DOUBLE_BARREL_EXTENDED_BARREL,
    FLASH_HIDER_BARREL,
    MUZZLE_BRAKE_9MM_BARREL,
    MUZZLE_BRAKE_BARREL,
    PX70_EXTENDED_BARREL,
    SUPRESSOR_BARREL,

    // 총열
    BIPOD_FOREND,
    KNIFE_FOREND,
    LASER9MM_FOREND,
    LASER_FOREND,
    TORCH_FOREND,

    // 손잡이
    ANGLED01_GRIP,
    ANGLED02_GRIP,
    VERTICAL01_GRIP,
    VERTICAL02_GRIP,
    VERTICAL03_GRIP,

    // 조준경
    AYAKASHI_SCOPE,
    C7_IRS,
    EON_CONV_IRS,
    HOLOGRAPHIC_SIGHT,
    LS80_IRS,
    RED_DOT_SIGHT,
    REFLEX_SIGHT,
    SK12_IRS,
    SPECTER_SIGHT,
    UTC_SIGHT,

    // 개머리판
    C7_STOCKA,
    C7_STOCKB,
    DOUBLE_BARREL_STOCK,
    EON_STOCKA,
    EON_STOCKB,
    KAROS_STOCK,
    PX70_STOCKA,
    PX70_STOCKB,
    ROGER_LR220,
    VSS_STOCKA,
    VSS_STOCKB
};

UENUM()
enum class EAmmoBoxType
{
    AMMO_12_GAUGE,
    AMMO_20MM,
    AMMO_22,
    AMMO_45,
    AMMO_50,
    AMMO_500SW,
    AMMO_5_45,
    AMMO_7_62,
    AMMO_9MM,
    NONE
};