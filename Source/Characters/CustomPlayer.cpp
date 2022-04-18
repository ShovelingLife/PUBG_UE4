﻿#include "CustomPlayer.h"
#include "CoreVehicle.h"
#include "CustomGameModeBase.h"
#include "DummyCharacter.h"
#include "Player_weapons/CoreWeapon.h"
#include "Player_weapons/CoreMeleeWeapon.h"
#include "Player_weapons/CoreThrowableWeapon.h"
#include "Player_weapons/CoreBullet.h"
#include "Player_weapons/WeaponManager.h"
#include "AI_PUBG/AI_character.h"
#include "PUBG_UE4/MyEnum.h"
#include "PUBG_UE4/SoundManager.h" 
#include "PUBG_UE4/CustomGameInstance.h" 
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/WidgetComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ACustomPlayer::ACustomPlayer()
{
    PrimaryActorTick.bCanEverTick = true;
    RootComponent = GetCapsuleComponent();
    UpdatePlayerSettings();
    InitCameraComp();
    InitMeshComp();
    InitAudioComp();
    InitAnimInstance();
    InitParticleComp();

    auto bp_DummyCharacter = ConstructorHelpers::FClassFinder<ADummyCharacter>(TEXT("Blueprint'/Game/Blueprints/BP_DummyCharacter.BP_DummyCharacter_C'"));

    if (bp_DummyCharacter.Succeeded())
        BP_DummyCharacter = bp_DummyCharacter.Class;
}

void ACustomPlayer::BeginPlay()
{
    Super::BeginPlay();

    // 무기 매니저 생성
    mpWeaponManager = GetWorld()->SpawnActor<AWeaponManager>(AWeaponManager::StaticClass());

    if (auto p_customGameInst = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance()))
    {
        p_customGameInst->DeleSetPlayerOtherState.BindUFunction(this, "SetOtherState");
        p_customGameInst->DeleDealPlayerDmg.BindUFunction(this, "DealDmg");
    }
    // UI용 캐릭터 생성
    pDummyCharacter = GetWorld()->SpawnActor<ADummyCharacter>(BP_DummyCharacter);

    if (pDummyCharacter)
        pDummyCharacter->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
}

void ACustomPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    CheckIfMoving();
    CheckIfVehicleNear();
    //Play_walk_sound();
    CheckForObject();
    TryToInteract();
    UpdateHealth();
}

void ACustomPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(InputComponent);

    // 이동 관련
    InputComponent->BindAxis(FName(TEXT("UpDown")),    this, &ACustomPlayer::MoveForwardBack);
    InputComponent->BindAxis(FName(TEXT("LeftRight")), this, &ACustomPlayer::MoveLeftRight);
    InputComponent->BindAxis(FName(TEXT("LookUp")),    this, &ACustomPlayer::LookUp);
    InputComponent->BindAxis(FName(TEXT("Turn")),       this, &ACustomPlayer::Turn);

    InputComponent->BindAction(FName(TEXT("Jump")),   IE_Pressed,  this, &ACustomPlayer::CustomJump);
    InputComponent->BindAction(FName(TEXT("Crouch")), IE_Pressed,  this, &ACustomPlayer::CustomCrouch);
    InputComponent->BindAction(FName(TEXT("Sprint")), IE_Pressed,  this, &ACustomPlayer::BeginSprint);
    InputComponent->BindAction(FName(TEXT("Sprint")), IE_Released, this, &ACustomPlayer::EndSprint);
    InputComponent->BindAction(FName(TEXT("Aim")),    IE_Pressed,  this, &ACustomPlayer::Aim);

    // 무기 관련
    InputComponent->BindAction(FName(TEXT("EquipFirstWeapon")),  IE_Pressed,  this, &ACustomPlayer::EquipFirstWeapon);
    InputComponent->BindAction(FName(TEXT("EquipSecondWeapon")), IE_Pressed,  this, &ACustomPlayer::EquipSecondWeapon);
    InputComponent->BindAction(FName(TEXT("EquipThirdWeapon")),  IE_Pressed,  this, &ACustomPlayer::EquipThirdWeapon);
    InputComponent->BindAction(FName(TEXT("EquipFourthWeapon")), IE_Pressed,  this, &ACustomPlayer::EquipFourthWeapon);
    InputComponent->BindAction(FName(TEXT("EquipFifthWeapon")),  IE_Pressed,  this, &ACustomPlayer::EquipFifthWeapon);
    InputComponent->BindAction(FName(TEXT("ChangeShootMode")),   IE_Pressed,  this, &ACustomPlayer::ChangeShootMode);
    InputComponent->BindAction(FName(TEXT("SwapScrollingUp")),   IE_Pressed,  this, &ACustomPlayer::SwapScrollingUp);
    InputComponent->BindAction(FName(TEXT("SwapScrollingDown")), IE_Pressed,  this, &ACustomPlayer::SwapScrollingDown);
    InputComponent->BindAction(FName(TEXT("Shoot")),             IE_Pressed,  this, &ACustomPlayer::BeginShooting);
    InputComponent->BindAction(FName(TEXT("Shoot")),             IE_Released, this, &ACustomPlayer::EndShooting);
    InputComponent->BindAction(FName(TEXT("Reload")),            IE_Pressed,  this, &ACustomPlayer::Reload);
    InputComponent->BindAction(FName(TEXT("Proning")),           IE_Pressed,  this, &ACustomPlayer::Proning);
    InputComponent->BindAction(FName(TEXT("Interact")),          IE_Pressed,  this, &ACustomPlayer::BeginInteract);
    InputComponent->BindAction(FName(TEXT("Interact")),          IE_Released, this, &ACustomPlayer::EndInteract);
    InputComponent->BindAction(FName(TEXT("Inventory")),         IE_Pressed,  this, &ACustomPlayer::OpenInventory);
}

