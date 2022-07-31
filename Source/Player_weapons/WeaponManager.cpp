﻿#include "WeaponManager.h"
#include "CoreMeleeWeapon.h"
#include "CoreThrowableWeapon.h"
#include "CoreWeapon.h"
#include "DrawDebugHelpers.h"
#include "PUBG_UE4/BaseInteraction.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "PUBG_UE4/SoundManager.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h" 
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Objects/CoreBullet.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"

AWeaponManager::AWeaponManager()
{
	PrimaryActorTick.bCanEverTick = true;

    // 경로 예측 오브젝트 초기화
    InitGrenadePath();
}

void AWeaponManager::BeginPlay()
{
    Super::BeginPlay();

    // 투척류 무기 맨 마지막 지점 오브젝트 생성
    if (BP_GrenadeEndPoint)
    {
        GrenadeEndPoint = GetWorld()->SpawnActor<AActor>(BP_GrenadeEndPoint, FTransform::Identity);

        if (GrenadeEndPoint)
            GrenadeEndPoint->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
    }
    mpGameInst = Cast< UCustomGameInstance>(GetWorld()->GetGameInstance());
}

void AWeaponManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    UpdateCurrentWeaponArr();
    CheckReloading(DeltaTime);
    CheckShooting(DeltaTime);
}

void AWeaponManager::CheckForEquippedWeapon()
{
    //
}

void AWeaponManager::InitGrenadePath()
{
    SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));   
    RootComponent = SplineComp;

    auto BP_GrenadeEndPointRef = ConstructorHelpers::FClassFinder<AActor>(TEXT("/Game/UI/PredictGrenadePath/BP_PredictEndpoint.BP_PredictEndpoint_C"));

    if (BP_GrenadeEndPointRef.Succeeded())
        BP_GrenadeEndPoint = BP_GrenadeEndPointRef.Class;

    auto PATH_MESH = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Cylinder"));

    if (PATH_MESH.Succeeded())
        PathMesh = PATH_MESH.Object;

    auto PATH_MAT = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("/Game/UI/PredictGrenadePath/PredictionPathMat"));

    if (PATH_MAT.Succeeded())
        PathMat = PATH_MAT.Object;
}

void AWeaponManager::UpdateCurrentWeaponArr()
{
    TArray< ABaseInteraction*> p_arrCurrentWeapon
    {
        pFirstGun,
        pSecondGun,
        pPistol,
        pMelee,
        pThrowable
    };
    for (int i = 0; i < p_arrCurrentWeapon.Num(); i++)
         bArrWeaponEquipped[i] = (p_arrCurrentWeapon[i] != nullptr);
}

void AWeaponManager::PlaySound(EWeaponSoundType SoundType)
{
    TArray< ABaseInteraction*> p_arrCurrentWeapon
    {
        pFirstGun,
        pSecondGun,
        pPistol,
        pMelee,
        pThrowable
    };
    if (CurrentWeaponType != NONE)
    {
        int index = ((int)CurrentWeaponType) - 1;
        
        if (mpGameInst)
        {
            if (auto p_soundManager = mpGameInst->pSoundManager)
            {
                if (auto p_weapon = p_arrCurrentWeapon[index])
                    p_soundManager->PlayGunSound(p_weapon->GetActorLocation(), SoundType, GetWeaponType(p_weapon));
            }   
        }
    }
}

ECurrentWeaponType AWeaponManager::GetWeaponIndex(FString Direction, int StartIndex) const
{
    // 위에서 아래
    if (Direction == "Down")
    {
        for (int i = StartIndex - 1; i > -1; i--)
        {
            if (bArrWeaponEquipped[i])
                return (ECurrentWeaponType)(i + 1);
        }
    }
    // 아래에서 위
    else
    {
        for (int i = StartIndex - 1; i < 5; i++)
        {
            if (bArrWeaponEquipped[i])
                return (ECurrentWeaponType)(i + 1);
        }
    }
    return ECurrentWeaponType::NONE;
}

