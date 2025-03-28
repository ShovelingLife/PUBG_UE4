/**
 * \file Vehicle_data.h
 *
 * \brief 차량 데이터
 *
 * \ingroup PUBG_UE4
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "VehicleData.generated.h"

 /**
   * \brief CSV로부터 가지고올 차량 정보 구조체\
  */
USTRUCT(BlueprintType)
struct FsVehicleData : public FTableRowBase
{
    GENERATED_BODY()

    /** \brief 공통 차량 데이터 */
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FString Type      = "";
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FString MeshPath  = "";
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FString AudioPath = "";

    /** \brief 카메라 데이터 */
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector CameraLocation = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector CameraRotation = FVector::ZeroVector;

    /** \brief 플레이어 1 데이터 */
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector SeatLocation1       = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector SeatCameraLocation1 = FVector::ZeroVector;

    /** \brief 플레이어 2 데이터 */
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector SeatLocation2       = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector SeatCameraLocation2 = FVector::ZeroVector;

    /** \brief 플레이어 3 데이터 */
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector SeatLocation3       = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector SeatCameraLocation3 = FVector::ZeroVector;

    /** \brief 플레이어 4 데이터 */
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector SeatLocation4       = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector SeatCameraLocation4 = FVector::ZeroVector;

    /** \brief 기타 테이터 */
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) int MaxSeater  = 0;
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) int Durability = 0;
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) int Speed      = 0;

    FString AnimInstancePath = "";
    std::pair<FVector, FVector> ArrPlayerSeatPos[4];

public:
    FsVehicleData() = default;
};