void ACustomPlayer::UpdatePlayerSettings()
{
    // 플레이어 설정
    GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
    GetCharacterMovement()->MaxWalkSpeed = 350.f;
}

void ACustomPlayer::InitCameraComp()
{
    // 카메라 컴포넌트 초기화
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    CameraComp    = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

    // 카메라를 부모 컴포넌트에 부착
    SpringArmComp->SetupAttachment(GetCapsuleComponent());
    CameraComp->SetupAttachment(SpringArmComp);

    // 카메라 설정
    SpringArmComp->TargetArmLength = 150.f;
    SpringArmComp->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));
    SpringArmComp->SetWorldLocation(FVector(0.f, 0.f, 80.f));
    SpringArmComp->bUsePawnControlRotation = true;
}

void ACustomPlayer::InitMeshComp()
{
    // 메쉬 초기화
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/Characters/UE4_Mannequin/Mesh/SK_Mannequin"));

    if (SK_MANNEQUIN.Succeeded())
        GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);

    GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
}

void ACustomPlayer::InitAudioComp()
{
    AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio_comp"));
    AudioComp->SetupAttachment(RootComponent);
}

void ACustomPlayer::InitAnimInstance()
{
    // 애니메이션 초기화
    static ConstructorHelpers::FClassFinder<UAnimInstance> BP_ANIM(TEXT("/Game/Blueprints/Animations/BP_PlayerAnimInstance"));

    if (BP_ANIM.Succeeded())
        GetMesh()->SetAnimInstanceClass(BP_ANIM.Class);
}

void ACustomPlayer::InitParticleComp()
{

}