void AWeaponManager::AttachWeapon(ABaseInteraction* pWeapon, FString SocketName, bool bCheck /* = true */)
{
    if (!pWeapon)
        return;

    // 총기일 때만 인벤토리 총알과 연동 용도
    if (auto p_gun = Cast<ACoreWeapon>(pWeapon))
        p_gun->bInInventory = true;

    // 소켓 기반 무기 종류 판별 후 다운캐스팅
    if (SocketName == "HandGunSock")
    {
        pPistol           = Cast<ACoreWeapon>(pWeapon);
        CurrentWeaponType = PISTOL;
    }
    else if (SocketName == "FirstGunSock")
    {
        pFirstGun         = Cast<ACoreWeapon>(pWeapon);
        CurrentWeaponType = FIRST;
    }
    else if (SocketName == "SecondGunSock")
    {
        // 중복 무기 방지
        if (bCheck)
        {
            if (pFirstGun == Cast<ACoreWeapon>(pWeapon))
                return;
        }
        pSecondGun        = Cast<ACoreWeapon>(pWeapon);
        CurrentWeaponType = SECOND;
    }
    // 근접무기/투척류
    else
    {
        if (SocketName == "MeleeSock")
        {
            pMelee = Cast<ACoreMeleeWeapon>(pWeapon);
            CurrentWeaponType = MELEE;            
        }
        else
        {
            if (pWeapon->IsA<ACoreThrowableWeapon>())
                CurrentWeaponType = THROWABLE;
        }
            
    }
    auto playerMesh = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetMesh();
    pWeapon->ChangeCollisionSettings(false);
    pWeapon->AttachToMesh(playerMesh, *SocketName);
}

void AWeaponManager::ResetAfterDetaching(ABaseInteraction* pWeapon, FTransform NewPos)
{
    if (pWeapon)
        pWeapon->Detach(NewPos);

    if (auto p_gun = Cast<ACoreWeapon>(pWeapon))
    {   
        p_gun->WeaponData.CurrentMaxBulletCount = 0;
        p_gun->bInInventory = false;
    }
}

void AWeaponManager::PredictGrenadePath()
{
    auto p_player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    if (!SplineComp ||
        !p_player)
        return;

    mbThrowingGrenade = true;
    // 투척류 예측 경로 데이터 설정
    FVector socketPos = p_player->GetMesh()->GetSocketLocation("GrenadeThrowSock");
    FVector launchVelocity = UKismetMathLibrary::GetForwardVector(p_player->GetActorRotation()) * GrenadeDirection * 1500.f;
    FPredictProjectilePathParams predictParams(50.f, socketPos, launchVelocity, 2.f, EObjectTypeQuery::ObjectTypeQuery1);
    predictParams.bTraceWithCollision = true;
    predictParams.SimFrequency = 15.f;
    predictParams.OverrideGravityZ = 0.f;
    FPredictProjectilePathResult result;
    UGameplayStatics::PredictProjectilePath(GetWorld(), predictParams, result);
    mGrenadeVelocity = predictParams.LaunchVelocity;

    GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, FString::Printf(TEXT("Num : %d, Launch Velocity : %s"), result.PathData.Num(), *predictParams.LaunchVelocity.ToString()));
    // 경로 지정
    for (int i = 0; i < result.PathData.Num(); i++)
        SplineComp->AddSplinePointAtIndex(result.PathData[i].Location, i, ESplineCoordinateSpace::World);

    for (int i = 0; i < SplineComp->GetNumberOfSplinePoints(); i++)
    {
        // 라인 메쉬 생성
        // 특이사항) 컴포넌트들은 NewObject으로 생성 가능하나 UI와 달리 하나의 엑터라고 볼 수가 있음
        // 따라서 StaticClass()으로 해줌으로서 엑터 오브젝트라는걸 명시해야됨.
        if (auto splineMeshComp = NewObject<USplineMeshComponent>(GetWorld(), USplineMeshComponent::StaticClass()))
        {
            splineMeshComp->SetMobility(EComponentMobility::Movable);
            splineMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            splineMeshComp->AttachToComponent(SplineComp, FAttachmentTransformRules::KeepRelativeTransform);
            splineMeshComp->RegisterComponentWithWorld(GetWorld());

            // 위치 설정
            //splineMeshComp->SetRelativeTransform(FTransform::Identity);
            FVector startPos, startTangent, endPos, endTangent;
            SplineComp->GetLocationAndTangentAtSplinePoint(i, startPos, startTangent, ESplineCoordinateSpace::World);
            SplineComp->GetLocationAndTangentAtSplinePoint(i + 1, endPos, endTangent, ESplineCoordinateSpace::World);
            splineMeshComp->SetStartAndEnd(startPos, startTangent, endPos, endTangent);

            // 비쥬얼 설정
            splineMeshComp->SetForwardAxis(ESplineMeshAxis::Z);
            splineMeshComp->SetStartScale(FVector2D(0.02f));
            splineMeshComp->SetEndScale(FVector2D(0.02f));
            splineMeshComp->SetStartOffset(FVector2D::ZeroVector);
            splineMeshComp->SetEndOffset(FVector2D::ZeroVector);
            splineMeshComp->SetStaticMesh(PathMesh);
            splineMeshComp->SetMaterial(0, PathMat);
            splineMeshComp->SetVisibility(true);
            arrSplineMeshComp.Add(splineMeshComp);
        }
    }
    // 도착 지점에 생성 및 설정
    if (GrenadeEndPoint)
    {
        auto endPos = result.LastTraceDestination.Location;
        GrenadeEndPoint->SetHidden(false);
        GrenadeEndPoint->SetActorRelativeLocation(endPos);

        if (pThrowable)
            pThrowable->GrenadeEndPos = endPos;
    }
}

