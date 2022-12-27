#include "CustomPlayer.h"
#include "CoreVehicle.h"
#include "CustomGameModeBase.h"
#include "DummyCharacter.h"
#include "TimerManager.h"
#include "AI_PUBG/AI_character.h"
#include "Player_weapons/CoreWeapon.h"
#include "Player_weapons/CoreMeleeWeapon.h"
#include "Player_weapons/CoreThrowableWeapon.h"
#include "Player_weapons/WeaponManager.h"
#include "Farmable_items/CoreAmmoBox.h"
#include "Farmable_items/CoreAttachment.h"
#include "Farmable_items/CoreFarmableItem.h"
#include "PUBG_UE4/MyEnum.h"
#include "PUBG_UE4/SoundManager.h" 
#include "PUBG_UE4/CustomGameInstance.h" 
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
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

ACustomPlayer::ACustomPlayer()
{
    PrimaryActorTick.bCanEverTick = true;
    RootComponent = GetCapsuleComponent();
    InitPlayerSettings();
    InitCameraComp();
    InitMeshComp();
    InitAudioComp();
    InitAnimInstance();
    InitParticleComp();

    // 캐릭터 UI 업데이트
    auto bp_DummyCharacter = ConstructorHelpers::FClassFinder<ADummyCharacter>(TEXT("Blueprint'/Game/1_Blueprints/BP_DummyCharacter.BP_DummyCharacter_C'"));

    if (bp_DummyCharacter.Succeeded())
        BP_DummyCharacter = bp_DummyCharacter.Class;
}

void ACustomPlayer::BeginPlay()
{
    Super::BeginPlay();

    // 무기 매니저 생성
    mpWeaponManager = GetWorld()->SpawnActor<AWeaponManager>(AWeaponManager::StaticClass());
    mpWeaponManager->DeleAim.BindUFunction(this, "TryToAim");
    mpCustomGameInst = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance());

    if (mpCustomGameInst)
    {
        mpCustomGameInst->DeleSetPlayerOtherState.BindUFunction(this, "SetOtherState");
        mpCustomGameInst->DeleDealPlayerDmg.BindUFunction(this, "DealDmg");
    }
    // UI용 캐릭터 생성
    pDummyCharacter = GetWorld()->SpawnActor<ADummyCharacter>(BP_DummyCharacter);
}

void ACustomPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    CheckIfMoving();
    CheckNearVehicle();
    CheckNearObj();
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
    InputComponent->BindAxis(FName(TEXT("Turn")),      this, &ACustomPlayer::Turn);

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

void ACustomPlayer::InitPlayerSettings()
{
    // 플레이어 설정
    auto characterMovement = GetCharacterMovement();
    characterMovement->GetNavAgentPropertiesRef().bCanCrouch = true;
    characterMovement->MaxWalkSpeed = 350.f;
}

void ACustomPlayer::InitCameraComp()
{
    // ------- FPS (1인칭) -------
    FPS_SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("FPS_SpringArm"));
    FPS_CameraComp    = CreateDefaultSubobject<UCameraComponent>(TEXT("FPS_Camera"));
    FPS_SpringArmComp->SetupAttachment(GetMesh(), "FPS_CameraSocket");
    FPS_CameraComp->SetupAttachment(FPS_SpringArmComp);
    FPS_SpringArmComp->TargetArmLength = 0.f;
    FPS_SpringArmComp->bUsePawnControlRotation = true;
    FPS_CameraComp->SetAutoActivate(true);

    // ------- TPS (3인칭) -------        
    TPS_SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPS_SpringArm"));
    TPS_CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TPS_Camera"));
    TPS_SpringArmComp->SetupAttachment(GetMesh());
    TPS_CameraComp->SetupAttachment(TPS_SpringArmComp);
    TPS_SpringArmComp->TargetArmLength = 150.f;
    TPS_SpringArmComp->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 80.f), FRotator(-20.f, 0.f, 0.f));
    TPS_SpringArmComp->bUsePawnControlRotation = true;
    TPS_SpringArmComp->Deactivate();
}

void ACustomPlayer::InitMeshComp()
{
    // 메쉬 초기화
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/2_Meshes/UE4_Mannequin/Mesh/SK_Mannequin"));

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
    static ConstructorHelpers::FClassFinder<UAnimInstance> BP_ANIM(TEXT("/Game/1_Blueprints/Animations/BP_PlayerAnimInstance"));

    if (BP_ANIM.Succeeded())
        GetMesh()->SetAnimInstanceClass(BP_ANIM.Class);
}

void ACustomPlayer::InitParticleComp()
{

}