void ACustomPlayer::CheckIfMoving()
{
    // 움직이고 있지 않음
    if (GetVelocity().IsZero())
    {
        if (mbMoving)
        {
            switch (CurrentState)
            {
            case EPlayerState::CROUCH_WALK:  CurrentState = EPlayerState::CROUCH; break;
            case EPlayerState::PRONING_WALK: CurrentState = EPlayerState::PRONING; break;
            case EPlayerState::AIM_WALK:     CurrentState = EPlayerState::AIM; break;

            default: CurrentState = EPlayerState::IDLE; break;
            }
        }
        mbMoving  = false;
        bSprinting = false;
    }
    else
    {
        switch (CurrentState)
        {
        // 숙이고 있음
        case EPlayerState::CROUCH:
        case EPlayerState::CROUCH_AIM:
            CurrentState = EPlayerState::CROUCH_WALK;
            break;

        case EPlayerState::PRONING: CurrentState = EPlayerState::PRONING_WALK; break;
        case EPlayerState::AIM:     CurrentState = EPlayerState::AIM_WALK; break;

        default:
            // 떨어지고 있음
            if (GetCharacterMovement()->IsFalling())
            {
                // 뛰면서 점프후 착지
                if (mSprintMultiplier > 1.f)
                {
                    bSprinting = true;
                    CurrentState = EPlayerState::SPRINT_JUMP;
                }
                else // 점프함
                    CurrentState = EPlayerState::JUMP;
            }
            else // 지면에 닿고있음
            {
                if (CurrentState == EPlayerState::SPRINT_JUMP)
                    CurrentState = EPlayerState::SPRINT;

                // 뛰고있음
                else if (CurrentState == EPlayerState::SPRINT)
                {
                    if (CurrentOxygen < 0)
                    {
                        CurrentState = EPlayerState::IDLE;
                        mSprintMultiplier = 1;
                        GetCharacterMovement()->MaxWalkSpeed = 350.f;
                        bSprinting = false;
                        return;
                    }
                    bSprinting = true;

                    if (mSprintMultiplier < 1.75f)
                    {
                        mSprintMultiplier += 0.25f;
                        GetCharacterMovement()->MaxWalkSpeed *= mSprintMultiplier;
                    }
                }
                // 
                else if (CurrentState == EPlayerState::IDLE ||
                    CurrentState == EPlayerState::JUMP)
                    CurrentState = EPlayerState::WALK;
            }
            break;
        }
        mbMoving = true;
    }
}

void ACustomPlayer::CheckForObject()
{
    FVector    direction = GetActorForwardVector() * 50;
    FVector    beginPos  = GetMesh()->GetSocketLocation("DetectObjectRaySock");
    FVector    endPos    = beginPos + direction;
    FHitResult hitResult;
    bool       b_collided = false;
    
    GetWorld()->LineTraceSingleByProfile(hitResult, beginPos, endPos, "Object");
    DrawDebugLine(GetWorld(), beginPos, endPos,FColor::Red);
    AActor* p_hittedActor = hitResult.GetActor();

    // 충돌한 오브젝트가 있을 시
    if (p_hittedActor)
    {
        // 충돌한 오브젝트가 무기일 시
        if (p_hittedActor->IsA<ACoreWeapon>()          ||
            p_hittedActor->IsA<ACoreThrowableWeapon>() ||
            p_hittedActor->IsA<ACoreMeleeWeapon>())
        {
            b_collided = true;
            mpCollidedWeapon = p_hittedActor;
        }
        if (auto p_obj = Cast<ABaseInteraction>(p_hittedActor))
            p_obj->bPlayerNear = true;
    }
    else
    {
        // 무기 접근 후 벗어남
        if (mpCollidedWeapon)
        {
            Cast<ABaseInteraction>(mpCollidedWeapon)->bPlayerNear = false;
            mpCollidedWeapon = nullptr;
        }
    }
}

void ACustomPlayer::CheckIfVehicleNear()
{
    if (bInVehicle)
        return;

    APlayerCameraManager* p_cameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
    FVector               beginPos      = GetActorLocation();
    FVector               forwardVec    = GetActorForwardVector() * 50;
    FHitResult            hitResult;
    FVector               endPos        = beginPos + forwardVec;
    GetWorld()->LineTraceSingleByObjectType(hitResult, beginPos, endPos, FCollisionObjectQueryParams(ECC_Vehicle));
    //DrawDebugLine(p_world, begin_pos, end_pos, FColor::Red, true, 5.f, (uint8)0U, 1.f);

    // 차량을 감지
    AActor* pHittedActor = hitResult.GetActor();

    if (!pHittedActor)
    {
        if (mpCollidedVehicle)
        {
            mpCollidedVehicle->bCollided = false;
            mpCollidedVehicle            = nullptr;
        }
        return;
    }
    if (pHittedActor->IsA(ACoreVehicle::StaticClass()))
    {
        mpCollidedVehicle = Cast<ACoreVehicle>(pHittedActor);
        
        if (mpCollidedVehicle)
            mpCollidedVehicle->bCollided = true;
    }
}

