#include "WeaponManager.h"
#include "CoreBullet.h"
#include "CoreMeleeWeapon.h"
#include "CoreThrowableWeapon.h"
#include "CoreWeapon.h"
#include "CoreBullet.h"
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
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
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
    if (CurrentWeaponType != ECurrentWeaponType::NONE)
    {
        int index = ((int)CurrentWeaponType) - 1;
        
        if (auto p_customGameInst = Cast< UCustomGameInstance>(GetWorld()->GetGameInstance()))
        {
            if (auto p_soundManager = p_customGameInst->pSoundManager)
            {
                if (auto p_weapon = p_arrCurrentWeapon[index])
                    p_soundManager->PlayGunSound(p_weapon->AudioComp, SoundType, GetWeaponType(p_weapon));
            }   
        }
    }
}

ECurrentWeaponType AWeaponManager::GetWeaponIndex(FString Direction, int StartIndex)
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

void AWeaponManager::Attach(ABaseInteraction* pWeapon, FString SocketName, bool bCheck /* = true */)
{
    if (!pWeapon)
        return;

    // 소켓 기반 무기 종류 판별 후 다운캐스팅
    if (SocketName == "HandGunSock")
    {
        pPistol           = Cast<ACoreWeapon>(pWeapon);
        CurrentWeaponType = ECurrentWeaponType::PISTOL;
    }
    else if (SocketName == "FirstGunSock")
    {
        pFirstGun         = Cast<ACoreWeapon>(pWeapon);
        CurrentWeaponType = ECurrentWeaponType::FIRST;
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
        CurrentWeaponType = ECurrentWeaponType::SECOND;
    }
    // 근접무기/투척류
    else
    {
        if (SocketName == "MeleeSock")
        {
            pMelee = Cast<ACoreMeleeWeapon>(pWeapon);
            CurrentWeaponType = ECurrentWeaponType::MELEE;            
        }
        else
            CurrentWeaponType = ECurrentWeaponType::THROWABLE;
    }
    auto p_rootComp = pWeapon->GetRootComponent();
    auto playerMesh = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetMesh();
    pWeapon->AttachToComponent(playerMesh, (p_rootComp->IsA< USkeletalMeshComponent>()) ? FAttachmentTransformRules::SnapToTargetNotIncludingScale : FAttachmentTransformRules::SnapToTargetIncludingScale, *SocketName);
    pWeapon->RegisterAllComponents();
}