EGunShootType AWeaponManager::GetNextShootType(EGunShootType CurrentType, FString GroupType)
{
    if (CurrentType != GetMaxShootType(GroupType))
    {
        switch (CurrentType)
        {
        case SINGLE: return BURST;
        case BURST:  return CONSECUTIVE;
        }
    }
    return SINGLE;
}

EGunShootType AWeaponManager::GetMaxShootType(FString WeaponGroup)
{
    TMap<FString, EGunShootType> mapGunShootType
    {
        // ------- 오직 단발 -------
        {"HandGun"  , SINGLE},
        {"SniperGun", SINGLE},
        {"Special"  , SINGLE},

        // ------- 단발 / 점사 -------
        {"ShotGun", BURST},

        // ------- 단발 / 점사 / 연사 -------
        {"AssaultGun", CONSECUTIVE},
        {"SMG"       , CONSECUTIVE}
    };
    return mapGunShootType[WeaponGroup];
}

FString AWeaponManager::GetShootTypeStr(EGunShootType Type)
{
    if (Type == MAX)
        return "Fail";

    TMap<EGunShootType, FString> mapGunShootType
    {
        { SINGLE,      "Single" },
        { BURST,       "Burst" },
        { CONSECUTIVE, "Consecutive" }
    };
    return "Current type : " + mapGunShootType[Type];
}

void AWeaponManager::Equip(AActor* pWeapon, bool bCheck /* = true */)
{
    ABaseInteraction* pCollidedWeapon = Cast<ABaseInteraction>(pWeapon);

    if (!pCollidedWeapon)
        return;
    
    pCollidedWeapon->bPlayerNear = false;

    // 총기 종류
    if (pWeapon->IsA<ACoreWeapon>())
    {
        // 권총일 시 무조건 3번 슬롯
        if (Cast<ACoreWeapon>(pWeapon)->WeaponData.GroupType == "HandGun")
        {
            if (!pPistol)
                AttachWeapon(pCollidedWeapon, "HandGunSock", bCheck);

            else
                SwapWorld(pPistol, pWeapon, "HandGunSock");
        }
        // 기타 총기 1,2번 슬롯
        else
        {
            if (!pFirstGun) // 첫번째 무기가 없을 시
                AttachWeapon(pCollidedWeapon, "FirstGunSock", bCheck);

            else
            {
                if (!pSecondGun) // 두번째 무기가 없을 시
                    AttachWeapon(pCollidedWeapon, "SecondGunSock", bCheck);

                else
                {
                    // 첫번째 무기 장착중
                    if      (CurrentWeaponType == FIRST)
                             SwapWorld(pFirstGun, pWeapon, "FirstGunSock");

                    // 두째 무기 장착중
                    else if (CurrentWeaponType == SECOND)
                             SwapWorld(pSecondGun, pWeapon, "SecondGunSock");
                }
            }
        }
    }
    // 근접 종류
    else if (pWeapon->IsA<ACoreMeleeWeapon>())
             AttachWeapon(pCollidedWeapon, "EquippedWeaponPosSock", bCheck);

    // 투척류
    else if (pWeapon->IsA<ACoreThrowableWeapon>())
    {
        // 수류탄을 장착중이지 않을 때만
        if (mpGameInst)
            mpGameInst->DeleSetItemOntoInventory.ExecuteIfBound(pCollidedWeapon, false);

        pWeapon->Destroy();
    }
}

