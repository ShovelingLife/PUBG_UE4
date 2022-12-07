#include "CoreVehicle.h"
#include "CustomPlayer.h"
#include "InstaDeformComponent.h"
#include "Animation/AnimInstance.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "PUBG_UE4/DataTableManager.h"
#include "Camera/CameraComponent.h"
#include "ChaosVehicleMovementComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ACoreVehicle::ACoreVehicle()
{
    PrimaryActorTick.bCanEverTick = true;
    AutoPossessAI = EAutoPossessAI::Disabled;

    for (int i = 0; i < (int)ESeatType::NONE; i++)
         mMapEmptySeat.Add({ (ESeatType)i, false });
}

void ACoreVehicle::BeginPlay()
{
    Super::BeginPlay();
    
    // 상호작용 UI 업데이트
    if (auto p_customGameInst = UCustomGameInstance::GetInst())
        p_customGameInst->DeleSetInteractionWidgetComp.ExecuteIfBound(InteractionWidgetComp, FString::Printf(TEXT("%s 탑승하기"), *mVehicleData.Type));
}

void ACoreVehicle::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateCarPosData();

    if (mpPlayer)
        mbPlayerInFirstSeat = (mpPlayer->CurrentSeatType == FIRST);

    if (InteractionWidgetComp)
        InteractionWidgetComp->SetVisibility(bCollided);
}

void ACoreVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComp)
{    
    Super::SetupPlayerInputComponent(InputComponent);

    // 운전석
    InputComponent->BindAxis(TEXT("SetThrottle"), this, &ACoreVehicle::Accelerate);
    InputComponent->BindAxis(TEXT("SetBrake"),    this, &ACoreVehicle::Brake);
    InputComponent->BindAxis(TEXT("SetSteering"), this, &ACoreVehicle::Handling);
    InputComponent->BindAxis(TEXT("LookUp"),      this, &ACoreVehicle::LookUp);
    InputComponent->BindAxis(TEXT("Turn"),        this, &ACoreVehicle::Turn);

    InputComponent->BindAction(TEXT("Interact"),           EInputEvent::IE_Pressed, this, &ACoreVehicle::PlayerExit);
    InputComponent->BindAction(TEXT("ChangeToFirstSeat"),  EInputEvent::IE_Pressed, this, &ACoreVehicle::ChangeToFirstSeat);
    InputComponent->BindAction(TEXT("ChangeToSecondSeat"), EInputEvent::IE_Pressed, this, &ACoreVehicle::ChangeToSecondSeat);
    InputComponent->BindAction(TEXT("ChangeToThirdSeat"),  EInputEvent::IE_Pressed, this, &ACoreVehicle::ChangeToThirdSeat);
    InputComponent->BindAction(TEXT("ChangeToFourthSeat"), EInputEvent::IE_Pressed, this, &ACoreVehicle::ChangeToFourthSeat);
}

void ACoreVehicle::Init(EVehicleType VehicleTypeIndex)
{
    mVehicleData = ADataTableManager::GetVehicleData((int)VehicleTypeIndex);
    InstaDeformComp = CreateDefaultSubobject<UInstaDeformComponent>(TEXT("InstaDeformComp"));
    InitSkeletalMesh();
    InitCamera();
    InitWheeledComp();
    InitCarPosComp();
    InitCarPosData();
}

void ACoreVehicle::InitCarPosComp()
{
    // 문짝 오브젝트 초기화
    FirstDoorPosComp = CreateDefaultSubobject<USceneComponent>(TEXT("FirstDoorPos"));
    SecondDoorPosComp = CreateDefaultSubobject<USceneComponent>(TEXT("SecondDoorPos"));
    ThirdDoorPosComp = CreateDefaultSubobject<USceneComponent>(TEXT("ThirdDoorPos"));
    FourthDoorPosComp = CreateDefaultSubobject<USceneComponent>(TEXT("FourthDoorPos"));

    FirstDoorPosComp->SetupAttachment(RootComponent);
    SecondDoorPosComp->SetupAttachment(RootComponent);
    ThirdDoorPosComp->SetupAttachment(RootComponent);
    FourthDoorPosComp->SetupAttachment(RootComponent);

    // 좌석 오브젝트 초기화
    FirstSeatPosComp  = CreateDefaultSubobject<USceneComponent>(TEXT("FirstSeatPos"));
    SecondSeatPosComp = CreateDefaultSubobject<USceneComponent>(TEXT("SecondSeatPos"));
    ThirdSeatPosComp  = CreateDefaultSubobject<USceneComponent>(TEXT("ThirdSeatPos"));
    FourthSeatPosComp = CreateDefaultSubobject<USceneComponent>(TEXT("FourthSeatPos"));

    FirstSeatPosComp->SetupAttachment(RootComponent);
    SecondSeatPosComp->SetupAttachment(RootComponent);
    ThirdSeatPosComp->SetupAttachment(RootComponent);
    FourthSeatPosComp->SetupAttachment(RootComponent);
}