void AWeaponManager::ResetAfterDetaching(ABaseInteraction* pWeapon, FTransform NewPos)
{
    if (!pWeapon)
        return;

    UMeshComponent* meshComponent = nullptr;

    if      (pWeapon->SkeletalMeshComp)
             meshComponent = pWeapon->SkeletalMeshComp;
    
    else if (pWeapon->StaticMeshComp)
             meshComponent = pWeapon->StaticMeshComp;

    // 컴포넌트를 탈착 > 현재 루트 컴포넌트에 부착 > 트랜스폼 초기화
    if (meshComponent)
    {
        meshComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        meshComponent->AttachToComponent(pWeapon->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        meshComponent->ResetRelativeTransform();
    }
    // 현재 무기를 탈착 후 월드에 소환
    pWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    pWeapon->SetActorTransform(NewPos);
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
    FVector launchVelocity = UKismetMathLibrary::GetForwardVector(p_player->GetActorRotation()) *  GrenadeDirection * 1500.f;
    FPredictProjectilePathParams predictParams(50.f, socketPos, launchVelocity, 2.f, EObjectTypeQuery::ObjectTypeQuery1);
    predictParams.bTraceWithCollision = true;
    predictParams.SimFrequency     = 15.f;
    predictParams.OverrideGravityZ = 0.f;    
    FPredictProjectilePathResult result;
    UGameplayStatics::PredictProjectilePath(GetWorld(), predictParams, result);
    mGrenadeVelocity = predictParams.LaunchVelocity;
    
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

void AWeaponManager::Equip(AActor* pWeapon, bool bCheck /* = true */)
{
    ABaseInteraction* p_collidedWeapon = Cast<ABaseInteraction>(pWeapon);

    if (!p_collidedWeapon)
        return;
    
    p_collidedWeapon->bPlayerNear = false;

    // 총기 종류
    if (pWeapon->IsA<ACoreWeapon>())
    {
        // 권총일 시 무조건 3번 슬롯
        if (Cast<ACoreWeapon>(pWeapon)->WeaponData.GroupType == "HandGun")
        {
            if (!pPistol)
                Attach(p_collidedWeapon, "HandGunSock", bCheck);

            else
                SwapWorld(pPistol, pWeapon, "HandGunSock");
        }
        // 기타 총기 1,2번 슬롯
        else
        {
            if (!pFirstGun) // 첫번째 무기가 없을 시
                Attach(p_collidedWeapon, "FirstGunSock", bCheck);

            else
            {
                if (!pSecondGun) // 두번째 무기가 없을 시
                    Attach(p_collidedWeapon, "SecondGunSock", bCheck);

                else
                {
                    // 첫번째 무기 장착중
                    if      (CurrentWeaponType == ECurrentWeaponType::FIRST)
                             SwapWorld(pFirstGun, pWeapon, "FirstGunSock");

                    // 두째 무기 장착중
                    else if (CurrentWeaponType == ECurrentWeaponType::SECOND)
                             SwapWorld(pSecondGun, pWeapon, "SecondGunSock");
                }
            }
        }
    }
    // 근접 종류
    else if (pWeapon->IsA<ACoreMeleeWeapon>())
             Attach(p_collidedWeapon, "EquippedWeaponPosSock", bCheck);

    // 투척류
    else if (pWeapon->IsA<ACoreThrowableWeapon>())
    {
        // 수류탄을 장착중이지 않을 때만
        if (auto p_customGameInst = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
            p_customGameInst->DeleSetItemOntoInventory.ExecuteIfBound(p_collidedWeapon, false);

        pWeapon->Destroy();
    }
}

void AWeaponManager::Shoot()
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
        // 사운드 적용 및 총알 1개 차감
        p_gun->WeaponData.CurrentBulletCount--;
        PlaySound(EWeaponSoundType::SHOT);

        // 레이캐스트 적용
        auto        cameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
        FVector     beginPos = p_gun->SkeletalMeshComp->GetSocketLocation("Barrel");
        FVector     forwardVec = cameraManager->GetActorForwardVector() * 500;
        FHitResult  hitResult;
        FVector     endPos = beginPos + forwardVec;
        GetWorld()->LineTraceSingleByObjectType(hitResult, beginPos, endPos, FCollisionObjectQueryParams(ECC_Pawn));
        FRotator bulletRotation = UKismetMathLibrary::FindLookAtRotation(beginPos, endPos);
        GetWorld()->SpawnActor<ACoreBullet>(p_gun->BP_Bullet, beginPos, bulletRotation);
        p_gun->ParticleComp->Activate(true);
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
    ABaseInteraction* p_weapon = GetWeaponByIndex(CurrentWeaponType);    

    if (auto p_gun = Cast<ACoreWeapon>(p_weapon))
    {
        auto& weaponData = p_gun->WeaponData;
        int result = (weaponData.CurrentBulletCount > 0) ? (weaponData.MaxBulletCount - weaponData.CurrentBulletCount) : p_gun->WeaponData.MaxBulletCount;
        weaponData.MaxBulletCount     -= result;
        weaponData.CurrentBulletCount += result;
        PlaySound(EWeaponSoundType::RELOAD);
        mbReloading = true;
    }
}

void AWeaponManager::ThrowGrenade()
{
    if (pThrowable)
    {
        pThrowable->Throw(mGrenadeVelocity);
        pThrowable = nullptr;
        mGrenadeVelocity = FVector::ZeroVector;
        mbThrowingGrenade = false;
        GrenadeDirection = 0.f;

        if (GrenadeEndPoint)
            GrenadeEndPoint->SetHidden(true);
    }
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
    int totalWeapon  = 0;

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
        if (currentIndex == (int)ECurrentWeaponType::FIRST)
            CurrentWeaponType = GetWeaponIndex("Down", (int)ECurrentWeaponType::THROWABLE);

        else
        {
            // 현재 위치에서 탐색
            ECurrentWeaponType finalIndex = GetWeaponIndex("Down", currentIndex - 1);

            // 발견하지 못했을 시
            CurrentWeaponType = (finalIndex == ECurrentWeaponType::NONE) ? GetWeaponIndex("Down", (int)ECurrentWeaponType::THROWABLE) : finalIndex;
        }
    }
    // 위로 스크롤
    else
    {
        // 현재 마지막 원소에 접근 할 시
        if (currentIndex == (int)ECurrentWeaponType::THROWABLE)
            CurrentWeaponType = (pFirstGun) ? ECurrentWeaponType::FIRST : GetWeaponIndex("Up", 1);

        else
        {
            // 현재 위치에서 탐색
            ECurrentWeaponType finalIndex = GetWeaponIndex("Up", currentIndex + 1);

            // 발견하지 못했을 시
            CurrentWeaponType = (finalIndex == ECurrentWeaponType::NONE) ? GetWeaponIndex("Up", 1) : finalIndex;
        }
    }
    Swap(tmpWeaponType, true);
    return true;
}

void AWeaponManager::SwapWorld(ABaseInteraction* pNewWeapon, AActor* pCurrentWeapon, FString SocketName)
{
    ResetAfterDetaching(pNewWeapon, pCurrentWeapon->GetActorTransform());
    Attach(Cast<ABaseInteraction>(pCurrentWeapon), SocketName);
}

void AWeaponManager::Swap(ECurrentWeaponType WeaponType, bool bScrolling /* = false */)
{
    auto playerMesh      = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetMesh();
    auto attachmentRules = FAttachmentTransformRules::SnapToTargetIncludingScale;
    auto typeToAttach    = (bScrolling) ? CurrentWeaponType : WeaponType;
    auto typeToDettach   = (bScrolling) ? WeaponType : CurrentWeaponType;

    // 장착하고자 하는 무기를 장착
    switch (typeToAttach)
    {
    case ECurrentWeaponType::FIRST:   pFirstGun->AttachToComponent(playerMesh, attachmentRules, "EquippedWeaponPosSock"); break;
    case ECurrentWeaponType::SECOND: pSecondGun->AttachToComponent(playerMesh, attachmentRules, "EquippedWeaponPosSock"); break;
    case ECurrentWeaponType::PISTOL:    pPistol->AttachToComponent(playerMesh, attachmentRules, "EquippedWeaponPosSock"); break;
    case ECurrentWeaponType::MELEE: break;
    case ECurrentWeaponType::THROWABLE: pThrowable->AttachToComponent(playerMesh, attachmentRules, *(pThrowable->WeaponData.Type + "Sock")); break;
    }
    // 장착 되있던 무기를 탈착
    switch (typeToDettach)
    {
    case ECurrentWeaponType::FIRST:   pFirstGun->AttachToComponent(playerMesh, attachmentRules, "FirstGunSock");  break;
    case ECurrentWeaponType::SECOND: pSecondGun->AttachToComponent(playerMesh, attachmentRules, "SecondGunSock"); break;
    case ECurrentWeaponType::PISTOL:    pPistol->AttachToComponent(playerMesh, attachmentRules, "HandGunSock");   break;
    case ECurrentWeaponType::MELEE: break;
    case ECurrentWeaponType::THROWABLE: break;
    }
    CurrentWeaponType = typeToAttach;
}

int AWeaponManager::Swap(ABaseInteraction* pNewWeapon, ABaseInteraction* pCurrentWeapon /* = nullptr */, ECurrentWeaponType WeaponType /* = ECurrentWeaponType::NONE */)
{
#define ERROR -1

    switch (WeaponType)
    {
    case ECurrentWeaponType::FIRST: case ECurrentWeaponType::SECOND:
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
            Attach(p_newWeapon,     "SecondGunSock", false);
            Attach(p_currentWeapon, "FirstGunSock",  false);
        }
        // 두번째 총과 첫번째 총 교체
        else if (p_currentWeapon == pFirstGun &&
                 p_newWeapon     == pSecondGun)
        {
            Attach(p_currentWeapon, "SecondGunSock", false);
            Attach(p_newWeapon,     "FirstGunSock",  false);
        }
        // 인벤토리 총과 첫번째 총 교체
        else
        {
            FString sockName = (WeaponType == ECurrentWeaponType::FIRST) ? "FirstGunSock" : "SecondGunSock";
            SwapWorld(p_currentWeapon, p_newWeapon, sockName);
        }
    }
    break;

    case ECurrentWeaponType::PISTOL:
    {
        auto p_newWeapon = Cast<ACoreWeapon>(pNewWeapon);

        if (!p_newWeapon || 
            (p_newWeapon && p_newWeapon->WeaponData.GroupType != "HandGun"))
            return ERROR;

        if (p_newWeapon != pPistol)
            SwapWorld(pCurrentWeapon, p_newWeapon, "HandGunSock");
    }
    break;

    case ECurrentWeaponType::MELEE:
    {
        auto p_newMelee = Cast<ACoreMeleeWeapon>(pNewWeapon);

        if (!p_newMelee ||
            (p_newMelee && p_newMelee->WeaponData.GroupType != "Melee"))
            return ERROR;
    }
    break;

    case ECurrentWeaponType::THROWABLE:
    {
        auto newThrowable = Cast<ACoreThrowableWeapon>(pNewWeapon);

        if (!newThrowable)
            return ERROR;

        // 오브젝트 생성 후 투척류로 지정
        if (!pThrowable)
            pThrowable = GetWorld()->SpawnActor<ACoreThrowableWeapon>(ACoreThrowableWeapon::StaticClass());
        
        pThrowable->Setup(newThrowable);
        Attach(pThrowable, pThrowable->WeaponData.Type + "Sock");
    }
    break;
    }
    return 0;
}