void AWeaponManager::ClickEvent()
{
    ABaseInteraction* p_weapon = GetWeaponByIndex(CurrentWeaponType);

    if (!p_weapon ||
        mbReloading)
        return;

    if (p_weapon->IsA<ACoreWeapon>())
    {
        auto p_gun = Cast<ACoreWeapon>(p_weapon);
        auto weaponData = p_gun->WeaponData;

        // 총알 부족한지 확인
        if (weaponData.CurrentBulletCount == 0)
        {
            // 총알이 하나라도 존재할 시
            if (weaponData.CurrentMaxBulletCount > 0)
                Reload();

            else
            {
                PlaySound(EWeaponSoundType::EMPTY_AMMO);
                bShooting = false;
                return;
            }
        }   
        bShooting = true;
        PlaySound(EWeaponSoundType::SHOT);

        // 사운드 적용 및 총알 1개 차감
        if (mpGameInst)
            mpGameInst->DeleDeleteInventoryItem.ExecuteIfBound(p_gun->WeaponData.BulletType);                

        // ------- 레이캐스트 범위 구하기 -------

        FVector spawnPos = p_gun->SkeletalMeshComp->GetSocketLocation("Barrel");

        // 카메라 기반으로 구하기
        auto cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
        auto traceStartPos = UKismetMathLibrary::ProjectPointOnToPlane(cameraManager->GetCameraLocation(), spawnPos, cameraManager->GetActorForwardVector());
        auto traceEndPos = traceStartPos + (cameraManager->GetActorForwardVector() * 2500.f);

        // 게임 뷰포트 가운데 지점 구하기    
        FVector2D vPortCenterPos = GEngine->GameViewport->Viewport->GetSizeXY() / 2.f;
        FVector deprojPos, deprojDir;
        UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(GetWorld(), 0), vPortCenterPos, deprojPos, deprojDir);
        FVector endPos = deprojPos + (deprojDir * 2500.f);

        // 레이캐스트 적용(시작지점 : 카메라) 충돌 시 = true > 충돌 지점, false > 레이 끝 지점 
        FHitResult hitResult;
        FCollisionQueryParams collisionQueryParams;
        collisionQueryParams.AddIgnoredActor(p_gun);
        collisionQueryParams.AddIgnoredActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
                
        auto bTraced   = GetWorld()->LineTraceSingleByChannel(hitResult, traceStartPos, traceEndPos, ECollisionChannel::ECC_Visibility, collisionQueryParams);
        FVector hitPos = UKismetMathLibrary::SelectVector(hitResult.ImpactPoint, hitResult.TraceEnd, bTraced);

        // 총알 복제 후 위치 및 회전값 지정
        FRotator rot = UKismetMathLibrary::FindLookAtRotation(traceStartPos, traceEndPos);
        auto p_bullet = GetWorld()->SpawnActor<ACoreBullet>(p_gun->BP_Bullet, spawnPos, rot);
        p_gun->ParticleComp->Activate(true);

        DrawDebugLine(GetWorld(), traceStartPos, traceEndPos, FColor::Red, true);
    }
    // 근접 무기
    else if(p_weapon->IsA<ACoreMeleeWeapon>())
    {
            
    }
    // 투척류 무기
    else
    {
        // 크레모아가 아닌 무기일 때만 투척 경로 지정
        if (pThrowable &&
            pThrowable->WeaponType != EThrowableWeaponType::CLAYMORE)
            PredictGrenadePath();
    }
}

