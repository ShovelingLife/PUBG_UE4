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
#include "PUBG_UE4/WeaponEnum.h"
#include "GameFramework/Character.h"
#include "CustomPlayer.generated.h"

/** \brief 델리게이트 모음 (인벤토리 열기/닫기) */
DECLARE_DELEGATE(FDeleOpenInventory)
DECLARE_DELEGATE(FDeleCloseInventory)

class ABaseInteraction;
class ACoreWeapon;
class ADummyCharacter;
class AWeaponManager;
class UCustomGameInstance;

class UAudioComponent;
class UCameraComponent;
class UParticleSystemComponent;
class USpringArmComponent;

UCLASS()
class CHARACTERS_API ACustomPlayer : public ACharacter
{
    GENERATED_BODY()
        
using enum EWeaponType;
using enum EPlayerState;

private:
    UPROPERTY() 
    UCustomGameInstance* mpCustomGameInst = nullptr;

    // 플레이어가 사용하는 변수
    UPROPERTY() 
    AWeaponManager* mpWeaponManager = nullptr;

    UPROPERTY() 
    ABaseInteraction* mpCollidedItem = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Audio)    
    UAudioComponent* AudioComp    = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Particle) 
    UParticleSystemComponent* ParticleComp = nullptr;

    /** \brief 이동 관련 변수 */
    FVector mDirectionUpDown    = FVector::ZeroVector;
    FVector mDirectionLeftRight = FVector::ZeroVector;
    float   mSprintTime         = 0.f;
    float   mMaxSprintTime      = 0.5f;
    float   mSprintMultiplier   = 1.f;
    bool    mbMoving            = false;
    bool    mbInteracting       = false;
    bool    mbInventoryOpened = false;

// 플레이어 컴포넌트 및 상태 변수
public:
    // ------- FPS 카메라 -------
    UPROPERTY(VisibleAnywhere, Category = CameraVar) 
    USpringArmComponent* FPS_SpringArmComp = nullptr;

    UPROPERTY(VisibleAnywhere, Category = CameraVar) 
    UCameraComponent* FPS_CameraComp = nullptr;

    // ------- TPS 카메라 -------
    UPROPERTY(VisibleAnywhere, Category = CameraVar) 
    USpringArmComponent* TPS_SpringArmComp = nullptr;
    
    UPROPERTY(VisibleAnywhere, Category = CameraVar) 
    UCameraComponent* TPS_CameraComp = nullptr;

    // ------- 인벤토리 캐릭터 UI 관련 -------
    UPROPERTY(EditAnywhere)
    TSubclassOf<ADummyCharacter> BP_DummyCharacter;

    UPROPERTY()
    ADummyCharacter* pDummyCharacter;

    FDeleOpenInventory  DeleOpenInventory;
    FDeleCloseInventory DeleCloseInventory;

    // 에임 관련

    // 플레이어 상태 관련 변수
    EPlayerState CurrentState;
    EPlayerOtherState CurrentOtherState = EPlayerOtherState::NONE;
    const float  kMaxHealth           = 100.f;
    float        CurrentHealth        = 100.f;
    float        CurrentInjuredHealth = 100.f;
    float        CurrentOxygen        = 1.f;
    bool         bAnimationPlaying    = false;

    UPROPERTY(EditAnywhere) float Zval;
    FTimerHandle AimTimerHandle;
    
    // 차량 관련 변수
    ESeatType CurrentSeatType = ESeatType::NONE;
    bool bInVehicle = false;

public:
    ACustomPlayer();

private: 
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// 초기화 함수들
private:
    // 플레이어에 대한 정보 갱신
    void InitPlayerSettings();

    // 카메라 컴포넌트 초기화
    void InitCameraComp();

    // 메시 컴포넌트 초기화
    void InitMeshComp();

    // 오디오 컴포넌트 초기화
    void InitAudioComp();

    // 애님 인스턴스 초기화
    void InitAnimInstance();

    // 파티클 시스템 초기화
    void InitParticleComp();

private:
    // 플레이어 움직임 감지
    void CheckIfMoving();

    // 충돌한 오브젝트 확인 (아이템) (레이캐스트 : 플레이어 발 밑)
    void CheckNearObj();

    // 근처에 차량이 있는지 감지 (레이캐스트 : 플레이어 상체)
    void CheckNearVehicle();

    // 상호 작용 중일 시 (무기 줍기/차량 탑승)
    void TryToInteract();

    // F키 눌렀을 시 상호 작용
    void BeginInteract() { mbInteracting = true; }

    // F키를 뗐을 시
    void EndInteract()   { mbInteracting = false; }

    // 점프
    void CustomJump();

    // 숙임
    void CustomCrouch();

    // 엎드림
    void Proning();

    // 뛰기 시작함
    void BeginSprint();

    // 뛰다 멈춤
    void EndSprint();

    // 앞 뒤로 움직임
    void MoveForwardBack(float Value);

    // 좌 우로 움직임
    void MoveLeftRight(float Value);

    /**
     * \brief 위 아래 카메라 전환
     * \param _value 카메라 회전 값
     */
    void LookUp(float Value);

    /**
     * \brief 좌우 카메라 전환
     * \param _value 카메라 회전 값
     */
    void Turn(float Value);

    // ------- UI 관련 -------

    // 인벤토리 열음/닫음
    void OpenInventory();

    void UpdateHealth();

    // ------- 무기 관련 -------

    // 사격
    void BeginShooting();

    // 사격 중지
    void EndShooting();

    // 재장전
    void Reload();

    // 조준 
    void Aim();

    // 조준 해제 체크
    UFUNCTION()
    void TryToAim();

    // 격발 모드 변경
    void ChangeShootMode();

    // 마우스 위로 스크롤
    void SwapScrollingUp() { CheckForWeapon(NONE, "Up"); }

    // 마우스 아래로 스크롤
    void SwapScrollingDown() { CheckForWeapon(NONE, "Down"); }

    // 첫번째 무기 장착
    void EquipFirstWeapon() { CheckForWeapon(FIRST); }

    // 두번째 무기 장착
    void EquipSecondWeapon() { CheckForWeapon(SECOND); }

    // 세번째 무기 장착
    void EquipThirdWeapon() { CheckForWeapon(PISTOL); }

    // 네번째 무기 장착
    void EquipFourthWeapon() { CheckForWeapon(MELEE); }

    // 다섯번째 무기 장착
    void EquipFifthWeapon() { CheckForWeapon(THROWABLE); }

    void CheckForWeapon(EWeaponType CurrentWeaponType = NONE, FString Direction = "");

    // 조준선을 확대
    UFUNCTION()
    void ZoomIn();

    UFUNCTION()
    // 조준선을 원위치 시킴
    void ZoomOut();

public:
    UFUNCTION() 
    void DealDmg(float DmgVal);

    UFUNCTION() 
    void SetOtherState(EPlayerOtherState OtherState) { this->CurrentOtherState = OtherState; }

    AWeaponManager* GetWeaponManager() { return mpWeaponManager; }

    ACoreWeapon* GetCurrentWeapon();

    /**
     * \brief 플레이어가 차량에서 나감
     * \param _exit_location 나가는 위치
     */
    void ExitFromVehicle(FVector ExitLocation);
};