void AWeaponManager::ChangeShootMode()
{
    auto p_weapon     = GetWeaponByIndex(CurrentWeaponType);
    int  maxShootType = 0;

    // 총기일 시
    if (p_weapon &&
        p_weapon->IsA<ACoreWeapon>())
    {
        auto    p_gun = Cast<ACoreWeapon>(p_weapon);
        FString weaponGroup = p_gun->WeaponData.GroupType;

        // 단발/점사/연사 가능
        if      (weaponGroup == "AssaultGun" ||
                 weaponGroup == "SMG")
                 maxShootType = (int)EGunShootType::CONSECUTIVE;

        // 단발/점사 가능
        else if (weaponGroup == "ShotGun")
                 maxShootType = (int)EGunShootType::BURST;

        // 오직 단발만 가능
        else if (weaponGroup == "HandGun"   ||
                 weaponGroup == "SniperGun" ||
                 weaponGroup == "Special")
                 maxShootType = (int)EGunShootType::SINGLE;

        // 격발 방식 변경
        int currentGunShootType = (int)p_gun->ShootType;
        p_gun->ShootType = (currentGunShootType < maxShootType) ? (EGunShootType)++currentGunShootType : EGunShootType::SINGLE;
        
        // 팝업 UI 설정
        FString	shootTypeStr = "";

        switch (p_gun->ShootType) // 현재 격발 상태에 따라 
        {
        case EGunShootType::SINGLE:		 shootTypeStr = "Single";	   break;
        case EGunShootType::BURST:		 shootTypeStr = "Burst";	   break;
        case EGunShootType::CONSECUTIVE: shootTypeStr = "Consecutive"; break;
        }
        if (auto p_gameInst = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
            p_gameInst->DeleSetShootTypeNotificationTxt.ExecuteIfBound("Current type : " + shootTypeStr);
    }
}

void AWeaponManager::ChangeAimPose(bool bAiming)
{
    // 현재 착용 중인 무기를 가지고옴
    ACoreWeapon* p_gun = nullptr;
    FString socketName = "";

    switch (CurrentWeaponType)
    {
    case ECurrentWeaponType::FIRST:  p_gun = pFirstGun;  socketName = "FirstGunSock";  break;
    case ECurrentWeaponType::SECOND: p_gun = pSecondGun; socketName = "SecondGunSock"; break;
    case ECurrentWeaponType::PISTOL: p_gun = pPistol;    socketName = "HandGunSock";   break;
    }
    if (!p_gun)
        return;

    // 캐릭터 메쉬에다 부착
    if (auto p_character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
    {
        if (USkeletalMeshComponent* skeletalMeshComp = p_character->GetMesh())
            p_gun->AttachToComponent(skeletalMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, *((bAiming) ? "EquippedWeaponPosSock" : socketName));
    }
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
            static bool bShooted = false;
            float timeToWait = 0.25f;
            mCurrentShootTime += TranscurredTime;

            switch (p_gun->ShootType)
            {
            case EGunShootType::SINGLE:

                Shoot();
                bShooting = false;
                break;

            case EGunShootType::BURST:

                if (mCurrentShootTime > timeToWait)
                {
                    Shoot();
                    mCurrentShootTime = 0.f;
                    mBurstCount++;
                }
                if (mBurstCount == 3)
                {
                    mBurstCount = 0;
                    bShooting = false;
                }
                break;

                // 쿨타임 적용
            case EGunShootType::CONSECUTIVE:

                if (mCurrentShootTime > timeToWait)
                {
                    Shoot();
                    mCurrentShootTime = 0.f;
                }
                break;

            default: mCurrentShootTime = 0.f;
            }
        }
    }
    else
    {
        mCurrentShootTime = 0.f;
        mBurstCount = 0;
    }
}