void AWeaponManager::Reload()
{
    if (auto p_gun = Cast<ACoreWeapon>(GetWeaponByIndex(CurrentWeaponType)))
    {
        PlaySound(EWeaponSoundType::RELOAD);
        auto& weaponData = p_gun->WeaponData;
        int result = (weaponData.CurrentBulletCount > 0) ? (weaponData.MaxBulletCount - weaponData.CurrentBulletCount) : p_gun->WeaponData.MaxBulletCount;
        weaponData.MaxBulletCount     -= result;
        weaponData.CurrentBulletCount += result;
        mbReloading = true;
    }
}

void AWeaponManager::ThrowGrenade()
{
    if (pThrowable)
    {
        pThrowable->Throw(mGrenadeVelocity);
        mGrenadeVelocity  = FVector::ZeroVector;
        mbThrowingGrenade = false;
        GrenadeDirection  = 0.f;
        
        if (GrenadeEndPoint)
            GrenadeEndPoint->SetHidden(true);
    }
    SetNull(THROWABLE);
    DeleSetExplosiveUI.ExecuteIfBound(nullptr);
    ResetGrenadePath();
}

void AWeaponManager::ResetGrenadePath()
{
    // 경로 제거 및 메쉬 제거
    SplineComp->ClearSplinePoints();

    for (int i = 0; i < arrSplineMeshComp.Num(); i++)
    {
        if (auto splineMeshComp = arrSplineMeshComp[i])
            splineMeshComp->DestroyComponent();
    }
    arrSplineMeshComp.Empty();
}

void AWeaponManager::UpdateGrenadePath()
{
    if (mbThrowingGrenade)
    {
        ResetGrenadePath();
        PredictGrenadePath();
    }
}

bool AWeaponManager::ScrollSelect(FString Pos)
{
    // 현재 무기 인덱스 갖고와서 선택
    auto tmpWeaponType = CurrentWeaponType;
    int currentIndex = (int)CurrentWeaponType;
    int totalWeapon  = -1;

    for (int i = 0; i < 5; i++)
    {
        // 현재 장착 중인 무기
        if (bArrWeaponEquipped[i])
            totalWeapon++;
    }
    // 무기가 1개일 시 옮길 필요가 없음
    if (totalWeapon == 0)
        return false;

    // 아래로 스크롤
    if (Pos == "Down")
    {
        // 마지막 원소 0 도달 시
        if (currentIndex == (int)FIRST)
            CurrentWeaponType = GetWeaponIndex("Down", (int)THROWABLE);

        else
        {
            CurrentWeaponType = GetWeaponIndex("Down", currentIndex - 1); // 현재 위치에서 탐색

            // 발견하지 못했을 시
            if (CurrentWeaponType == NONE)
                CurrentWeaponType = GetWeaponIndex("Down", (int)THROWABLE);
        }
    }
    // 위로 스크롤
    else
    {
        // 현재 마지막 원소에 접근 할 시
        if (currentIndex == (int)THROWABLE)
            CurrentWeaponType = (pFirstGun) ? FIRST : GetWeaponIndex("Up", 1);

        else
        {
            CurrentWeaponType = GetWeaponIndex("Up", currentIndex + 1); // 현재 위치에서 탐색

            // 발견하지 못했을 시
            if (CurrentWeaponType == NONE)
                CurrentWeaponType = GetWeaponIndex("Up", 1);
        }
    }
    Swap(tmpWeaponType, true);
    return true;
}

void AWeaponManager::SwapWorld(ABaseInteraction* pNewWeapon, AActor* pCurrentWeapon, FString SocketName)
{
    ResetAfterDetaching(pNewWeapon, pCurrentWeapon->GetActorTransform());
    AttachWeapon(Cast<ABaseInteraction>(pCurrentWeapon), SocketName);
}

