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

class ACoreAmmoBox;
class ACoreAttachment;
class ACoreFarmableItem;
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
    UPROPERTY() UCustomGameInstance* mpCustomGameInst = nullptr;
    /** \brief 플레이어가 사용하는 변수 */
    UPROPERTY() AWeaponManager*  mpWeaponManager            = nullptr;
    UPROPERTY() AActor*          mpCollidedWeapon           = nullptr;
    UPROPERTY() ACoreAttachment* mpCollidedWeaponAttachment = nullptr;
    UPROPERTY() ACoreAmmoBox* mpCollidedAmmoBox = nullptr;
    UPROPERTY() ACoreFarmableItem* mpFarmableItem           = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Audio)    UAudioComponent*          AudioComp    = nullptr;
    UPROPERTY(VisibleAnywhere, Category = Particle) UParticleSystemComponent* ParticleComp = nullptr;

    /** \brief 이동 관련 변수 */
    FVector mDirectionUpDown    = FVector::ZeroVector;
    FVector mDirectionLeftRight = FVector::ZeroVector;
    float   mSprintTime         = 0.f;
    float   mMaxSprintTime      = 0.5f;
    float   mSprintMultiplier   = 1.f;
    bool    mbMoving            = false;
    bool    mbInteracting       = false;
    bool    mbInventoryOpened = false;
    bool mbAiming = false;

// 플레이어 컴포넌트 및 상태 변수
public:
    UPROPERTY(EditAnywhere) TSubclassOf<ADummyCharacter> BP_DummyCharacter;
    UPROPERTY() ADummyCharacter* pDummyCharacter;

    UPROPERTY(VisibleAnywhere, Category = CameraVar) USpringArmComponent* FPS_SpringArmComp = nullptr;
    UPROPERTY(VisibleAnywhere, Category = CameraVar) UCameraComponent*    FPS_CameraComp    = nullptr;

    UPROPERTY(VisibleAnywhere, Category = CameraVar) USpringArmComponent* TPS_SpringArmComp = nullptr;
    UPROPERTY(VisibleAnywhere, Category = CameraVar) UCameraComponent*    TPS_CameraComp    = nullptr;

    UPROPERTY(VisibleAnywhere, Category = CameraVar) UCameraComponent* Aim_CameraComp = nullptr;
    UPROPERTY(VisibleAnywhere, Category = CameraVar) FTimerHandle AimTimerHandle;
    //UPROPERTY(VisibleAnywhere, Category = TPS_Variable) UStaticMeshComponent*    TPS_MeshComp    = nullptr;

    FDeleOpenInventory  DeleOpenInventory;
    FDeleCloseInventory DeleCloseInventory;

    /** \brief 플레이어 상태 관련 변수 */
    EPlayerState CurrentState;
    EPlayerOtherState CurrentOtherState = EPlayerOtherState::NONE;
    const float  kMaxHealth           = 100.f;
    float        CurrentHealth        = 100.f;
    float        CurrentInjuredHealth = 100.f;
    float        CurrentOxygen        = 1.f;
    bool         bAnimationPlaying    = false;

    UPROPERTY(EditAnywhere) float Zval;
    
    /** \brief 차량 관련 변수 */
    ESeatType CurrentSeatType = ESeatType::NONE;
    UPROPERTY(BlueprintReadOnly) bool bInVehicle = false;

public:
    ACustomPlayer();

private: 
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// 초기화 함수들
private:
    /** \brief 플레이어에 대한 정보 갱신 */
    void InitPlayerSettings();

    /** \brief 카메라 컴포넌트 초기화 */
    void InitCameraComp();

    /** \brief 메시 컴포넌트 초기화 */
    void InitMeshComp();

    /** \brief 오디오 컴포넌트 초기화 */
    void InitAudioComp();

    /** \brief 애님 인스턴스 초기화 */
    void InitAnimInstance();

    /** \brief 파티클 시스템 초기화 */
    void InitParticleComp();

private:
    /** \brief 플레이어 움직임 감지 */
    void CheckIfMoving();

    /** \brief 충돌한 오브젝트 확인 (아이템) (레이캐스트 : 플레이어 발 밑) */
    void CheckNearObj();

    /** \brief 근처에 차량이 있는지 감지 (레이캐스트 : 플레이어 상체) */
    void CheckNearVehicle();

    /** \brief 상호 작용 중일 시 (무기 줍기/차량 탑승) */
    void TryToInteract();

    /** \brief F키 눌렀을 시 상호 작용 */
    void BeginInteract() { mbInteracting = true; }

    /** \brief F키를 뗐을 시 */
    void EndInteract()   { mbInteracting = false; }

    /** \brief 점프 */
    void CustomJump();

    /** \brief 숙임 */
    void CustomCrouch();

    /** \brief 엎드림 */
    void Proning();

    /** \brief 뛰기 시작함 */
    void BeginSprint();

    /** \brief 뛰다 멈춤 */
    void EndSprint();

    /** \brief 앞 뒤로 움직임 */
    void MoveForwardBack(float Value);

    /** \brief 좌 우로 움직임 */
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

    /** \brief 인벤토리 열음/닫음 */
    void OpenInventory();

    void UpdateHealth();

    // ------- 무기 관련 -------

    /** \brief 사격 */
    void BeginShooting();

    /** \brief 사격 중지 */
    void EndShooting();

    /** \brief 재장전 */
    void Reload();

    /** \brief 조준 */
    void Aim();

    /** \brief 격발 모드 변경 */
    void ChangeShootMode();

    /** \brief 마우스 위로 스크롤 */
    void SwapScrollingUp() { CheckForWeapon(NONE, "Up"); }

    /** \brief 마우스 아래로 스크롤 */
    void SwapScrollingDown() { CheckForWeapon(NONE, "Down"); }

    /** \brief 첫번째 무기 장착 */
    void EquipFirstWeapon() { CheckForWeapon(FIRST); }

    /** \brief 두번째 무기 장착 */
    void EquipSecondWeapon() { CheckForWeapon(SECOND); }

    /** \brief 세번째 무기 장착 */
    void EquipThirdWeapon() { CheckForWeapon(PISTOL); }

    /** \brief 네번째 무기 장착 */
    void EquipFourthWeapon() { CheckForWeapon(MELEE); }

    /** \brief 다섯번째 무기 장착 */
    void EquipFifthWeapon() { CheckForWeapon(THROWABLE); }

    void CheckForWeapon(EWeaponType CurrentWeaponType = NONE, FString Direction = "");

    void ChangePerspective();

    // 조준선을 확대
    void ZoomIn();

    // 조준선을 원위치 시킴
    void ZoomOut();

public:
    UFUNCTION() void DealDmg(float DmgVal);

    UFUNCTION() void SetOtherState(EPlayerOtherState OtherState) { this->CurrentOtherState = OtherState; }

    AWeaponManager* GetWeaponManager() { return mpWeaponManager; }

    ACoreWeapon* GetCurrentWeapon();

    /**
     * \brief 플레이어가 차량에서 나감
     * \param _exit_location 나가는 위치
     */
    void ExitFromVehicle(FVector ExitLocation);
};