//int AWeaponManager::GetMaxBulletCount(ECurrentWeaponType WeaponType)
//{
//    ABaseInteraction* p_weapon = GetWeaponByIndex(WeaponType);
//
//    if (p_weapon &&
//        p_weapon->IsA<ACoreWeapon>())
//        return Cast<ACoreWeapon>( p_weapon)->WeaponData.CurrentMaxBulletCount;
//
//    else
//        return 1;
//}

ABaseInteraction* AWeaponManager::GetWeaponByIndex(ECurrentWeaponType WeaponType)
{
    if (WeaponType == ECurrentWeaponType::NONE)
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
    int index = (int)WeaponType;
    return arrWeapon[index - 1];
}

ACoreWeapon* AWeaponManager::GetCurrentWeapon()
{
    return Cast<ACoreWeapon>(GetWeaponByIndex(CurrentWeaponType));
}

ECurrentWeaponType AWeaponManager::GetWeaponIndex(ABaseInteraction* pWeapon)
{
    // 원소랑 일치한 데이터를 찾음
    TArray<Chaos::Pair<ABaseInteraction*, ECurrentWeaponType>> arrCurrentWeaponType
    {
        Chaos::MakePair<ABaseInteraction*, ECurrentWeaponType>(nullptr,    ECurrentWeaponType::NONE),
        Chaos::MakePair<ABaseInteraction*, ECurrentWeaponType>(pFirstGun,  ECurrentWeaponType::FIRST),
        Chaos::MakePair<ABaseInteraction*, ECurrentWeaponType>(pSecondGun, ECurrentWeaponType::SECOND),
        Chaos::MakePair<ABaseInteraction*, ECurrentWeaponType>(pPistol,    ECurrentWeaponType::PISTOL),
        Chaos::MakePair<ABaseInteraction*, ECurrentWeaponType>(pMelee,     ECurrentWeaponType::MELEE),
        Chaos::MakePair<ABaseInteraction*, ECurrentWeaponType>(pThrowable, ECurrentWeaponType::THROWABLE)
    };
    for (auto item : arrCurrentWeaponType )
    {
        if (item.First == pWeapon)
            return item.Second;
    }
    return ECurrentWeaponType::NONE;
}

