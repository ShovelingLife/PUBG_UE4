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
#include "PUBG_UE4/MyEnum.h"
#include "PUBG_UE4/VehicleData.h"
#include "CoreVehicle.generated.h"

class ACustomPlayer;
class APawn;
class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;

UCLASS()
class CHARACTERS_API ACoreVehicle : public AWheeledVehiclePawn
{
    GENERATED_BODY()

protected:
    /**
     * \brief 차량 관련 컴포넌트들
     */
    UPROPERTY(VisibleAnywhere, Category = Camera) USpringArmComponent* SpringArmComp = nullptr;
    UPROPERTY(VisibleAnywhere, Category = Camera) UCameraComponent*    CameraComp    = nullptr;

    /**
     * \brief 문짝 위치 컴포넌트들
     */
    UPROPERTY(EditAnywhere, Category = Door_Pos) USceneComponent* FirstDoorPosComp;
    UPROPERTY(EditAnywhere, Category = Door_Pos) USceneComponent* SecondDoorPosComp;
    UPROPERTY(EditAnywhere, Category = Door_Pos) USceneComponent* ThirdDoorPosComp;
    UPROPERTY(EditAnywhere, Category = Door_Pos) USceneComponent* FourthDoorPosComp;

    /**
     * \brief 좌석 위치 컴포넌트들
     */
    UPROPERTY(EditAnywhere, Category = Seat_Pos) USceneComponent* FirstSeatPosComp;
    UPROPERTY(EditAnywhere, Category = Seat_Pos) USceneComponent* SecondSeatPosComp;
    UPROPERTY(EditAnywhere, Category = Seat_Pos) USceneComponent* ThirdSeatPosComp;
    UPROPERTY(EditAnywhere, Category = Seat_Pos) USceneComponent* FourthSeatPosComp;

    /**
     * \brief 기타 변수들
     */
    UPROPERTY(VisibleAnywhere,Category = Seat_pos)  TMap<ESeatType, FVector> MapSeatPos;
    UPROPERTY(VisibleAnywhere, Category = Door_pos) TMap<ESeatType, FVector> MapDoorPos;

    /**
     * \brief 차량 좌석 위치 변수
     */
    TMap<ESeatType, bool> mMapEmptySeat
    {
        { ESeatType::FIRST, false },
        { ESeatType::SECOND,false },
        { ESeatType::THIRD, false },
        { ESeatType::FOURTH,false },
        { ESeatType::NONE,  false }
    };
    ACustomPlayer* mpPlayer;
    FsVehicleData  mVehicleData;                       
    const FString  mkWheelPath         = "/Game/Blueprints/Vehicles/Wheel/";
    FString        mMeshPath           = "";
    int            mCurrentPlayerCount = 0;
    bool           mbPlayerInFirstSeat = false;

public:
    UPROPERTY(VisibleAnywhere, Category = Widget_component) UWidgetComponent* InteractionWidgetComp;
    bool bCollided = false;

public:
    ACoreVehicle();

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
    void Init(EVehicleType _vehicle_type_index);

    /**
     * .
     * 차량에 필요한 컴포넌트 초기화 해주는 함수
     */
    void InitCarPosComp();

    /**
     * \brief 자동차 문짝 및 좌석 위치 초기화
     */
    void InitCarPosData();

    /**
     * \brief 자동차 문짝 및 좌석 위치 갱신 
     */
    void UpdateCarPosData();

    /**
     * \brief 카메라 컴포넌트 초기화
     */
    void InitCamera();

    /**
     * \brief 차량 메시 초기화
     */
    void InitSkeletalMesh();

    /**
     * \brief 차량 바퀴 컴포넌트 초기화
     */
    void InitWheeledComp();

// 차량 속성 관련 함수
protected:
    /**
     * \brief 플레이어가 차량에서 나감
     */
    void PlayerExit();

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
    void LookUp(float _value);

    /**
     * \brief 카메라를 좌우로 회전
     * \param _value 회전 값
     */
    void Turn(float _value);

    /**
     * \brief 어느 문 통해 들어갔는지 확인
     */
    void CheckForDoorPos();

    /**
     * \brief 플레이어를 좌석에 배치
     */
    void SetPlayerIntoSeat();

    /**
     * \brief 차 안에서 플레이어의 위치를 업데이트 시킴
     * \param _seat_type 좌석 종류
     */
    void UpdatePlayerSeatLocation(ESeatType _seat_type);

    /**
     * \brief 첫번째 좌석으로 이동
     */
    void ChangeToFirstSeat() { if (!mpPlayer) return;  UpdatePlayerSeatLocation(ESeatType::FIRST); }

    /**
     * \brief 두번째 좌석으로 이동
     */
    void ChangeToSecondSeat() { if (!mpPlayer) return; UpdatePlayerSeatLocation(ESeatType::SECOND); }

    /**
     * \brief 세번째 좌석으로 이동
     */
    void ChangeToThirdSeat() { if (mVehicleData.MaxSeater == 2 || !mpPlayer) return; UpdatePlayerSeatLocation(ESeatType::THIRD); }

    /**
     * \brief 네번째 좌석으로 이동
     */
    void ChangeToFourthSeat() { if (mVehicleData.MaxSeater == 2 || !mpPlayer) return; UpdatePlayerSeatLocation(ESeatType::FOURTH); }
    
public:
    /**
     * \brief 빈 좌석인지 확인
     * \param _p_player 플레이어
     * \return boolean 탑승 여부
     */
    bool IsSeatAvailable(ACustomPlayer* _p_player);
};