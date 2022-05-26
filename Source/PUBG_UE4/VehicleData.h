/**
 * \file Vehicle_data.h
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
#include "VehicleData.generated.h"

 /**
   * \brief CSV�κ��� ������� ���� ���� ����ü\
  */
USTRUCT(BlueprintType)
struct FsVehicleData : public FTableRowBase
{
    GENERATED_BODY()

    /** \brief ���� ���� ������ */
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FString Type                    = "";
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FString MeshPath                = "";
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FString AudioPath               = "";
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector ColliderSize            = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector ColliderPos             = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector InteractionColliderSize = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector InteractionColliderPos  = FVector::ZeroVector;

    /** \brief ī�޶� ������ */
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector CameraLocation = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector CameraRotation = FVector::ZeroVector;

    /** \brief �÷��̾� 1 ������ */
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector SeatLocation1       = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector SeatCameraLocation1 = FVector::ZeroVector;

    /** \brief �÷��̾� 2 ������ */
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector SeatLocation2       = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector SeatCameraLocation2 = FVector::ZeroVector;

    /** \brief �÷��̾� 3 ������ */
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector SeatLocation3       = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector SeatCameraLocation3 = FVector::ZeroVector;

    /** \brief �÷��̾� 4 ������ */
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector SeatLocation4       = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) FVector SeatCameraLocation4 = FVector::ZeroVector;

    /** \brief ��Ÿ ������ */
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) float SpringArmLength = 0.f;
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) int   MaxSeater       = 0;
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) int   Durability      = 0;
    UPROPERTY(BlueprintReadWrite, Category = VehicleData) int   Speed           = 0;

    FString AnimInstancePath = "";
    std::pair<FVector, FVector> ArrPlayerSeatPos[4];

public:
    FsVehicleData() = default;
};