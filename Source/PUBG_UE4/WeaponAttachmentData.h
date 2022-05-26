#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponAttachmentData.generated.h"

USTRUCT(BlueprintType)
struct FsWeaponAttachmentData : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, Category = WeaponAttachmentData) FString Type;
    UPROPERTY(BlueprintReadWrite, Category = WeaponAttachmentData) FString GroupType;
    UPROPERTY(BlueprintReadWrite, Category = WeaponAttachmentData) FString WeaponMatchType;
    UPROPERTY(BlueprintReadWrite, Category = WeaponAttachmentData) FString MeshPath;
    UPROPERTY(BlueprintReadWrite, Category = WeaponAttachmentData) FString Description;
};