void ACustomPlayer::CheckIfMoving()
{
    auto characterMovementComp = GetCharacterMovement();    
    //GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, GetVelocity().ToString());
    
    // 움직이고 있음
    if (characterMovementComp->Velocity.Size() > 0.f)
    {
        switch (CurrentState)
        {
            // 숙이고 있음
        case CROUCH:
        case CROUCH_AIM:
            CurrentState = CROUCH_WALK;
            break;

        case PRONING: CurrentState = PRONING_WALK; break;
        case AIM:     CurrentState = AIM_WALK;     break;

        default:
            // 뛰면서 점프후 착지
            if (characterMovementComp->IsFalling())
                CurrentState = (mSprintMultiplier > 1.f) ? SPRINT_JUMP : JUMP;

            else // 지면에 닿고있음
            {
                if (CurrentState == SPRINT_JUMP)
                    CurrentState = SPRINT;

                // 뛰고있음
                else if (CurrentState == SPRINT)
                {
                    if (CurrentOxygen < 0)
                    {
                        CurrentState = IDLE;
                        mSprintMultiplier = 1;
                        characterMovementComp->MaxWalkSpeed = 350.f;
                    }
                    if (mSprintMultiplier < 1.75f)
                    {
                        mSprintMultiplier += 0.25f;
                        characterMovementComp->MaxWalkSpeed *= mSprintMultiplier;
                    }
                }
                // 
                else if (CurrentState == IDLE ||
                    CurrentState == JUMP)
                    CurrentState = WALK;
            }
            break;
        }
        mbMoving = true;
    }
    else
    {
        if (mbMoving)
        {
            switch (CurrentState)
            {
            case CROUCH_WALK:  CurrentState = CROUCH;  break;
            case PRONING_WALK: CurrentState = PRONING; break;
            case AIM_WALK:     CurrentState = AIM;     break;

            default: CurrentState = IDLE; break;
            }
        }
        mbMoving = false;       
    }
}

void ACustomPlayer::CheckNearObj()
{
    FVector    direction = GetActorForwardVector() * 100;
    FVector    beginPos  = GetMesh()->GetSocketLocation("DetectObjectRaySock");
    FVector    endPos    = beginPos + direction;
    FHitResult hitResult;
    
    GetWorld()->LineTraceSingleByProfile(hitResult, beginPos, endPos, "Object");
    AActor* p_hittedActor = hitResult.GetActor();

    if (!p_hittedActor)
        return;

    // 충돌한 오브젝트가 상호작용 가능한 오브젝트일 시
    if (p_hittedActor->IsA<ABaseInteraction>())
        mpCollidedItem = Cast<ABaseInteraction>(p_hittedActor);
}

void ACustomPlayer::CheckNearVehicle()
{
    if (bInVehicle)
        return;

    FVector    beginPos   = GetActorLocation();
    FVector    forwardVec = GetActorForwardVector() * 50;
    FVector    endPos     = beginPos + forwardVec;
    FHitResult hitResult;
    GetWorld()->LineTraceSingleByObjectType(hitResult, beginPos, endPos, FCollisionObjectQueryParams(ECC_Vehicle));

}

void ACustomPlayer::TryToInteract()
{
    if (!mpCustomGameInst ||
        !mpCollidedItem   ||
        !mbInteracting)
        return;

    // 충돌된 오브젝트가 무기일 시
    if (mpWeaponManager->TryToEquip(mpCollidedItem))
    {
        if (auto p_soundManager = mpCustomGameInst->pSoundManager)
            p_soundManager->PlayPlayerSound(AudioComp, EPlayerSoundType::WEAPON_EQUIP);

        mpCollidedItem = nullptr;
        return;
    }
    // 획득 가능한 오브젝트일 시
    else if (mpCollidedItem->IsA<ACoreFarmableItem>())
    {
        // 인벤토리에 추가한 뒤 맵에서 제거
        mpCustomGameInst->DeleSetItemOntoInventory.ExecuteIfBound(mpCollidedItem, false);
        mpCollidedItem->Destroy();
        mpCollidedItem = nullptr;
    }
}

void ACustomPlayer::CustomJump()
{
    if (CurrentState == CROUCH  ||
        CurrentState == PRONING ||
        CurrentState == INJURED)
        return;

    Jump();
    mpWeaponManager->UpdateGrenadePath();
}

