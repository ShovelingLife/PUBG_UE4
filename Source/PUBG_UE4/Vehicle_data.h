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
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Vehicle_data.generated.h"
 
/**
  * \brief CSV로부터 가지고올 차량 정보 구조체\
 */
USTRUCT(BlueprintType)
struct Fs_vehicle_data : public FTableRowBase
{
    GENERATED_BODY()

    /**
      * \brief 공통 차량 데이터 
     */
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FString type                      = "";
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FString mesh_path                 = "";
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FString audio_path                = "";
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector collider_size             = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector collider_pos              = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector interaction_collider_size = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector interaction_collider_pos  = FVector::ZeroVector;

    /**
      * \brief 카메라 데이터 
     */
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector camera_location = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector camera_rotation = FVector::ZeroVector;

    /**
      * \brief 플레이어 1 데이터
     */
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector seat_1_player_location = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector seat_1_camera_location = FVector::ZeroVector;
                     
    /**
      * \brief 플레이어 2 데이터
     */
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector seat_2_player_location = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector seat_2_camera_location = FVector::ZeroVector;
                                       
    /**
      * \brief 플레이어 3 데이터
     */
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector seat_3_player_location = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector seat_3_camera_location = FVector::ZeroVector;
                                      
    /**
      * \brief 플레이어 4 데이터
     */
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector seat_4_player_location = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) FVector seat_4_camera_location = FVector::ZeroVector;

    /**
      * \brief 기타 테이터
     */
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) float spring_arm_length = 0.f;
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) int   max_seater = 0;
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) int   durability = 0;
    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data) int   speed             = 0;

    FString                     anim_instance_path = "";
    std::pair<FVector, FVector> arr_player_seat_pos[4];

public:
    Fs_vehicle_data() = default;
};

UCLASS()
class PUBG_UE4_API AVehicle_data : public AActor
{
	GENERATED_BODY()
};