void ACustomPlayer::TryToInteract()
{
    if (mbInteracting)
    {
        // 무기랑 충돌 시
        if (mpCollidedWeapon)
        {
            if(auto p_customGameInst = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
            {
                if(auto p_soundManager = p_customGameInst->pSoundManager)
                    p_soundManager->PlayPlayerSound(AudioComp, EPlayerSoundType::WEAPON_EQUIP);
            }
            mpWeaponManager->Equip(mpCollidedWeapon);
        }
        if (mpCollidedVehicle)
        {
            //  차량 탑승 상태
            if (mpCollidedVehicle->IsSeatAvailable(this))
            {
                mpCollidedVehicle->bCollided = false;
                bInVehicle = true;
            }
            mpCollidedVehicle = nullptr;
        }
    }
}

void ACustomPlayer::MoveForwardBack(float Value)
{
    if (!bAnimationPlaying)
    {
        if (CurrentState == EPlayerState::INJURED)
            AddMovementInput(GetActorForwardVector() * (Value / 4));

        else
            AddMovementInput(GetActorForwardVector() * Value * mSprintMultiplier);
    }
    mpWeaponManager->UpdateGrenadePath();
}

void ACustomPlayer::MoveLeftRight(float Value)
{
    if (!bAnimationPlaying)
    {
        if (CurrentState == EPlayerState::INJURED)
            AddMovementInput(GetActorRightVector() * (Value / 4));
            
        else
            AddMovementInput(GetActorRightVector() * Value * mSprintMultiplier);
    }
    mpWeaponManager->UpdateGrenadePath();
}

void ACustomPlayer::LookUp(float Value)
{
    if (!mbInventoryOpened)
        AddControllerPitchInput(Value);

    // 경로 거리 설정
    auto grenadeDirection = mpWeaponManager->GrenadeDirection;
    auto target = (Value / 10.f) + grenadeDirection;
    auto interptVal = UKismetMathLibrary::FInterpTo(grenadeDirection, target, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 50.f);
    
    if (interptVal < 0.7f &&
        interptVal > -0.1f)
        mpWeaponManager->GrenadeDirection = interptVal;
    
    mpWeaponManager->UpdateGrenadePath();
}

void ACustomPlayer::Turn(float _Value)
{
    if (!mbInventoryOpened)
        AddControllerYawInput(_Value);

    mpWeaponManager->UpdateGrenadePath();
}

void ACustomPlayer::CustomJump()
{
    if (CurrentState == EPlayerState::CROUCH ||
        CurrentState == EPlayerState::PRONING ||
        CurrentState == EPlayerState::INJURED)
        return;

    Jump();
    mpWeaponManager->UpdateGrenadePath();
}

void ACustomPlayer::CustomCrouch()
{
    if (mbMoving ||
        GetCharacterMovement()->IsFalling())
        return;

    SpringArmComp->SetRelativeLocation(FVector(0.f, 0.f, 80.f));

    // 숙이고 있음
    if (GetCharacterMovement()->IsCrouching())
    {
        CurrentState = (CurrentState == EPlayerState::CROUCH_AIM) ? EPlayerState::AIM : EPlayerState::IDLE;
        UnCrouch();
    }
    else
    {
        CurrentState = (CurrentState == EPlayerState::AIM) ? EPlayerState::CROUCH_AIM : EPlayerState::CROUCH;
        Crouch();
    }
}

void ACustomPlayer::Proning()
{
    FVector locationVal = FVector::ZeroVector;

    switch (CurrentState)
    {
    case EPlayerState::AIM:  CurrentState = EPlayerState::PRONING_AIM; break;

    case EPlayerState::IDLE: 
    case EPlayerState::CROUCH:
    case EPlayerState::CROUCH_WALK:

        if (CurrentState != EPlayerState::IDLE)
            UnCrouch();

        CurrentState = EPlayerState::PRONING; 
        break;

    case EPlayerState::PRONING: 
    case EPlayerState::PRONING_WALK:
        CurrentState = EPlayerState::IDLE; break;

    default: return;
    }
    SpringArmComp->SetRelativeLocation((CurrentState == EPlayerState::PRONING || CurrentState == EPlayerState::PRONING_AIM) ? FVector(0.f, 0.f, -50.f) : FVector(0.f, 0.f, 80.f));
}

void ACustomPlayer::BeginSprint()
{
    if (!GetVelocity().IsZero() &&
        CurrentState != EPlayerState::INJURED)
        CurrentState = EPlayerState::SPRINT;
}

void ACustomPlayer::EndSprint()
{
    CurrentState                         = EPlayerState::IDLE;
    mSprintMultiplier                    = 1;
    GetCharacterMovement()->MaxWalkSpeed = 350.f;
}

void ACustomPlayer::OpenInventory()
{
    if (!mbInventoryOpened)
    {
        DeleOpenInventory.ExecuteIfBound();
        mbInventoryOpened = true;
    }
    else
    {
        SpringArmComp->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));
        DeleCloseInventory.ExecuteIfBound();
        mbInventoryOpened = false;
    }
}

