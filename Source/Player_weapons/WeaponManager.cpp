﻿#include "WeaponManager.h"
#include "CoreMeleeWeapon.h"
#include "CoreThrowableWeapon.h"
#include "CoreWeapon.h"
#include "DrawDebugHelpers.h"
#include "GunRecoilComponent.h"
#include "Farmable_items/CoreAttachment.h"
#include "Farmable_items/CoreBarrel.h"
#include "Farmable_items/CoreForend.h"
#include "Farmable_items/CoreGrip.h"
#include "Farmable_items/CoreSight.h"
#include "Farmable_items/CoreStock.h"
#include "PUBG_UE4/BaseInteraction.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h" 
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
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
    mArrWeapon.SetNum(6);

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
    if (auto p_customGameInst = UCustomGameInstance::GetInst())
        p_customGameInst->DelePredictGrenadePath.BindUFunction(this, "PredictGrenadePath");
}

void AWeaponManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    mArrWeapon = { nullptr, pFirstGun, pSecondGun, pPistol, pMelee, pThrowable };
}

void AWeaponManager::CheckForEquippedWeapon()
{
    //
}

void AWeaponManager::InitGrenadePath()
{
    SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));   
    RootComponent = SplineComp;

    // 수류탄 경로 마지막 지점 엑터 생성
    ConstructorHelpers::FClassFinder<AActor> BP_GrenadeEndPointRef(TEXT("/Game/UI/PredictGrenadePath/BP_PredictEndpoint.BP_PredictEndpoint_C"));

    if (BP_GrenadeEndPointRef.Succeeded())
        BP_GrenadeEndPoint = BP_GrenadeEndPointRef.Class;

    // 수류탄 경로 그리기를 위한 모형
    ConstructorHelpers::FObjectFinder<UStaticMesh> PATH_MESH(TEXT("/Engine/BasicShapes/Cylinder"));

    if (PATH_MESH.Succeeded())
        PathMesh = PATH_MESH.Object;

    // 수류탄 경로 모형 색상
    ConstructorHelpers::FObjectFinder<UMaterial> PATH_MAT(TEXT("/Game/UI/PredictGrenadePath/PredictionPathMat"));

    if (PATH_MAT.Succeeded())
        PathMat = PATH_MAT.Object;
}

void AWeaponManager::PredictGrenadePath()
{
    auto p_player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    if (!SplineComp ||
        !p_player)
        return;

    mbThrowingGrenade = true;
    
#pragma region 투척류 예측 경로 데이터 설정

    FVector socketPos = p_player->GetMesh()->GetSocketLocation("GrenadeThrowSock");
    FVector launchVelocity = UKismetMathLibrary::GetForwardVector(p_player->GetActorRotation()) * GrenadeDirection * 1500.f;
    FPredictProjectilePathParams predictParams(50.f, socketPos, launchVelocity, 2.f, EObjectTypeQuery::ObjectTypeQuery1);
    predictParams.bTraceWithCollision = true;
    predictParams.SimFrequency = 15.f;
    predictParams.OverrideGravityZ = 0.f;
    FPredictProjectilePathResult result;
    UGameplayStatics::PredictProjectilePath(GetWorld(), predictParams, result);
    mGrenadeVelocity = predictParams.LaunchVelocity;
    
#pragma endregion

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
#pragma region 초기화

            splineMeshComp->SetMobility(EComponentMobility::Movable);
            splineMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            splineMeshComp->AttachToComponent(SplineComp, FAttachmentTransformRules::KeepRelativeTransform);
            splineMeshComp->RegisterComponentWithWorld(GetWorld());

#pragma endregion

#pragma region 위치 설정

            //splineMeshComp->SetRelativeTransform(FTransform::Identity);
            FVector startPos, startTangent, endPos, endTangent;
            SplineComp->GetLocationAndTangentAtSplinePoint(i, startPos, startTangent, ESplineCoordinateSpace::World);
            SplineComp->GetLocationAndTangentAtSplinePoint(i + 1, endPos, endTangent, ESplineCoordinateSpace::World);
            splineMeshComp->SetStartAndEnd(startPos, startTangent, endPos, endTangent);

#pragma endregion

#pragma region 비쥬얼 설정
            
            splineMeshComp->SetForwardAxis(ESplineMeshAxis::Z);
            splineMeshComp->SetStartScale(FVector2D(0.02f));
            splineMeshComp->SetEndScale(FVector2D(0.02f));
            splineMeshComp->SetStartOffset(FVector2D::ZeroVector);
            splineMeshComp->SetEndOffset(FVector2D::ZeroVector);
            splineMeshComp->SetStaticMesh(PathMesh);
            splineMeshComp->SetMaterial(0, PathMat);
            splineMeshComp->SetVisibility(true);
            arrSplineMeshComp.Add(splineMeshComp);

#pragma endregion
        }
    }
