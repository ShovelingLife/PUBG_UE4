/**
 * \file Core_vehicle.h
 *
 * \brief 차량 메인 클래스 (틀대) 관련
 *
 * \ingroup Characters
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "PUBG_UE4/My_enum.h"
#include "PUBG_UE4/Data_table_manager.h"
#include "Core_vehicle.generated.h"

class ACustom_player;
class APawn;
class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;

UCLASS()
class CHARACTERS_API ACore_vehicle : public AWheeledVehiclePawn
{
    GENERATED_BODY()

protected:
    /**
     * \brief 차량 관련 컴포넌트들
     */
    UPROPERTY(VisibleAnywhere, Category = Camera) USpringArmComponent* mp_spring_arm_comp = nullptr;
    UPROPERTY(VisibleAnywhere, Category = Camera) UCameraComponent* mp_camera_comp = nullptr;

    /**
     * \brief 문짝 위치 컴포넌트들
     */
    UPROPERTY(EditAnywhere, Category = Door_Pos) USceneComponent* first_door_pos_comp;
    UPROPERTY(EditAnywhere, Category = Door_Pos) USceneComponent* second_door_pos_comp;
    UPROPERTY(EditAnywhere, Category = Door_Pos) USceneComponent* third_door_pos_comp;
    UPROPERTY(EditAnywhere, Category = Door_Pos) USceneComponent* fourth_door_pos_comp;

    /**
     * \brief 좌석 위치 컴포넌트들
     */
    UPROPERTY(EditAnywhere, Category = Seat_Pos) USceneComponent* first_seat_pos_comp;
    UPROPERTY(EditAnywhere, Category = Seat_Pos) USceneComponent* second_seat_pos_comp;
    UPROPERTY(EditAnywhere, Category = Seat_Pos) USceneComponent* third_seat_pos_comp;
    UPROPERTY(EditAnywhere, Category = Seat_Pos) USceneComponent* fourth_seat_pos_comp;

    /**
     * \brief 기타 변수들
     */
    UPROPERTY(VisibleAnywhere,Category = Seat_pos)  TMap<e_seat_type, FVector> m_map_seat_pos;
    UPROPERTY(VisibleAnywhere, Category = Door_pos) TMap<e_seat_type, FVector> m_map_door_pos;

    /**
     * \brief 차량 좌석 위치 변수
     */
    TMap<e_seat_type, bool> m_map_empty_seat
    {
        { e_seat_type::FIRST, false },
        { e_seat_type::SECOND,false },
        { e_seat_type::THIRD, false },
        { e_seat_type::FOURTH,false },
        { e_seat_type::NONE,  false }
    };
    ACustom_player*    m_player;
    Fs_vehicle_data    m_vehicle_data;
                       
    const FString      mk_wheel_path             = "/Game/Blueprints/Vehicles/Wheel/";
    FString            m_mesh_path               = "";
    int                m_current_player_count    = 0;
    bool               m_is_player_in_first_seat = false;

public:
    UPROPERTY(VisibleAnywhere, Category = Widget_component) UWidgetComponent* mp_interaction_widget_comp;
    bool is_collided = false;

public:
    ACore_vehicle();

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float) override;

    virtual void SetupPlayerInputComponent(class UInputComponent*) override;

protected:
    /**
     * \brief 차량 종류에 따라 초기화
     * 
     * \param _vehicle_type_index 차량 종류
     */
    void Init(e_vehicle_type _vehicle_type_index);

    /**
     * .
     * 차량에 필요한 컴포넌트 초기화 해주는 함수
     */
    void Init_car_pos_component();

    /**
     * \brief 자동차 문짝 및 좌석 위치 초기화
     */
    void Init_car_pos_data();

    /**
     * \brief 자동차 문짝 및 좌석 위치 갱신 
     */
    void Update_car_pos_data();

    /**
     * \brief 카메라 컴포넌트 초기화
     */
    void Init_camera();

    /**
     * \brief 차량 메시 초기화
     */
    void Init_skeletal_mesh();

    /**
     * \brief 차량 바퀴 컴포넌트 초기화
     */
    void Init_wheeled_component();

// 차량 속성 관련 함수
protected:
    /**
     * \brief 플레이어가 차량에서 나감
     */
    void Player_exit();

    /**
     * \brief 가속
     * \param _value 가속 값
     */
    void Accelerate(float _value);

    /**
     * \brief 브레이크
     * \param _value 브레이크 값
     */
    void Brake(float _value);

    /**
     * \brief 회전
     * \param _value 회전 값
     */
    void Handling(float _value);

    /**
     * \brief 카메라를 위 아래로 회전
     * \param _value 회전 값
     */
    void Look_up(float _value);

    /**
     * \brief 카메라를 좌우로 회전
     * \param _value 회전 값
     */
    void Turn(float _value);

    /**
     * \brief 어느 문 통해 들어갔는지 확인
     */
    void Check_for_door_pos();

    /**
     * \brief 플레이어를 좌석에 배치
     */
    void Set_player_into_seat_location();

    /**
     * \brief 차 안에서 플레이어의 위치를 업데이트 시킴
     * \param _seat_type 좌석 종류
     */
    void Update_player_seat_location(e_seat_type _seat_type);

    /**
     * \brief 첫번째 좌석으로 이동
     */
    void Change_to_first_seat() { if (!m_player) return;  Update_player_seat_location(e_seat_type::FIRST); }

    /**
     * \brief 두번째 좌석으로 이동
     */
    void Change_to_second_seat() { if (!m_player) return; Update_player_seat_location(e_seat_type::SECOND); }

    /**
     * \brief 세번째 좌석으로 이동
     */
    void Change_to_third_seat() { if (m_vehicle_data.max_seater == 2 || !m_player) return; Update_player_seat_location(e_seat_type::THIRD); }

    /**
     * \brief 네번째 좌석으로 이동
     */
    void Change_to_fourth_seat() { if (m_vehicle_data.max_seater == 2 || !m_player) return; Update_player_seat_location(e_seat_type::FOURTH); }
    
public:
    /**
     * \brief 빈 좌석인지 확인
     * \param _p_player 플레이어
     * \return boolean 탑승 여부
     */
    bool Check_available_seat(ACustom_player* _p_player);
};