void ACoreVehicle::InitCarPosData()
{
    MapDoorPos.Add(FIRST,  FirstDoorPosComp->GetComponentLocation());
    MapSeatPos.Add(FIRST,  FirstSeatPosComp->GetComponentLocation());
    MapDoorPos.Add(SECOND, SecondDoorPosComp->GetComponentLocation());
    MapSeatPos.Add(SECOND, SecondSeatPosComp->GetComponentLocation());

    // 문 4개있는 차량일 시
    if(mVehicleData.MaxSeater == 4)
    {
        MapDoorPos.Add(THIRD,  ThirdDoorPosComp->GetComponentLocation());
        MapSeatPos.Add(THIRD,  ThirdSeatPosComp->GetComponentLocation());
        MapDoorPos.Add(FOURTH, FourthDoorPosComp->GetComponentLocation());
        MapSeatPos.Add(FOURTH, FourthSeatPosComp->GetComponentLocation());
    }
}

void ACoreVehicle::UpdateCarPosData()
{
    // 좌석 및 문짝 위치 업데이트
    MapDoorPos[FIRST]  = FirstDoorPosComp->GetComponentLocation();
    MapSeatPos[FIRST]  = FirstSeatPosComp->GetComponentLocation();
    MapDoorPos[SECOND] = SecondDoorPosComp->GetComponentLocation();
    MapSeatPos[SECOND] = SecondSeatPosComp->GetComponentLocation();

    if (mVehicleData.MaxSeater == 4)
    {
        MapDoorPos[THIRD]  = ThirdDoorPosComp->GetComponentLocation();
        MapSeatPos[THIRD]  = ThirdSeatPosComp->GetComponentLocation();
        MapDoorPos[FOURTH] = FourthDoorPosComp->GetComponentLocation();
        MapSeatPos[FOURTH] = FourthSeatPosComp->GetComponentLocation();
    }
}

void ACoreVehicle::InitCamera()
{
    // 스프링 암 초기화 및 설정
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(GetMesh());
    SpringArmComp->bUsePawnControlRotation = true;

    // 카메라 초기화 및 설정
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp);
    CameraComp->SetRelativeRotation(FRotator::MakeFromEuler(mVehicleData.CameraRotation));
    CameraComp->SetWorldLocation(mVehicleData.CameraLocation);
}

void ACoreVehicle::InitSkeletalMesh()
{
    // 경로로부터 메시 생성
    ConstructorHelpers::FObjectFinder<USkeletalMesh> MESH(*mVehicleData.MeshPath);

    if (MESH.Succeeded())
        GetMesh()->SetSkeletalMesh(MESH.Object);

    this->SetRootComponent(GetMesh());
    GetMesh()->SetSimulatePhysics(true);

    // 애님 인스턴스 초기화
    auto animInst = ConstructorHelpers::FClassFinder<UAnimInstance>(*mVehicleData.AnimInstancePath);

    if (animInst.Succeeded())
        GetMesh()->SetAnimInstanceClass(animInst.Class);

    // 위젯 컴포넌트 초기화
    InteractionWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
    InteractionWidgetComp->SetupAttachment(RootComponent);
}

void ACoreVehicle::InitWheeledComp()
{
    UChaosWheeledVehicleMovementComponent* vehicleMoveComp = CastChecked<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());
     
    // 바퀴 설정
    auto bpFrontWheel = ConstructorHelpers::FClassFinder<UChaosVehicleWheel>(*(mkWheelPath + "BP_FrontWheel"));
    auto bpRearWheel  = ConstructorHelpers::FClassFinder<UChaosVehicleWheel>(*(mkWheelPath + "BP_RearWheel"));
    auto floatCurve   = ConstructorHelpers::FObjectFinder<UCurveFloat>(*(mkWheelPath + "VehicleMovementCompExternalCurve"));
    
    // 예외처리
    if (!bpFrontWheel.Succeeded() ||
        !bpRearWheel.Succeeded()  ||
        !floatCurve.Succeeded())
        return;

    // 차량 바퀴 설정
    TArray<FName> arrWheelName
    {
        "Wheel_Front_Left",
        "Wheel_Front_Right",
        "Wheel_Rear_Left",
        "Wheel_Rear_Right"
    };
    FChaosWheelSetup wheelSetup;
    wheelSetup.WheelClass = bpFrontWheel.Class;

    for (int i = 0; i < arrWheelName.Num(); i++)
    {
        // 뒷바퀴 초기화
        if (i == 2)
            wheelSetup.WheelClass = bpRearWheel.Class;

        wheelSetup.BoneName = arrWheelName[i];
        vehicleMoveComp->WheelSetups.Add(wheelSetup);
    }
    vehicleMoveComp->EngineSetup.TorqueCurve.ExternalCurve = floatCurve.Object;
}