#pragma region 도착 지점에 생성 및 설정
    if (GrenadeEndPoint)
    {
        auto endPos = result.LastTraceDestination.Location;
        GrenadeEndPoint->SetHidden(false);
        GrenadeEndPoint->SetActorRelativeLocation(endPos);

        if (pThrowable)
            pThrowable->GrenadeEndPos = endPos;
    }
#pragma endregion
}

void AWeaponManager::AttachWeapon(ABaseInteraction* pWeapon, FString SocketName, bool bCheck /* = true */)
{
    if (!pWeapon)
        return;

    // 총기일 때만 인벤토리 총알과 연동 용도
    if (auto p_gun = Cast<ACoreWeapon>(pWeapon))
        p_gun->bInInventory = true;

    // 소켓 기반 무기 종류 판별 후 다운캐스팅
    if (SocketName == "FirstGunSock")
    {
        pFirstGun = Cast<ACoreWeapon>(pWeapon);

        if (pFirstGun)
            CurrentWeaponType = FIRST;
    }
    else if (SocketName == "SecondGunSock")
    {
        // 무기 중복 방지
        if (bCheck &&
            pFirstGun == Cast<ACoreWeapon>(pWeapon))
            return;

        pSecondGun  = Cast<ACoreWeapon>(pWeapon);

        if (pSecondGun)
            CurrentWeaponType = SECOND;
    }
    else if (SocketName == "HandGunSock")
    {
        pPistol = Cast<ACoreWeapon>(pWeapon);

        if (pPistol)
            CurrentWeaponType = PISTOL;
    }
    // 근접무기/투척류
    else if (SocketName == "MeleeSock")
    {
        pMelee = Cast<ACoreMeleeWeapon>(pWeapon);

        if (pMelee)
            CurrentWeaponType = MELEE;
    }
    else
    {
        pThrowable = Cast<ACoreThrowableWeapon>(pWeapon);

        if (pThrowable)
            CurrentWeaponType = THROWABLE;
    }    
    auto playerMesh = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetMesh();
    pWeapon->ChangeMeshSettings(false);
    pWeapon->GetMeshComp()->SetSimulatePhysics(false);
    pWeapon->AttachToMesh(playerMesh, SocketName);
    // pWeapon->SkeletalMeshComp->AttachToComponent(, FAttachmentTransformRules::SnapToTargetIncludingScale, *SocketName);
}

void AWeaponManager::Detach(ABaseInteraction* pWeapon)
{
    if (pWeapon)
        pWeapon->Detach();
}

ABaseInteraction* AWeaponManager::GetWeaponByIndex(EWeaponType WeaponType) const
{
    if (WeaponType == NONE)
        return nullptr;

    // 원소 인덱스에 따라 현재 무기를 가져옴
    return mArrWeapon[static_cast<int>(WeaponType)];
}

EWeaponType AWeaponManager::GetWeaponIndex(ABaseInteraction* pWeapon) const
{
    for (int i = 1; i < mArrWeapon.Num(); i++)
    {
        if (pWeapon == mArrWeapon[i])
            return (EWeaponType)(i + 1);
    }
    return NONE;
}

ACoreWeapon* AWeaponManager::GetCurrentGun() const { return Cast<ACoreWeapon>(GetWeaponByIndex(CurrentWeaponType)); }

int AWeaponManager::GetWeaponType(ABaseInteraction* pWeapon) const { return IsValid(pWeapon) ? pWeapon->ItemIdx : 0; }

void AWeaponManager::ClickEvent()
{
    if (ABaseInteraction* p_obj = GetWeaponByIndex(CurrentWeaponType))
        p_obj->ClickEvent();
}

void AWeaponManager::CreateExplosive(ACoreThrowableWeapon* pGrenade /* = nullptr */)
{
    if (!pGrenade)
        return;

    // 오브젝트 생성 후 투척류로 지정
    pThrowable = GetWorld()->SpawnActor<ACoreThrowableWeapon>(ACoreThrowableWeapon::StaticClass());
    pThrowable->Setup(pGrenade);
    AttachWeapon(pThrowable, "GrenadeSock");
    GetWorld()->GetGameInstance<UCustomGameInstance>()->DeleSetExplosiveUI.ExecuteIfBound(pThrowable);
}

