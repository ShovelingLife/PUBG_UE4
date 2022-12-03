#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UtilityData.generated.h"

/** \brief CSV로부터 가지고올 무기 정보 구조체 */
USTRUCT(BlueprintType)
struct FsUtilityData : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, Category = UtilityData) 
    FString Category = "";

    UPROPERTY(BlueprintReadWrite, Category = UtilityData) 
    FString Type = "";
    
    UPROPERTY(BlueprintReadWrite, Category = UtilityData) 
    FString MeshPath = "";

    // 가방 용량 / HP 회복 / 에너지 회복 등 하나로 통일
    UPROPERTY(BlueprintReadWrite, Category = UtilityData) 
    int PropVal = 0;
    
public:
    FsUtilityData() = default;
};