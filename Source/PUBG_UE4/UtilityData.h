#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UtilityData.generated.h"

/** \brief CSV�κ��� ������� ���� ���� ����ü */
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

    // ���� �뷮 / HP ȸ�� / ������ ȸ�� �� �ϳ��� ����
    UPROPERTY(BlueprintReadWrite, Category = UtilityData) 
    int PropVal = 0;
    
public:
    FsUtilityData() = default;
};