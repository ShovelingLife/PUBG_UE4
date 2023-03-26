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
class UInstaDeformComponent;
class UWidgetComponent;

UCLASS()
class CHARACTERS_API ACoreVehicle : public AWheeledVehiclePawn
{
using enum ESeatType;

    GENERATED_BODY()

protected:
#pragma region 차량 관련 컴포넌트들

    UPROPERTY(VisibleAnywhere, Category = Property) 
    UInstaDeformComponent* InstaDeformComp = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Camera) 
    USpringArmComponent* SpringArmComp = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Camera) 
    UCameraComponent* CameraComp = nullptr;

#pragma endregion

#pragma region 문짝 위치 컴포넌트들

    UPROPERTY(EditAnywhere, Category = DoorPos) 
    USceneComponent* FirstDoorPosComp;

    UPROPERTY(EditAnywhere, Category = DoorPos) 
    USceneComponent* SecondDoorPosComp;

    UPROPERTY(EditAnywhere, Category = DoorPos) 
    USceneComponent* ThirdDoorPosComp;

    UPROPERTY(EditAnywhere, Category = DoorPos) 
    USceneComponent* FourthDoorPosComp;

#pragma endregion

#pragma region 좌석 위치 컴포넌트들

    UPROPERTY(EditAnywhere, Category = SeatPos) 
    USceneComponent* FirstSeatPosComp;

    UPROPERTY(EditAnywhere, Category = SeatPos) 
    USceneComponent* SecondSeatPosComp;

    UPROPERTY(EditAnywhere, Category = SeatPos) 
    USceneComponent* ThirdSeatPosComp;

    UPROPERTY(EditAnywhere, Category = SeatPos) 
    USceneComponent* FourthSeatPosComp;

#pragma endregion

#pragma region 기타 변수들

    UPROPERTY(VisibleAnywhere,Category = SeatPos)  
    TMap<ESeatType, FVector> MapSeatPos;

    UPROPERTY(VisibleAnywhere, Category = DoorPos) 
    TMap<ESeatType, FVector> MapDoorPos;

#pragma endregion

#pragma region 차량 좌석 위치 변수

    const FString  mkWheelPath = "/Game/1_Blueprints/Vehicles/Wheel/";
    TMap<ESeatType, bool> mMapEmptySeat;
    ACustomPlayer* mpPlayer;
    FsVehicleData  mVehicleData;                       
    FString        mMeshPath           = "";
    int            mCurrentPlayerCount = 0;
    bool           mbPlayerInFirstSeat = false;

#pragma endregion

public:
    UPROPERTY(VisibleAnywhere, Category = WidgetComp) 
    UWidgetComponent* WidgetComp;
    
    bool bPlayerNear = false;

public:
    ACoreVehicle();

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent*) override;

protected:
    
#pragma region 초기화 함수들

    // 차량 종류에 따라 초기화
    void Init(EVehicleType VehicleTypeIndex);

    // 차량에 필요한 컴포넌트 초기화 해주는 함수
    void InitCarPosComp();

    // 자동차 문짝 및 좌석 위치 초기화
    void InitCarPosData();

    // 자동차 문짝 및 좌석 위치 갱신
    void UpdateCarPosData();

    // 카메라 컴포넌트 초기화
    void InitCamera();

    // 차량 메시 초기화
    void InitSkeletalMesh();

    // 차량 바퀴 컴포넌트 초기화
	void InitWheeledComp();

#pragma endregion

#pragma region 차량 속성 관련 함수
protected:
    // 플레이어가 차량에서 나감
    void PlayerExit();

    // 가속
    void Accelerate(float Value);

    // 브레이크
    void Brake(float Value);

    // 회전
    void Handling(float Value);

    // 카메라를 위 아래로 회전
    void LookUp(float Value);

    // 카메라를 좌우로 회전
    void Turn(float Value);

    // 어느 문 통해 들어갔는지 확인
    void CheckForDoorPos();

    // 플레이어를 좌석에 배치
    void SetPlayerIntoSeat();

    // 차 안에서 플레이어의 위치를 업데이트 시킴
    void UpdatePlayerSeatLocation(ESeatType SeatType);

    // 첫번째 좌석으로 이동
    void ChangeToFirstSeat() { UpdatePlayerSeatLocation(FIRST); }

    // 두번째 좌석으로 이동
    void ChangeToSecondSeat() { UpdatePlayerSeatLocation(SECOND); }

    // 세번째 좌석으로 이동
    void ChangeToThirdSeat() { UpdatePlayerSeatLocation(THIRD); }

    // 네번째 좌석으로 이동
    void ChangeToFourthSeat() { UpdatePlayerSeatLocation(FOURTH); }

#pragma endregion
    
public:
    /**
     * \brief 빈 좌석인지 확인
     * \param _p_player 플레이어
     * \return boolean 탑승 여부
     */
    bool IsSeatAvailable(ACustomPlayer* pPlayer);
};