void AWeaponManager::Swap(ECurrentWeaponType WeaponType, bool bScrolling /* = false */)
{
    auto playerMesh   = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetMesh();
    auto attachType   = (bScrolling) ? CurrentWeaponType : WeaponType;
    auto detachType   = (bScrolling) ? WeaponType : CurrentWeaponType;
    CurrentWeaponType = attachType;

    // 장착 되있던 무기를 탈착
    switch (detachType)
    {
    case FIRST:     if (pFirstGun)  pFirstGun->AttachToMesh(playerMesh, "FirstGunSock"); break;
    case SECOND:    if (pSecondGun) pSecondGun->AttachToMesh(playerMesh, "SecondGunSock"); break;
    case PISTOL:    if (pPistol)    pPistol->AttachToMesh(playerMesh, "HandGunSock");      break;
    case MELEE:     AttachWeapon(pMelee, ""); break;
    case THROWABLE: if (pThrowable) pThrowable->Destroy(); break;
    }
    // 탈착 되있던 무기를 장착
    if (CurrentWeaponType == THROWABLE)
        CreateExplosive(pThrowable);
    
    else
        ChangeAimPose(mbAiming);
}

int AWeaponManager::Swap(ABaseInteraction* pNewWeapon, ABaseInteraction* pCurrentWeapon /* = nullptr */, ECurrentWeaponType WeaponType /* = ECurrentWeaponType::NONE */)
{
#define ERROR -1

    switch (WeaponType)
    {
    case FIRST: case SECOND:
    {
        auto p_newWeapon     = Cast<ACoreWeapon>(pNewWeapon);
        auto p_currentWeapon = Cast<ACoreWeapon>(pCurrentWeapon);

        if (pNewWeapon)
        {
            if (pNewWeapon->IsA<ACoreMeleeWeapon>() ||
                pNewWeapon->IsA<ACoreThrowableWeapon>())
                return ERROR;
        }
        if (p_newWeapon &&
            p_newWeapon->WeaponData.GroupType == "HandGun")
            return ERROR;

        // 첫번째 총과 두번째 총 교체
        if (p_newWeapon     == pFirstGun &&
            p_currentWeapon == pSecondGun)
        {
            AttachWeapon(p_newWeapon,     "SecondGunSock", false);
            AttachWeapon(p_currentWeapon, "FirstGunSock",  false);
        }
        // 두번째 총과 첫번째 총 교체
        else if (p_currentWeapon == pFirstGun &&
                 p_newWeapon     == pSecondGun)
        {
            AttachWeapon(p_currentWeapon, "SecondGunSock", false);
            AttachWeapon(p_newWeapon,     "FirstGunSock",  false);
        }
        // 인벤토리 총과 첫번째 총 교체
        else
        {
            FString sockName = (WeaponType == FIRST) ? "FirstGunSock" : "SecondGunSock";
            SwapWorld(p_currentWeapon, p_newWeapon, sockName);
        }
    }
    break;

    case PISTOL:
    {
        auto p_newWeapon = Cast<ACoreWeapon>(pNewWeapon);

        if (!p_newWeapon || 
            (p_newWeapon && p_newWeapon->WeaponData.GroupType != "HandGun"))
            return ERROR;

        if (p_newWeapon != pPistol)
            SwapWorld(pCurrentWeapon, p_newWeapon, "HandGunSock");
    }
    break;

    case MELEE:
    {
        auto p_newMelee = Cast<ACoreMeleeWeapon>(pNewWeapon);

        if (!p_newMelee ||
            (p_newMelee && p_newMelee->WeaponData.GroupType != "Melee"))
            return ERROR;
    }
    break;

    case THROWABLE: CreateExplosive(Cast<ACoreThrowableWeapon>(pNewWeapon)); break;
    }
    return 0;
}