void ACustomPlayer::CustomCrouch()
{
    auto characterMovement = GetCharacterMovement();

    if (mbMoving ||
        characterMovement->IsFalling())
        return;

    TPS_SpringArmComp->SetRelativeLocation(FVector(0.f, 0.f, 80.f));

    // 숙이고 있음
    if (characterMovement->IsCrouching())
    {
        CurrentState = (CurrentState == CROUCH_AIM) ? AIM : IDLE;
        UnCrouch();
    }
    else
    {
        CurrentState = (CurrentState == AIM) ? CROUCH_AIM : CROUCH;
        Crouch();
    }
}

void ACustomPlayer::Proning()
{
    // 숙이고 있는지 체크
    switch (CurrentState)
    {
    case IDLE: case CROUCH: case CROUCH_WALK:

        if (CurrentState != IDLE)
            UnCrouch();

        CurrentState = PRONING; 
        break;

    case PRONING: case PRONING_WALK: CurrentState = IDLE; break;

    case AIM:  CurrentState = PRONING_AIM; break;

    default: return;
    }
    bool bProwning = (CurrentState == PRONING || CurrentState == PRONING_AIM);
    TPS_SpringArmComp->SetRelativeLocation(FVector(0.f, 0.f, bProwning  ? -50.f : 80.f));
}

void ACustomPlayer::BeginSprint()
{
    if (!GetVelocity().IsZero() &&
        CurrentState != INJURED)
        CurrentState = SPRINT;
}

void ACustomPlayer::EndSprint()
{
    CurrentState = IDLE;
    mSprintMultiplier = 1;
    GetCharacterMovement()->MaxWalkSpeed = 350.f;
}

void ACustomPlayer::MoveForwardBack(float Value)
{
    if (!bAnimationPlaying)
        AddMovementInput(GetActorForwardVector() * ((CurrentState == INJURED) ? (Value / 4) : (Value * mSprintMultiplier)));

    mpWeaponManager->UpdateGrenadePath();
}

void ACustomPlayer::MoveLeftRight(float Value)
{
    if (!bAnimationPlaying)
        AddMovementInput(GetActorRightVector() * ((CurrentState == INJURED) ? (Value / 4) : (Value * mSprintMultiplier)));

    mpWeaponManager->UpdateGrenadePath();
}

void ACustomPlayer::LookUp(float Value)
{
    if (!mbInventoryOpened)
        AddControllerPitchInput(Value);

    // 경로 거리 설정
    auto grenadeDir = mpWeaponManager->GrenadeDirection;
    auto target = (Value / 10.f) + grenadeDir;
    auto interptVal = UKismetMathLibrary::FInterpTo(grenadeDir, target, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 50.f);
    mpWeaponManager->GrenadeDirection = interptVal;
    mpWeaponManager->UpdateGrenadePath();
}

void ACustomPlayer::Turn(float _Value)
{
    if (!mbInventoryOpened)
        AddControllerYawInput(_Value);

    mpWeaponManager->UpdateGrenadePath();
}

void ACustomPlayer::OpenInventory()
{
    if (mpWeaponManager->IsFiring())
        return;

    if (!mbInventoryOpened)
    {
        DeleOpenInventory.ExecuteIfBound();
        mbInventoryOpened = true;
    }
    else
    {
        DeleCloseInventory.ExecuteIfBound();
        mbInventoryOpened = false;
        TPS_SpringArmComp->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));
    }
}

void ACustomPlayer::UpdateHealth()
{
    if (CurrentHealth == 0.f && 
        CurrentState  != DEAD)
        CurrentState = INJURED;

    if (CurrentInjuredHealth == 0.f)
        CurrentState = DEAD;
}

void ACustomPlayer::BeginShooting()
{
    if (mbInventoryOpened)
        return;

    // 연사일 때만 한번 발 사
    // 투척류일 시 경로 예측
    if (mpWeaponManager->GetCurrentGun() ||
        mpWeaponManager->CurrentWeaponType == THROWABLE)
        mpWeaponManager->ClickEvent();
}

void ACustomPlayer::EndShooting()
{
    if (mbInventoryOpened)
        return;

    // 투척류 무기일 시 뗐을 때만 발동    
    if (mpWeaponManager->CurrentWeaponType == THROWABLE)
        mpWeaponManager->ThrowGrenade();

    mpWeaponManager->DeactivateFiring();
}

void ACustomPlayer::Reload()
{
}

