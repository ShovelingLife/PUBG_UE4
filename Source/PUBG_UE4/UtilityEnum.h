// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UtilityEnum.generated.h"


UENUM()
enum class EBackpackType : uint8
{
    FIRST,
    SECOND,
    THIRD,
    MAX
};

UENUM()
enum class EHelmetType : uint8
{
    FIRST = (uint8)EBackpackType::MAX,
    SECOND,
    THIRD,
    MAX
};

UENUM()
enum class EBodyArmorType : uint8
{
    FIRST = (uint8)EHelmetType::MAX,
    SECOND,
    THIRD,
    MAX
};