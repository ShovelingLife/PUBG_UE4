// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Vehicle_data.generated.h"

#define MAX_VEHICLE_COUNT 6

// CSV로부터 가지고올 차량 정보 구조체
USTRUCT(BlueprintType)
struct Fs_vehicle_data : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_name)
    FString type = "";

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_path)
        FString mesh_path = "";

    FString anim_instance_path = "";

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_path)
        FString audio_path = "";

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_collider)
        FVector collider_size = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_collider)
        FVector collider_pos = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_interaction_collider)
        FVector interaction_collider_size = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_interaction_collider)
        FVector interaction_collider_pos = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_camera)
        FVector camera_location = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_camera)
        FVector camera_rotation = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_seat_1_player)
        FVector seat_1_player_location = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_seat_1_camera)
        FVector seat_1_camera_location = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_seat_2_player)
        FVector seat_2_player_location = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_seat_2_camera)
        FVector seat_2_camera_location = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_seat_3_player)
        FVector seat_3_player_location = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_seat_3_camera)
        FVector seat_3_camera_location = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_seat_4_player)
        FVector seat_4_player_location = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_seat_4_camera)
        FVector seat_4_camera_location = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_camera)
        float spring_arm_length = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = Vehicle_data_variable)
        int     max_seater = 0;

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        int     durability = 0;

    UPROPERTY(BlueprintReadWrite, Category = Weapon_data_variable)
        int     speed = 0;

    std::pair<FVector, FVector> arr_player_seat_pos[4];
};

UCLASS()
class PUBG_UE4_API AVehicle_data : public AActor
{
	GENERATED_BODY()
};
