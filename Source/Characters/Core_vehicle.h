// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "PUBG_UE4/My_enum.h"
#include "PUBG_UE4/Data_table_manager.h"
#include "Core_vehicle.generated.h"
class ACustom_player;
class APawn;

UCLASS()
class CHARACTERS_API ACore_vehicle : public AWheeledVehiclePawn
{
    GENERATED_BODY()

protected:
    // 컴포넌트들
    UPROPERTY(VisibleAnywhere, Category = Camera)
        class USpringArmComponent* mp_spring_arm = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Camera)
        class UCameraComponent* mp_camera = nullptr;

    // 문짝 위치
    UPROPERTY(EditAnywhere, Category = Door_Pos)
        USceneComponent* first_door_pos;

    UPROPERTY(EditAnywhere, Category = Door_Pos)
        USceneComponent* second_door_pos;

    UPROPERTY(EditAnywhere, Category = Door_Pos)
        USceneComponent* third_door_pos;

    UPROPERTY(EditAnywhere, Category = Door_Pos)
    USceneComponent* fourth_door_pos;

    // 좌석 위치
    UPROPERTY(EditAnywhere, Category = Seat_Pos)
        USceneComponent* first_seat_pos;

    UPROPERTY(EditAnywhere, Category = Seat_Pos)
        USceneComponent* second_seat_pos;

    UPROPERTY(EditAnywhere, Category = Seat_Pos)
        USceneComponent* third_seat_pos;

    UPROPERTY(EditAnywhere, Category = Seat_Pos)
    USceneComponent* fourth_seat_pos;


    // 기타 변수
    UPROPERTY(VisibleAnywhere,Category = Seat_pos)
        TMap<e_seat_type, FVector> m_map_seat_pos;

    UPROPERTY(VisibleAnywhere, Category = Door_pos)
        TMap<e_seat_type, FVector> m_map_door_pos;

    TMap<e_seat_type, bool> m_map_empty_seat
    {
        { e_seat_type::FIRST,false },
        { e_seat_type::SECOND,false },
        { e_seat_type::THIRD,false },
        { e_seat_type::FOURTH,false }
    };
    ACustom_player*       m_player;
    Fs_vehicle_data       m_vehicle_data;
    e_seat_type           m_seat_type = e_seat_type::NONE;
    const FString         mk_wheel_path = "/Game/Blueprints/Vehicles/Wheel/";
    FString               m_mesh_path = "";
    int                   m_current_player_count = 0;

public:
    // ����
    TSubclassOf< UUserWidget>	widget_bp_class;

    UPROPERTY(VisibleAnywhere, Category = UI)
        class UWidgetComponent* p_widget_component;

public:
    // Sets default values for this actor's properties
    ACore_vehicle();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float) override;

    virtual void NotifyActorBeginOverlap(AActor*) override;

    virtual void NotifyActorEndOverlap(AActor*) override;

    virtual void SetupPlayerInputComponent(class UInputComponent*) override;

protected:
    void Load_from_csv_vehicle(e_vehicle_type, Fs_vehicle_data&);

    void Init_car_pos_component();

    void Init(e_vehicle_type);

    void Init_UI();

    void Update_widget_component();

    void Init_camera();

    void Init_skeletal_mesh(FString);

    void Init_wheeled_component();

    void Init_car_pos_data();

    void Player_exit();

    // 
    void Accelerate(float);

    void Brake(float);

    // 
    void Handling(float);

    void Check_for_door_pos();

    void Check_empty_seat();

    void Change_to_first_seat() { if (!m_player) return;  m_seat_type = e_seat_type::FIRST; }

    void Change_to_second_seat() { if (!m_player) return; m_seat_type = e_seat_type::SECOND; }

    void Change_to_third_seat()  { if (m_vehicle_data.max_seater == 2 || !m_player) return; m_seat_type = e_seat_type::THIRD; }

    void Change_to_fourth_seat() { if (m_vehicle_data.max_seater == 2 || !m_player) return; m_seat_type = e_seat_type::FOURTH; }

    void Update_player_location();

    // 운전 방지
    bool Is_player_in_first_seat();

public:
    // 탑승 가능 여부 확인
    bool Check_available_seat(ACustom_player*);
};