void ACustomPlayer::UpdateHealth()
{
    if (CurrentHealth == 0.f &&
        CurrentState != EPlayerState::DEAD)
        CurrentState = EPlayerState::INJURED;

    if (CurrentInjuredHealth == 0.f)
        CurrentState = EPlayerState::DEAD;
}

void ACustomPlayer::BeginShooting()
{
    if (mbInventoryOpened)
        return;

    mpWeaponManager->bShooting = true;
    mpWeaponManager->Shoot();
}

void ACustomPlayer::EndShooting()
{
    if (mbInventoryOpened)
        return;

    mpWeaponManager->bShooting = false;

    // 투척류 무기일 시 뗐을 때만 발동
    if (mpWeaponManager->bArrWeaponEquipped[4])
        mpWeaponManager->ThrowGrenade();
}

void ACustomPlayer::Reload()
{
}

void ACustomPlayer::Aim()
{
    bool b_firstEquipped  = false;
    bool b_secondEquipped = false;

    // 무기 장착하고 있지 않은 상태
    if (!b_firstEquipped &&
        !b_secondEquipped)
        return;

    switch (CurrentState)
    {
    // 기본자세 > 에임
    case EPlayerState::IDLE:        CurrentState = EPlayerState::AIM;         bAiming = true;  break;
    case EPlayerState::CROUCH:      CurrentState = EPlayerState::CROUCH_AIM;  bAiming = true;  break;
    case EPlayerState::PRONING:     CurrentState = EPlayerState::PRONING_AIM; bAiming = true;  break;

    // 에임 > 기본 자세
    case EPlayerState::AIM:         CurrentState = EPlayerState::IDLE;        bAiming = false; break;
    case EPlayerState::CROUCH_AIM:  CurrentState = EPlayerState::CROUCH;      bAiming = false; break;
    case EPlayerState::PRONING_AIM: CurrentState = EPlayerState::PRONING;     bAiming = false; break;
    }
    mpWeaponManager->ChangeAimPose((int)bAiming);
}

void ACustomPlayer::ChangeShootMode()
{
    mpWeaponManager->ChangeShootMode();
}

void ACustomPlayer::CheckForWeapon(FString Direction /* = "" */, ECurrentWeaponType WeaponType /* = ECurrentWeaponType::NONE */)
{
    auto p_customGameInst = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    bool bPlayAudio = false;

    if (!p_customGameInst)
        return;

    if (mpWeaponManager)
    {
        if (auto p_soundManager = p_customGameInst->pSoundManager)
        {
            // 마우스 휠로 무기 선택
            if      (Direction != "")
                     bPlayAudio = mpWeaponManager->ScrollSelect(Direction);

            // 키보드 숫자 키로 무기 선택
            else if ((int)WeaponType > 0)
                     bPlayAudio = mpWeaponManager->IsWeaponAvailable((WeaponType));

            if (bPlayAudio)
                p_soundManager->PlayPlayerSound(AudioComp, EPlayerSoundType::WEAPON_SWAP);
        }
    }
}

void ACustomPlayer::ExitFromVehicle(FVector ExitPos)
{
    // 플레이어 충돌체 관련
    auto capsuleComp = GetCapsuleComponent();
    capsuleComp->SetMobility(EComponentMobility::Movable);
    capsuleComp->SetCollisionProfileName("Pawn");
    capsuleComp->SetEnableGravity(true);

    // 플레이어 위치 및 카메라 위치
    CurrentSeatType = ESeatType::NONE;
    bInVehicle      = false;
    SetActorLocation(ExitPos);
    SpringArmComp->SetRelativeLocation(FVector(0.f, 0.f, 80.f));
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void ACustomPlayer::DealDmg(float DmgVal)
{
    // 예시) 현재 HP 50 / 받은 데미지 75 = 추가 HP 25만큼 차감
    auto extraDmgVal = CurrentHealth - DmgVal;

    if(extraDmgVal < 0)
    {
        CurrentHealth = 0.f;
        CurrentInjuredHealth -= extraDmgVal;
    }
    else
        CurrentHealth -= DmgVal;
}