void ACoreVehicle::PlayerExit()
{
    // 플레이어 초기세팅으로
    auto p_playerController = UGameplayStatics::GetPlayerController(this, 0);
    p_playerController->Possess(mpPlayer);
    mMapEmptySeat[mpPlayer->CurrentSeatType] = false;
    mpPlayer->ExitFromVehicle(MapDoorPos[mpPlayer->CurrentSeatType]);
    mCurrentPlayerCount--;
}

void ACoreVehicle::Accelerate(float Value)
{
    if (mbPlayerInFirstSeat)
        GetVehicleMovementComponent()->SetThrottleInput(Value);
}

void ACoreVehicle::Brake(float Value)
{
    if (mbPlayerInFirstSeat)
        GetVehicleMovementComponent()->SetBrakeInput(Value);
}

void ACoreVehicle::Handling(float Value)
{
    if (mbPlayerInFirstSeat)
        GetVehicleMovementComponent()->SetSteeringInput(Value);
}

void ACoreVehicle::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

void ACoreVehicle::Turn(float Value)
{
    AddControllerYawInput(Value);
}

void ACoreVehicle::CheckForDoorPos()
{
    ESeatType seatType = NONE;
    float     arrVecDistance[4]{ 0.f };
    auto maxSeater = mVehicleData.MaxSeater;

    // 벡터 확인
    for (int i = 0; i < maxSeater; i++)
    {
        FVector playerPos  = mpPlayer->GetActorLocation();
        FVector carDoorPos = MapDoorPos[(ESeatType)i];
        arrVecDistance[i]  = FVector::Distance(playerPos.GetAbs(), carDoorPos.GetAbs());
    }
    auto distance1 = arrVecDistance[0], distance2 = arrVecDistance[1], distance3 = arrVecDistance[2], distance4 = arrVecDistance[3];

    // 문짝이 2개일 시
    if (maxSeater == 2)
        seatType = (distance1 < distance2) ? FIRST : SECOND;

    // 문짝이 4개일 시
    else
    {
        // 왼쪽 첫번째 좌석
        if      (distance1 < distance2 &&
                 distance1 < distance3 &&
                 distance1 < distance4)
                 seatType = FIRST;

        // 왼쪽 두번째 좌석
        else if (distance2 < distance1 &&
                 distance2 < distance3 &&
                 distance2 < distance4)
                 seatType = SECOND;

        // 오른쪽 첫번째 좌석
        else if (distance3 < distance1 &&
                 distance3 < distance2 &&
                 distance3 < distance4)
                 seatType = THIRD;

        // 오른쪽 첫번째 좌석
        else if (distance4 < distance1 &&
                 distance4 < distance2 &&
                 distance4 < distance3)
                 seatType = FOURTH;
    }
    if (seatType != NONE)
    {
        mMapEmptySeat[seatType]   = true;
        mpPlayer->CurrentSeatType = seatType;
    }
}

void ACoreVehicle::SetPlayerIntoSeat()
{
    auto p_playerController = UGameplayStatics::GetPlayerController(this, 0);
    auto p_playerCollider   = mpPlayer->GetCapsuleComponent();

    // 플레이어 세팅
    mpPlayer->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    mpPlayer->SetActorLocation(MapSeatPos[mpPlayer->CurrentSeatType]);

    // 플레이어 콜라이더 세팅
    p_playerCollider->SetMobility(EComponentMobility::Static);
    p_playerCollider->SetCollisionProfileName("OverlapAll");
    p_playerController->Possess(this);
    mCurrentPlayerCount++;
}

void ACoreVehicle::UpdatePlayerSeatLocation(ESeatType SeatType)
{
    if (!mpPlayer)
        return;

    if (SeatType == THIRD ||
        SeatType == FOURTH)
    {
        if (mVehicleData.MaxSeater == 2)
            return;
    }
    // 플레이어가 지면을 뚫고 내려갈 수 있으므로 이동 가능 설정 후 즉시 불가능으로 설정
    auto p_playerCollider = mpPlayer->GetCapsuleComponent();
    p_playerCollider->SetMobility(EComponentMobility::Movable);
    mpPlayer->CurrentSeatType = SeatType;
    mpPlayer->SetActorLocation(MapSeatPos[SeatType]);
    p_playerCollider->SetMobility(EComponentMobility::Static);
}

bool ACoreVehicle::IsSeatAvailable(ACustomPlayer* pPlayer)
{
    if (mCurrentPlayerCount < mVehicleData.MaxSeater - 1)
    {
        mpPlayer = pPlayer;

        // 빈 좌석 확인 후 위치 가져오기
        CheckForDoorPos();

        if (!mMapEmptySeat[pPlayer->CurrentSeatType])
            return false;

        SetPlayerIntoSeat();
        return true;
    }
    return false;
}