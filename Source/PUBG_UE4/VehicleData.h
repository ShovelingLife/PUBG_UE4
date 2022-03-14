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
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FString Type                    = "";
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FString MeshPath                = "";
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FString AudioPath               = "";
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector ColliderSize            = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector ColliderPos             = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector InteractionColliderSize = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector InteractionColliderPos  = FVector::ZeroVector;

    /** \brief ī�޶� ������ */
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector CameraLocation = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector CameraRotation = FVector::ZeroVector;

    /** \brief �÷��̾� 1 ������ */
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector SeatLocation1       = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector SeatCameraLocation1 = FVector::ZeroVector;

    /** \brief �÷��̾� 2 ������ */
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector SeatLocation2       = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector SeatCameraLocation2 = FVector::ZeroVector;

    /** \brief �÷��̾� 3 ������ */
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector SeatLocation3       = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector SeatCameraLocation3 = FVector::ZeroVector;

    /** \brief �÷��̾� 4 ������ */
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector SeatLocation4       = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector SeatCameraLocation4 = FVector::ZeroVector;

    /** \brief ��Ÿ ������ */
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) float SpringArmLength = 0.f;
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) int   MaxSeater       = 0;
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) int   Durability      = 0;
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) int   Speed           = 0;

    FString AnimInstancePath = "";
    std::pair<FVector, FVector> ArrPlayerSeatPos[4];

public:
    FsVehicleData() = default;
};