void AWeaponManager::ChangeShootMode()
{
    // 총기일 시
    if (auto p_weapon = GetWeaponByIndex(CurrentWeaponType))
    {
        if (p_weapon->IsA<ACoreWeapon>())
        {
            if (auto p_gun = Cast<ACoreWeapon>(p_weapon))
            {
                // 격발 방식 변경
                p_gun->ShootType = GetNextShootType(p_gun->ShootType, p_gun->WeaponData.GroupType);

                // 팝업 UI 설정
                if (mpGameInst)
                    mpGameInst->DeleSetShootTypeNotificationTxt.ExecuteIfBound(GetShootTypeStr(p_gun->ShootType));
            }
        }
    }
}

void AWeaponManager::ChangeAimPose(bool bAiming)
{
    mbAiming = bAiming;
    // 현재 착용 중인 무기를 가지고옴
    ACoreWeapon* p_gun = nullptr;
    FString      socketName = "";

    switch (CurrentWeaponType)
    {
    case FIRST:  p_gun = pFirstGun;  socketName = "FirstGunSock";  break;
    case SECOND: p_gun = pSecondGun; socketName = "SecondGunSock"; break;
    case PISTOL: p_gun = pPistol;    socketName = "HandGunSock";   break;
    }
    // 캐릭터 메쉬에다 부착
    if (p_gun)
        p_gun->AttachToMesh(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetMesh(), mbAiming ? "EquippedWeaponPosSock" : socketName);
}

void AWeaponManager::CheckReloading(float TranscurredTime)
{
    // 재장전 중
    if (mbReloading)
    {
        mCurrentReloadTime += TranscurredTime;

        if (mCurrentReloadTime > mkReloadTime)
        {
            mCurrentReloadTime = 0.f;
            mbReloading        = false;
        }
    }
}

void AWeaponManager::CheckShooting(float TranscurredTime)
{
    if (bShooting)
    {
        if (auto p_gun = Cast<ACoreWeapon>(GetWeaponByIndex(CurrentWeaponType)))
        {
            float timeToWait = 0.25f;
            mCurrentShootTime += TranscurredTime;

            switch (p_gun->ShootType)
            {
            case SINGLE: bShooting = false; break;

            case BURST:

                if (mCurrentShootTime > timeToWait)
                {
                    ClickEvent();
                    mCurrentShootTime = 0.f;
                    mBurstCount++;
                }
                if (mBurstCount == 2)
                    bShooting = false;

                break;

            // 쿨타임 적용
            case CONSECUTIVE:

                if (mCurrentShootTime > timeToWait)
                {
                    ClickEvent();
                    mCurrentShootTime = 0.f;
                }
                break;
            }
        }
    }
    else
    {
        mCurrentShootTime = 0.f;
        mBurstCount = 0;
    }
}

void AWeaponManager::CreateExplosive(ACoreThrowableWeapon* pGrenade /* = nullptr */)
{
    if (!pGrenade)
        return;

    // 오브젝트 생성 후 투척류로 지정
    pThrowable = GetWorld()->SpawnActor<ACoreThrowableWeapon>(ACoreThrowableWeapon::StaticClass());
    pThrowable->Setup(pGrenade);
    AttachWeapon(pThrowable, pThrowable->WeaponData.Type + "Sock");
    DeleSetExplosiveUI.ExecuteIfBound(pThrowable);
}

ABaseInteraction* AWeaponManager::GetWeaponByIndex(ECurrentWeaponType WeaponType) const
{
    if (WeaponType == NONE)
        return nullptr;

    // 원소 인덱스에 따라 현재 무기를 가져옴
    TArray<ABaseInteraction*> arrWeapon
    {
        pFirstGun,
        pSecondGun,
        pPistol,
        pMelee,
        pThrowable
    };
    int index = static_cast<int>(WeaponType);

    if (index == 0)
        return nullptr;

    return arrWeapon[index - 1];
}

ACoreWeapon* AWeaponManager::GetCurrentGun() const { return Cast<ACoreWeapon>(GetWeaponByIndex(CurrentWeaponType)); }

ECurrentWeaponType AWeaponManager::GetWeaponIndex(ABaseInteraction* pWeapon) const
{
    TMap<ABaseInteraction*, ECurrentWeaponType> arrWeaponKeyType
    { 
        { pFirstGun,  FIRST},
        { pSecondGun, SECOND},
        { pPistol,    PISTOL},
        { pMelee,     MELEE},
        { pThrowable, THROWABLE}
    };
    for (auto item : arrWeaponKeyType)
    {
        if (pWeapon == item.Key)
            return item.Value;
    }
    return NONE;
}