void AWeaponManager::ThrowGrenade()
{
    if (!pThrowable)
        return;
    
    pThrowable->Throw(mGrenadeVelocity);
    mGrenadeVelocity  = FVector::ZeroVector;
    mbThrowingGrenade = false;
    GrenadeDirection  = 0.f;

    if (GrenadeEndPoint)
        GrenadeEndPoint->SetHidden(true);

    SetNull(THROWABLE);
    GetWorld()->GetGameInstance<UCustomGameInstance>()->DeleSetExplosiveUI.ExecuteIfBound(nullptr);
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
    int direction = (int)CurrentWeaponType;        
    EWeaponType changeType = NONE;

#pragma region 스크롤 아래로

    if (Pos == "Down")
    {
        if (direction == (int)FIRST)
            direction = (int)NONE;

        for (int i = direction - 1; i > 0; i--)
        {
            if (mArrWeapon[i])
            {
                changeType = (EWeaponType)i;
                break;
            }
        }
        if (changeType == NONE)
            changeType = THROWABLE;
    }
#pragma endregion

#pragma region 스크롤 위로

    else if (Pos == "Up")
    {
        for (int i = direction + 1; i < 6; i++)
        {
            if (mArrWeapon[i])
            {
                changeType = (EWeaponType)i;
                break;
            }
        }
        if (changeType == NONE)
            changeType = FIRST;
    }
#pragma endregion
    return Swap(changeType, true);
} 

bool AWeaponManager::TryToEquip(ABaseInteraction* pWeapon, bool bCheck /* = true */)
{
    // 총기 종류
    if (pWeapon->IsA<ACoreWeapon>())
    {
        // 권총일 시 무조건 3번 슬롯
        if (Cast<ACoreWeapon>(pWeapon)->WeaponData.GroupType == "HandGun")
        {
            AttachWeapon((!pPistol) ? pWeapon : pPistol, "HandGunSock", (!pPistol));
            return pPistol == nullptr;
        }
        // 기타 총기 1,2번 슬롯
        else
        {
            if (!pFirstGun) // 첫번째 무기가 없을 시
            {
                AttachWeapon(pWeapon, "FirstGunSock", bCheck);
                return true;
            }
            else
            {
                if (!pSecondGun) // 두번째 무기가 없을 시
                {
                    AttachWeapon(pWeapon, "SecondGunSock", bCheck);
                    return true;
                }
                else
                {
                    // 첫번째 무기 장착중
                    if (CurrentWeaponType == FIRST)
                    {
                        SwapWorld(pFirstGun, pWeapon, "FirstGunSock");
                        return pFirstGun == nullptr;
                    }
                    // 두째 무기 장착중
                    else if (CurrentWeaponType == SECOND)
                    {
                        SwapWorld(pSecondGun, pWeapon, "SecondGunSock");
                        return pSecondGun == nullptr;
                    }
                }
            }
        }
    }
    // 근접 종류
    else if (pWeapon->IsA<ACoreMeleeWeapon>())
    {
        AttachWeapon(pWeapon, "EquippedWeaponPosSock", bCheck);
        return pMelee == nullptr;
    }
    // 투척류
    else if (pWeapon->IsA<ACoreThrowableWeapon>())
    {
        // 수류탄을 장착중이지 않을 때만
        if (auto p_customGameInst = UCustomGameInstance::GetInst())
            p_customGameInst->DeleSetItemOntoInventory.ExecuteIfBound(pWeapon, false);

        pWeapon->Destroy();
        return true;
    }
    return false;
}

void AWeaponManager::SwapWorld(ABaseInteraction* pNewWeapon, AActor* pCurrentWeapon, FString SocketName)
{
    Detach(pNewWeapon);
    AttachWeapon(Cast<ABaseInteraction>(pCurrentWeapon), SocketName);
}

// 추가 수정
bool AWeaponManager::Swap(EWeaponType WeaponType, bool bScrolling /* = false */)
{
    if (WeaponType == CurrentWeaponType ||
        WeaponType == NONE)
        return false;

    // 키 숫자 눌렀는데 없을 시
    if (!mArrWeapon[(int)WeaponType])
        return false;

    // 무기 맞교환
    switch (WeaponType)
    {
    case EWeaponType::MELEE: break;
    case EWeaponType::THROWABLE: CreateExplosive(pThrowable); break;

    default:
    {
        int idx = (int)WeaponType;

        // 조준 시 탈착 후 재장착

        // 기존 무기 탈착
        AttachWeapon(mArrWeapon[(int)CurrentWeaponType], mArrSocketName[idx - 1]);        
        // Detach(mArrWeapon[(int)CurrentWeaponType]);

        // 새로운 무기 장착
        AttachWeapon(mArrWeapon[idx], mArrSocketName[idx - 1]);
    }            
    } 
    return true;
}