void ACustomPlayer::Aim()
{
    auto currentWeaponType = mpWeaponManager->CurrentWeaponType;

    if (currentWeaponType == MELEE ||
        currentWeaponType == THROWABLE)
        return;

    // 현재 착용 중인 무기를 가지고옴
    ACoreWeapon* p_gun = mpWeaponManager->GetCurrentGun();
    bool bAiming = false;

    switch (CurrentState)
    {
    // 기본자세 > 에임
    case IDLE:        CurrentState = AIM;         bAiming = true;  break;
    case CROUCH:      CurrentState = CROUCH_AIM;  bAiming = true;  break;
    case PRONING:     CurrentState = PRONING_AIM; bAiming = true;  break;

    // 에임 > 기본 자세
    case AIM:         CurrentState = IDLE;    break;
    case CROUCH_AIM:  CurrentState = CROUCH;  break;
    case PRONING_AIM: CurrentState = PRONING; break;
    }
    if (!p_gun)
    {
        // 총기 카메라 위치 > 플레이어 카메라 위치
        GetWorld()->GetTimerManager().SetTimer(AimTimerHandle, this, &ACustomPlayer::ZoomOut, 0.01f, true);
        return;
    }
    // 캐릭터 메쉬에다 부착
    auto p_player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    p_gun->AttachToMesh(GetMesh(), bAiming ? "EquippedWeaponPosSock" : TArray<FString>{ "FirstGunSock", "SecondGunSock", "HandGunSock" } [(int)mpWeaponManager->CurrentWeaponType - 1] );

    // 플레이어 카메라 위치 > 총기 카메라 위치
    if (bAiming)
        GetWorld()->GetTimerManager().SetTimer(AimTimerHandle, this, &ACustomPlayer::ZoomIn, 0.01f, true);

    // 총기 카메라 위치 > 플레이어 카메라 위치
    else
        GetWorld()->GetTimerManager().SetTimer(AimTimerHandle, this, &ACustomPlayer::ZoomOut, 0.01f, true);
}

void ACustomPlayer::TryToAim()
{
    // 조준 하고 있을 시 해제
    if (CurrentState == AIM        ||
        CurrentState == CROUCH_AIM ||
        CurrentState == PRONING_AIM)
        Aim();
}

void ACustomPlayer::ChangeShootMode() { mpWeaponManager->ChangeShootMode(); }

void ACustomPlayer::CheckForWeapon(EWeaponType WeaponType /* = ECurrentWeaponType::NONE */, FString Direction /* = "" */)
{    
    if (mpWeaponManager->IsFiring())
        return;

    bool bChanged = false;

    // 마우스 휠로 무기 선택
    if (Direction != "")
        bChanged = mpWeaponManager->ScrollSelect(Direction);

    // 키보드 숫자 키로 무기 선택
    else
        bChanged = mpWeaponManager->Swap(WeaponType);

    if (bChanged)
        mpCustomGameInst->pSoundManager->PlayPlayerSound(AudioComp, EPlayerSoundType::WEAPON_SWAP);
}

void ACustomPlayer::ZoomIn()
{
    auto fieldOfView = FPS_CameraComp->FieldOfView;

    if (fieldOfView <= 45.f)
    {
        GetWorld()->GetTimerManager().ClearTimer(AimTimerHandle);
        return;
    }
    FPS_CameraComp->SetFieldOfView(--fieldOfView);
}

void ACustomPlayer::ZoomOut()
{
    auto fieldOfView = FPS_CameraComp->FieldOfView;

    if (fieldOfView >= 90.f)
    {
        GetWorld()->GetTimerManager().ClearTimer(AimTimerHandle);
        return;
    }
    FPS_CameraComp->SetFieldOfView(++fieldOfView);
}

// UFUNCTION()
void ACustomPlayer::DealDmg(float DmgVal)
{
    // 예시) 현재 HP 50 / 받은 데미지 75 = 추가 HP 25만큼 차감
    auto extraDmgVal = CurrentHealth - DmgVal;

    if (extraDmgVal < 0)
    {
        CurrentHealth = 0.f;
        CurrentInjuredHealth -= extraDmgVal;
    }
    else
        CurrentHealth -= DmgVal;
}

ACoreWeapon* ACustomPlayer::GetCurrentWeapon() { return mpWeaponManager->GetCurrentGun(); }

void ACustomPlayer::ExitFromVehicle(FVector ExitPos)
{
    // 플레이어 충돌체 관련
    auto capsuleComp = GetCapsuleComponent();
    capsuleComp->SetMobility(EComponentMobility::Movable);
    capsuleComp->SetCollisionProfileName("Pawn");
    capsuleComp->SetEnableGravity(true);

    // 플레이어 위치 및 카메라 위치
    CurrentSeatType = ESeatType::NONE;
    bInVehicle = false;
    SetActorLocation(ExitPos);
    TPS_SpringArmComp->SetRelativeLocation(FVector(0.f, 0.f, 80.f));
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}