int AWeaponManager::GetWeaponType(ABaseInteraction* pWeapon)
{
    int weaponType = 0;

    // 총기일시
    if      (auto p_gun = Cast<ACoreWeapon>(pWeapon))
             weaponType = (int)p_gun->WeaponType;

    else if (auto p_melee = Cast<ACoreMeleeWeapon>(pWeapon))
             weaponType = (int)p_melee->WeaponType;

    else if (auto p_throwable = Cast<ACoreThrowableWeapon>(pWeapon))
             weaponType = (int)p_throwable->WeaponType;

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
    case ECurrentWeaponType::FIRST:     pFirstGun  = nullptr; break;
    case ECurrentWeaponType::SECOND:    pSecondGun = nullptr; break;
    case ECurrentWeaponType::PISTOL:    pPistol    = nullptr; break;
    case ECurrentWeaponType::MELEE:     pMelee     = nullptr; break;
    case ECurrentWeaponType::THROWABLE: pThrowable = nullptr; break;
    }
}

void AWeaponManager::SetMeshToPlayerUI(TArray<AActor*> pArrActor, USkeletalMeshComponent* SkeletalMeshComp)
{
    ACoreWeapon*          p_firstGunUI  = Cast<ACoreWeapon>(         pArrActor[(int)ECurrentWeaponType::FIRST]);
    ACoreWeapon*          p_secondGunUI = Cast<ACoreWeapon>(         pArrActor[(int)ECurrentWeaponType::SECOND]);
    ACoreWeapon*          p_pistolUI    = Cast<ACoreWeapon>(         pArrActor[(int)ECurrentWeaponType::PISTOL]);
    ACoreMeleeWeapon*     p_meleeUI     = Cast<ACoreMeleeWeapon>(    pArrActor[(int)ECurrentWeaponType::MELEE]);
    ACoreThrowableWeapon* p_throwableUI = Cast<ACoreThrowableWeapon>(pArrActor[(int)ECurrentWeaponType::THROWABLE]);

    p_firstGunUI->SkeletalMeshComp->SetSkeletalMesh( (pFirstGun)  ? pFirstGun->SkeletalMeshComp->SkeletalMesh   : nullptr);
    p_secondGunUI->SkeletalMeshComp->SetSkeletalMesh((pSecondGun) ? pSecondGun->SkeletalMeshComp->SkeletalMesh  : nullptr);
    p_pistolUI->SkeletalMeshComp->SetSkeletalMesh(   (pPistol)    ? pPistol->SkeletalMeshComp->SkeletalMesh     : nullptr);
    p_meleeUI->StaticMeshComp->SetStaticMesh(        (pMelee)     ? pMelee->StaticMeshComp->GetStaticMesh()     : nullptr);
    p_throwableUI->StaticMeshComp->SetStaticMesh(    (pThrowable) ? pThrowable->StaticMeshComp->GetStaticMesh() : nullptr);
}

bool AWeaponManager::IsWrong(ABaseInteraction* pWeapon, ECurrentWeaponType WeaponType, bool bFromWeaponSlot)
{
    if (!pWeapon)
        return true;

    auto groupType = pWeapon->ObjectGroupType;
    
    switch (WeaponType)
    {
    // 총기 
    case ECurrentWeaponType::FIRST: 
    case ECurrentWeaponType::SECOND:
    {
        auto p_gun = Cast<ACoreWeapon>(pWeapon);
        return (bFromWeaponSlot) ? (groupType == "Handgun") : (p_gun == pFirstGun || p_gun == pSecondGun);
    }
    case ECurrentWeaponType::PISTOL:    return (Cast<ACoreWeapon>(pWeapon)          == pPistol    || groupType != "HandGun");
    case ECurrentWeaponType::MELEE:     return (Cast<ACoreMeleeWeapon>(pWeapon)     == pMelee     || groupType != "Melee");
    case ECurrentWeaponType::THROWABLE: return (Cast<ACoreThrowableWeapon>(pWeapon) == pThrowable || groupType != "Explosive");
    }
    return true;
}