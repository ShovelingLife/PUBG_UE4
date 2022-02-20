/**
 * \file Custom_player.h
 *
 * \brief 캐릭터 클래스 관련
 *
 * \ingroup Characters
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "PUBG_UE4/MyEnum.h"
#include "GameFramework/Character.h"
#include "CustomPlayer.generated.h"

/**
 * \brief 델리게이트 모음 (인벤토리 열기/닫기)
 */
DECLARE_DELEGATE(FDeleOpenInventory)
DECLARE_DELEGATE(FDeleCloseInventory)

class ASoundManager;
class AWeaponManager;
class ACoreWeapon;
class ACoreVehicle;
class UAudioComponent;
class UParticleSystemComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class CHARACTERS_API ACustomPlayer : public ACharacter
{
    GENERATED_BODY()

private:
    /**
     * \brief 플레이어가 사용하는 변수
     */
    UPROPERTY() ASoundManager* mpSoundManager;
    UPROPERTY() AWeaponManager* mpWeaponManager;
    UPROPERTY() AActor*         mpCollidedWeapon  = nullptr;
    UPROPERTY() ACoreVehicle*   mpCollidedVehicle = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Audio)    UAudioComponent*          AudioComp;
    UPROPERTY(VisibleAnywhere, Category = Particle) UParticleSystemComponent* ParticleComp;

    /**
     * \brief 이동 관련 변수
     */
    FVector mDirectionUpDown    = FVector::ZeroVector;
    FVector mDirectionLeftRight = FVector::ZeroVector;
    float   mSprintTime          = 0.f;
    float   mMaxSprintTime      = 0.5f;
    float   mSprintMultiplier    = 1.f;
    bool    mbMoving            = false;
    bool    mbInteracting       = false;
    bool    mbInventoryOpened  = false;

 // 플레이어 컴포넌트 및 상태 변수
public:
    FDeleOpenInventory  DeleOpenInventory;
    FDeleCloseInventory DeleCloseInventory;

    UPROPERTY(VisibleAnywhere, Category = Camera) USpringArmComponent* SpringArmComp = nullptr;
    UPROPERTY(VisibleAnywhere, Category = Camera) UCameraComponent*    CameraComp = nullptr;

    /**
     * \brief 플레이어 상태 관련 변수
     */
    EPlayerState CurrentState;
    const float    kMaxHealth          = 100.f;
    float          CurrentHealth        = 100;
    float          CurrentOxygen        = 1.f;
    bool           bSprinting          = false;
    bool           bAnimationPlaying  = false;
    bool           bAiming             = false;
    bool           bWeaponEquipped    = false;
    
    /**
     * \brief 차량 관련 변수
     */
    ESeatType    CurrentSeatType = ESeatType::NONE;
    bool           bInVehicle     = false;

public:
    ACustomPlayer();

private: 
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// 초기화 함수들
private:
    /**
     * \brief 플레이어에 대한 정보 갱신
     */
    void UpdatePlayerSettings();

    /**
     * \brief 카메라 컴포넌트 초기화
     */
    void InitCameraComp();

    /**
     * \brief 메시 컴포넌트 초기화
     */
    void InitMeshComp();

    /**
     * \brief 오디오 컴포넌트 초기화
     */
    void InitAudioComp();

    /**
     * \brief 애님 인스턴스 초기화
     */
    void InitAnimInstance();

    /**
     * \brief 파티클 시스템 초기화
     */
    void InitParticleComp();

private:
    /**
     * \brief 플레이어 움직임 감지
     */
    void CheckIfMoving();

    /**
     * \brief 충돌한 오브젝트 확인 (아이템) (레이캐스트 : 플레이어 발 밑)
     */
    void CheckForObject();

    /**
     * \brief 근처에 차량이 있는지 감지 (레이캐스트 : 플레이어 상체)
     */
    void CheckIfVehicleNear();

    /**
     * \brief 상호 작용 중일 시 (무기 줍기/차량 탑승)
     */
    void TryToInteract();

    /**
     * \brief F키 눌렀을 시 상호 작용
     */
    void BeginInteract() { mbInteracting = true; }

    /**
     * \brief F키를 뗐을 시 
     */
    void EndInteract()   { mbInteracting = false; }
  
    /**
     * \brief 앞 뒤로 움직임
     */
    void MoveForwardBack(float);

    /**
     * \brief 좌 우로 움직임
     */
    void MoveLeftRight(float);

    /**
     * \brief 위 아래 카메라 전환
     * \param _value 카메라 회전 값
     */
    void LookUp(float _value);

    /**
     * \brief 좌우 카메라 전환
     * \param _value 카메라 회전 값
     */
    void Turn(float _value);

    /**
     * \brief 점프
     */
    void CustomJump();

    /**
     * \brief 숙임
     */
    void CustomCrouch();

    /**
     * \brief 엎드림
     */
    void Proning();

    /**
     * \brief 뛰기 시작함
     */
    void BeginSprint();

    /**
     * \brief 뛰다 멈춤
     */
    void EndSprint();


    // ------- UI 관련 -------

    /**
     * \brief 인벤토리 열음/닫음
     */
    void OpenInventory();


    // ------- 무기 관련 -------

    /**
     * \brief 사격
     */
    void BeginShooting();

    /**
     * \brief 사격 중지
     */
    void EndShooting();

    /**
     * \brief 재장전
     */
    void Reload();

    /**
     * \brief 조준
     */
    void Aim();

    /**
     * \brief 격발 모드 변경
     */
    void ChangeShootMode();

    /**
     * \brief 마우스 위로 스크롤
     */
    void SwapScrollingUp();

    /**
     * \brief 마우스 아래로 스크롤
     */
    void SwapScrollingDown();

    /**
     * \brief 첫번째 무기 장착
     */
    void EquipFirstWeapon();

    /**
     * \brief 두번째 무기 장착
     */
    void EquipSecondWeapon();

    /**
     * \brief 세번째 무기 장착
     */
    void EquipThirdWeapon();

    /**
     * \brief 네번째 무기 장착
     */
    void EquipFourthWeapon();

    /**
     * \brief 다섯번째 무기 장착
     */
    void EquipFifthWeapon();

public:
    /**
     * \brief 플레이어가 차량에서 나감
     * \param _exit_location 나가는 위치
     */
    void ExitFromVehicle(FVector _exit_location);

    /**
     * \brief 무기 매니저를 갖고옴
     * \return 무기 매니저
     */
    AWeaponManager* GetWeaponManager() { return mpWeaponManager; }
};