int AWeaponManager::GetWeaponType(ABaseInteraction* pWeapon) const
{
    int weaponType = 0;

    // 총기일시
    if      (auto p_gun = Cast<ACoreWeapon>(pWeapon))
             weaponType = static_cast<int>(p_gun->WeaponType);

    else if (auto p_melee = Cast<ACoreMeleeWeapon>(pWeapon))
             weaponType = static_cast<int>(p_melee->WeaponType);

    else if (auto p_throwable = Cast<ACoreThrowableWeapon>(pWeapon))
             weaponType = static_cast<int>(p_throwable->WeaponType);
    
    return weaponType;
}

void AWeaponManager::Drop(ECurrentWeaponType WeaponType)
{
    // 무기를 맵에다 드롭
    if (auto playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
    {
        ResetAfterDetaching(GetWeaponByIndex(WeaponType), FTransform(playerCharacter->GetActorRotation(), playerCharacter->GetActorLocation() + FVector(0.f, 75.f, 20.f)));
        SetNull(WeaponType);
    }
}

void AWeaponManager::SetNull(ECurrentWeaponType WeaponType)
{
    // 현재 보유 중인 무기랑 비교
    switch (WeaponType)
    {
    case FIRST:     pFirstGun  = nullptr; break;
    case SECOND:    pSecondGun = nullptr; break;
    case PISTOL:    pPistol    = nullptr; break;
    case MELEE:     pMelee     = nullptr; break;
    case THROWABLE: pThrowable = nullptr; break;
    }
}

bool AWeaponManager::IsWrongType(ABaseInteraction* pWeapon, ECurrentWeaponType WeaponType, bool bFromWeaponSlot)
{
    if (!pWeapon)
        return true;

    auto groupType = pWeapon->ObjectGroupType;
    
    switch (WeaponType)
    {
    // 총기 
    case FIRST: 
    case SECOND:
    {
        auto p_gun = Cast<ACoreWeapon>(pWeapon);
        return (bFromWeaponSlot) ? (groupType == "Handgun") : (p_gun == pFirstGun || p_gun == pSecondGun);
    }
    case PISTOL:    return (Cast<ACoreWeapon>(pWeapon)          == pPistol    || groupType != "HandGun");
    case MELEE:     return (Cast<ACoreMeleeWeapon>(pWeapon)     == pMelee     || groupType != "Melee");
    case THROWABLE: return (Cast<ACoreThrowableWeapon>(pWeapon) == pThrowable || groupType != "Explosive");
    }
    return true;
}

void AWeaponManager::SetMeshToPlayerUI(TArray<AActor*> pArrActor)
{
    auto p_firstGunUI  = Cast<ACoreWeapon>(pArrActor[(int)FIRST]);
    auto p_secondGunUI = Cast<ACoreWeapon>(pArrActor[(int)SECOND]);
    auto p_pistolUI    = Cast<ACoreWeapon>(pArrActor[(int)PISTOL]);
    auto p_meleeUI     = Cast<ACoreMeleeWeapon>(pArrActor[(int)MELEE]);
    auto p_throwableUI = Cast<ACoreThrowableWeapon>(pArrActor[(int)THROWABLE]);

    p_firstGunUI->SetSkeletalMesh((pFirstGun) ? pFirstGun->GetSkeletalMesh() : nullptr);
    p_secondGunUI->SetSkeletalMesh((pSecondGun) ? pSecondGun->GetSkeletalMesh() : nullptr);
    p_pistolUI->SetSkeletalMesh((pPistol) ? pPistol->GetSkeletalMesh() : nullptr);
    p_meleeUI->SetStaticMesh((pMelee) ? pMelee->GetStaticMesh() : nullptr);
    p_throwableUI->SetStaticMesh((pThrowable) ? pThrowable->GetStaticMesh() : nullptr);
}