int AWeaponManager::Swap(ABaseInteraction* pNewWeapon, ABaseInteraction* pCurrentWeapon /* = nullptr */, EWeaponType WeaponType /* = ECurrentWeaponType::NONE */)
{
#define ERROR -1

    if (!pNewWeapon)
        return ERROR;

    switch (WeaponType)
    {
    case FIRST: case SECOND:
    {
        // 타입이 안맞을 시
        if (pNewWeapon->IsA<ACoreMeleeWeapon>()     ||
            pNewWeapon->IsA<ACoreThrowableWeapon>() ||
            pNewWeapon->IsGroupType("HandGun"))
            return ERROR;

        auto p_newWeapon     = Cast<ACoreWeapon>(pNewWeapon);
        auto p_currentWeapon = Cast<ACoreWeapon>(pCurrentWeapon);        

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
        if (pNewWeapon->IsGroupType("HandGun"))
            return ERROR;

        if (pNewWeapon != pPistol)
            SwapWorld(pCurrentWeapon, pNewWeapon, "HandGunSock");
    }
    break;

    case MELEE:
    {
        if (pNewWeapon->IsGroupType("Melee"))
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
    if (auto p_gun = Cast<ACoreWeapon>(GetWeaponByIndex(CurrentWeaponType)))
        p_gun->ChangeShootMode();
}

void AWeaponManager::Drop(EWeaponType WeaponType)
{
    // 무기를 맵에다 드롭
    Detach(GetWeaponByIndex(WeaponType));
    SetNull(WeaponType);
    DeleAim.ExecuteIfBound();
}

void AWeaponManager::SetNull(EWeaponType WeaponType)
{
    switch (WeaponType)
    {
    case EWeaponType::FIRST:     pFirstGun  = nullptr; break;
    case EWeaponType::SECOND:    pSecondGun = nullptr; break;
    case EWeaponType::PISTOL:    pPistol    = nullptr; break;
    case EWeaponType::MELEE:     pMelee     = nullptr; break;
    case EWeaponType::THROWABLE: pThrowable = nullptr; break;
    }
}

void AWeaponManager::SetMeshToPlayerUI(TArray<AActor*> pArrActor)
{
    // 스켈레탈 메쉬를 더미 캐릭터가 장착하게 함.
    for (int i = 1; i < mArrWeapon.Num(); i++)
    {
        // 더미 캐릭터에게 UI 적용
        if (ABaseInteraction* pWeapon = Cast<ACoreWeapon>(pArrActor[i]))
            pWeapon->SetSkeletalMesh((mArrWeapon[i]) ? mArrWeapon[i]->GetSkeletalMesh() : nullptr);
    }
}

void AWeaponManager::DeactivateFiring()
{
    if (auto p_gun = Cast<ACoreWeapon>(GetWeaponByIndex(CurrentWeaponType)))
        p_gun->bShooting = (p_gun->ShootType == EGunShootType::BURST);
}

bool AWeaponManager::IsWrongType(ABaseInteraction* pWeapon, EWeaponType WeaponType, bool bFromWeaponSlot)
{
    if (!pWeapon)
        return true;

    switch (WeaponType)
    {
    // 총기 
    case FIRST:  
    case SECOND: 
    {
        if (pWeapon->IsGroupType("Handgun") ||
            !pWeapon->IsA<ACoreWeapon>()) 
            return true; 
        
        ACoreWeapon* p_gun = (WeaponType == FIRST) ? pFirstGun : pSecondGun;
        return Cast<ACoreWeapon>(pWeapon) == p_gun;
    }
    case PISTOL:    
    {
        if (Cast<ACoreWeapon>(pWeapon) == pPistol)
            return true;

        if (pWeapon->IsGroupType("Handgun"))
            return false;
    }
    case MELEE:     
    {
        if (Cast<ACoreMeleeWeapon>(pWeapon) == pMelee)
            return true;

        if (pWeapon->IsGroupType("Melee"))
            return false;
    }
    case THROWABLE: 
    {
        if (Cast<ACoreThrowableWeapon>(pWeapon) == pThrowable)
            return true;

        if (pWeapon->IsGroupType("Explosive") ||
            pWeapon->IsGroupType("Grenade"))
            return false;
    }
    }
    return true;
}

bool AWeaponManager::IsFiring()
{
    auto p_gun = Cast<ACoreWeapon>(GetWeaponByIndex(CurrentWeaponType));
    return p_gun != nullptr ? p_gun->bShooting : false;
}