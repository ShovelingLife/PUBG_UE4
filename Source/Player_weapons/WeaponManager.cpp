#include "WeaponManager.h"
#include "CoreBullet.h"
#include "CoreMeleeWeapon.h"
#include "CoreThrowableWeapon.h"
#include "CoreWeapon.h"
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
    if (BP_GrenadeEndPoint &&
        pThrowable)
        GrenadeEndPoint = GetWorld()->SpawnActor<AActor>(BP_GrenadeEndPoint, pThrowable->StaticMeshComp->GetSocketTransform("GrenadeThrowSocket"));

    if (GrenadeEndPoint)
        GrenadeEndPoint->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
}

void AWeaponManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    UpdateCurrentWeaponArr();

    // 총기일 때마다 확인
    if (bArrWeaponEquipped[3] &&
        bArrWeaponEquipped[4])
    {
        CheckReloading(DeltaTime);
        CheckContinouslyShooting(DeltaTime);
    }
    if (mbThrowingGrenade)
    {
        mPathTime += DeltaTime;

        if(mPathTime > 0.5f)
        {
            // 경로 제거 및 메쉬 제거
            SplineComp->ClearSplinePoints();

            for (int i = 0; i < arrSplineMeshComp.Num(); i++)
            {
                if (auto splineMeshComp = arrSplineMeshComp[i])
                    splineMeshComp->DestroyComponent();
            }
            arrSplineMeshComp.Empty();
            mPathTime = 0.f;
            mbThrowingGrenade = false;
        }        
    }
}

void AWeaponManager::CheckForEquippedWeapon()
{
    //
}

void AWeaponManager::InitGrenadePath()
{
    SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
    SplineComp->SetMobility(EComponentMobility::Static);
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

bool AWeaponManager::IsAmmoInsufficient(int BulletCount)
{
    bool b_ammo = (BulletCount != 0);

    if (b_ammo)
        Reload();

    else
        PlaySound(EWeaponSoundType::EMPTY_AMMO);

    return b_ammo;
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
    if (CurrentWeaponType == ECurrentWeaponType::NONE)
        return;

    TArray< ABaseInteraction*> p_arrCurrentWeapon
    {
        pFirstGun,
        pSecondGun,
        pPistol,
        pMelee,
        pThrowable
    };
    int index       = (int)CurrentWeaponType;
    int weaponIndex = GetWeaponType(p_arrCurrentWeapon[index]);

    if (weaponIndex == -1)
        return;

    if (auto p_customGameInst = Cast< UCustomGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (auto p_soundManager = p_customGameInst->pSoundManager)
            p_soundManager->PlayWeaponSound(p_arrCurrentWeapon[index]->AudioComp, SoundType, weaponIndex);
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

    pWeapon->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
    auto playerMesh = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetMesh();

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
    // 투척류/근접무기
    else
    {
        if (pWeapon->IsA<ACoreThrowableWeapon>())
        {
            pThrowable = Cast<ACoreThrowableWeapon>(pWeapon);

            if (pThrowable)
            {
                if      (auto p_capsuleColliderComp = pThrowable->GrenadeColliderComp)
                         p_capsuleColliderComp->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);

                else if (auto p_boxColliderComp = pThrowable->ColliderComp)
                         p_boxColliderComp->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);

                if (auto staticMeshComp = pThrowable->StaticMeshComp)
                    staticMeshComp->SetCollisionProfileName("NoCollision");
            }
            CurrentWeaponType = ECurrentWeaponType::THROWABLE;
        }
        else if (pWeapon->IsA<ACoreMeleeWeapon>())
        {
            pMelee            = Cast<ACoreMeleeWeapon>(pWeapon);
            CurrentWeaponType = ECurrentWeaponType::MELEE;
        }
    }
    USkeletalMeshComponent* skeletalMeshComp = pWeapon->SkeletalMeshComp;
    UStaticMeshComponent*   staticMeshComp   = pWeapon->StaticMeshComp;

    // 무기 메시를 플레이어 메시에 부착
    if      (skeletalMeshComp)
             skeletalMeshComp->AttachToComponent(playerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, *SocketName);

    else if (staticMeshComp)
             staticMeshComp->AttachToComponent(playerMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, *SocketName);
}

void AWeaponManager::ResetAfterDetaching(ABaseInteraction* pWeapon, FTransform NewPos)
{
    if (!pWeapon)
        return;

    USkeletalMeshComponent* skeletalMeshComp = pWeapon->SkeletalMeshComp;
    UStaticMeshComponent*   staticMeshComp   = pWeapon->StaticMeshComp;
    
    // 컴포넌트를 탈착 > 현재 루트 컴포넌트에 부착 > 트랜스폼 초기화
    if (skeletalMeshComp)
    {
        skeletalMeshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        skeletalMeshComp->AttachToComponent(pWeapon->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        skeletalMeshComp->ResetRelativeTransform();
    }
    else if (staticMeshComp)
    {
        staticMeshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        staticMeshComp->AttachToComponent(pWeapon->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        staticMeshComp->ResetRelativeTransform();
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
    mPathTime         = 0.f;
    // 투척류 예측 경로 데이터 설정
    FVector socketPos = p_player->GetMesh()->GetSocketLocation("GrenadeThrowSocket");
    FVector launchVelocity;

    if (auto p_playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
        launchVelocity = UKismetMathLibrary::GetForwardVector(p_playerController->GetControlRotation()) * 1000.f;

    RootComponent->SetMobility(EComponentMobility::Movable);
    RootComponent->SetWorldLocation(socketPos);
    RootComponent->SetMobility(EComponentMobility::Static);
    FPredictProjectilePathParams predictParams(25.f, socketPos, launchVelocity, 2.f, ECC_WorldStatic, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
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
        if (auto splineMeshComp = NewObject<USplineMeshComponent>(GetWorld(), USplineMeshComponent::StaticClass()))
        {
            // 위치 설정
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

            splineMeshComp->SetMobility(EComponentMobility::Static);
            splineMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            splineMeshComp->RegisterComponentWithWorld(GetWorld());
            splineMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
            arrSplineMeshComp.Add(splineMeshComp);
        }
    }
    auto length = SplineComp->GetSplineLength();

    // 도착 지점에 생성 및 설정
    if (GrenadeEndPoint)
    {
        GrenadeEndPoint->SetHidden(false);
        GrenadeEndPoint->SetActorRelativeLocation(result.LastTraceDestination.Location);
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
        if (!pThrowable)
            Attach(p_collidedWeapon, p_collidedWeapon->ObjectType + "Socket", bCheck);

        else
        {
            if (auto p_customGameInst = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
            {
                p_customGameInst->DeleSetItemOntoInventory.ExecuteIfBound(p_collidedWeapon, false);
                p_collidedWeapon->Destroy();
            }
        }
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

        // 총알 부족
        if (IsAmmoInsufficient(weaponData.CurrentBulletCount))
            return;

        // 사운드 적용 및 총알 1개 차감
        weaponData.CurrentBulletCount--;
        PlaySound(EWeaponSoundType::SHOT);

        // 레이캐스트 적용
        APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
        FVector               beginPos = p_gun->SkeletalMeshComp->GetSocketLocation("Muzzle_socket");
        FVector               forwardVec = cameraManager->GetActorForwardVector() * 500;
        FHitResult            hitResult;
        FVector               endPos = beginPos + forwardVec;
        GetWorld()->LineTraceSingleByObjectType(hitResult, beginPos, endPos, FCollisionObjectQueryParams(ECC_Pawn));
        FRotator bulletRotation = UKismetMathLibrary::FindLookAtRotation(beginPos, endPos);
        //DrawDebugLine(p_world, begin_pos, end_pos, FColor::Red, true, 5.f, (uint8)0U, 1.f);
        //DrawDebugLine(p_world, begin_pos, end_pos, FColor::Red, true, 5.f, (uint8)0U, 1.f);
        //GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Cyan, bullet_rotation.ToString());
        AActor* p_bullet = nullptr;

        if (p_gun->pBullet)
        {
            // 총알 회전값 수정 요망
            auto test_rotation = FRotator::MakeFromEuler(FVector(endPos.X, bulletRotation.Pitch, bulletRotation.Yaw));
            test_rotation.Yaw += 100.f;
            p_bullet = GetWorld()->SpawnActor(p_gun->pBullet->GetClass(), &beginPos, &bulletRotation);
            //GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Cyan, test_rotation.ToString());
            /*ACore_bullet* core_bullet = Cast<ACore_bullet>(bullet);

            if (core_bullet)
                core_bullet->mesh->SetRelativeRotation(bullet_rotation);*/
        }
        //if(another_character)

        // 파티클 적용
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
            pThrowable->CurrentWeaponType != EThrowableWeaponType::CLAYMORE)
            PredictGrenadePath();
    }
}

void AWeaponManager::Reload()
{
    ABaseInteraction* p_weapon = GetWeaponByIndex(CurrentWeaponType);    

    if (auto p_gun = Cast<ACoreWeapon>(p_weapon))
    {
        auto weaponData = p_gun->WeaponData;

        if (weaponData.CurrentBulletCount != weaponData.MaxBulletCount)
        {
            int result = (weaponData.CurrentBulletCount > 0) ? weaponData.MaxBulletCount - weaponData.CurrentBulletCount : p_gun->WeaponData.MaxBulletCount;
            p_gun->WeaponData.MaxBulletCount     -= result;
            p_gun->WeaponData.CurrentBulletCount += result;
            PlaySound(EWeaponSoundType::RELOAD);
            mbReloading = true;
        }
    }
}

void AWeaponManager::ThrowGrenade()
{
    if (pThrowable)
    {
        pThrowable->Throw(mGrenadeVelocity);
        pThrowable = nullptr;
    }
    mGrenadeVelocity = FVector::ZeroVector;
}

bool AWeaponManager::ScrollSelect(FString Pos)
{
    // 현재 무기 인덱스 갖고와서 선택
    ECurrentWeaponType finalIndex   = ECurrentWeaponType::NONE;
    int                currentIndex = (int)CurrentWeaponType;
    int                totalWeapon  = -1;

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
            finalIndex = GetWeaponIndex("Down", currentIndex - 1);

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
            finalIndex = GetWeaponIndex("Up", currentIndex + 1);

            // 발견하지 못했을 시
            CurrentWeaponType = (finalIndex == ECurrentWeaponType::NONE) ? GetWeaponIndex("Up", 1) : finalIndex;
        }
    }
    return true;
}

void AWeaponManager::SwapWorld(ABaseInteraction* pNewWeapon, AActor* pCurrentWeapon, FString SocketName)
{
    ResetAfterDetaching(pNewWeapon, pCurrentWeapon->GetActorTransform());
    Attach(Cast<ABaseInteraction>(pCurrentWeapon), SocketName);
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
        auto p_newThrowable = Cast<ACoreThrowableWeapon>(pNewWeapon);

        if (!p_newThrowable ||
            (p_newThrowable && p_newThrowable->WeaponData.GroupType != "Explosive"))
            return ERROR;

        if (pThrowable != p_newThrowable)
        {
            pThrowable = p_newThrowable;
            return 1;
        }
        else
            return ERROR;
    }
    break;
    }
    return 0;
}

void AWeaponManager::ChangeShootMode()
{
    auto p_weapon        = GetWeaponByIndex(CurrentWeaponType);
    int  maxGunShootType = 0;

    if (!p_weapon)
        return;

    // 총기일 시
    if (p_weapon->IsA<ACoreWeapon>())
    {
        auto    p_gun = Cast<ACoreWeapon>(p_weapon);
        FString weaponGroup = p_gun->WeaponData.GroupType;

        // 단발/점사/연사 가능
        if      (weaponGroup == "AssaultGun" ||
                 weaponGroup == "SMG")
                 maxGunShootType = (int)EGunShootType::CONSECUTIVE;

        // 단발/점사 가능
        else if (weaponGroup == "ShotGun")
                 maxGunShootType = (int)EGunShootType::BURST;

        // 오직 단발만 가능
        else if (weaponGroup == "HandGun" ||
                 weaponGroup == "SniperGun"  ||
                 weaponGroup == "Special")
                 maxGunShootType = (int)EGunShootType::SINGLE;

        // 격발 방식 변경
        int currentGunShootType = (int)p_gun->ShootType;

        if      (currentGunShootType < maxGunShootType)
                 p_gun->ShootType = (EGunShootType)++currentGunShootType;

        // 초기화
        else if (currentGunShootType == maxGunShootType)
                 p_gun->ShootType = EGunShootType::SINGLE;
    }
}

void AWeaponManager::ChangeAimPose(int _type)
{
    //
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

void AWeaponManager::CheckContinouslyShooting(float TranscurredTime)
{
    if (bShooting)
    {
        // 총기일 경우 쿨타임 적용
        mCurrentShootTime += TranscurredTime;

        if (mCurrentShootTime > mShootTime)
        {
            Shoot();
            mCurrentShootTime = 0.f;
            bShooting = mbChangedShootType;
        }
    }
}

bool AWeaponManager::IsWeaponAvailable(ECurrentWeaponType WeaponType)
{
    auto p_weapon = GetWeaponByIndex(WeaponType);

    if (p_weapon)
        CurrentWeaponType = WeaponType;

    return (p_weapon != nullptr);
}

int AWeaponManager::GetMaxBulletCount(ECurrentWeaponType WeaponType)
{
    ABaseInteraction* p_weapon = GetWeaponByIndex(WeaponType);

    if (p_weapon &&
        p_weapon->IsA<ACoreWeapon>())
        return Cast<ACoreWeapon>(p_weapon)->WeaponData.CurrentMaxBulletCount;

    else
        return 1;
}

ABaseInteraction* AWeaponManager::GetWeaponByIndex(ECurrentWeaponType WeaponType)
{
    ABaseInteraction* p_weapon = nullptr;

    switch (WeaponType)
    {
    case ECurrentWeaponType::FIRST:      p_weapon = pFirstGun;  break;
    case ECurrentWeaponType::SECOND:     p_weapon = pSecondGun; break;
    case ECurrentWeaponType::PISTOL:     p_weapon = pPistol;    break;
    case ECurrentWeaponType::MELEE:      p_weapon = pMelee;     break;
    case ECurrentWeaponType::THROWABLE:  p_weapon = pThrowable; break;
    }
    return p_weapon;
}

ECurrentWeaponType AWeaponManager::GetWeaponIndex(ABaseInteraction* pWeapon)
{
    if      (pWeapon == pFirstGun)
             return ECurrentWeaponType::FIRST;

    else if (pWeapon == pSecondGun)
             return ECurrentWeaponType::SECOND;

    else if (pWeapon == pPistol)
             return ECurrentWeaponType::PISTOL;

    else if (pWeapon == pMelee)
             return ECurrentWeaponType::THROWABLE;

    else if (pWeapon == pThrowable)
             return ECurrentWeaponType::THROWABLE;

    else
             return ECurrentWeaponType::NONE;
}

int AWeaponManager::GetWeaponType(ABaseInteraction* pWeapon)
{
    int weaponType = -1;

    // 총기일시
    if      (auto p_gun = Cast<ACoreWeapon>(pWeapon))
             weaponType = (int)p_gun->WeaponType;

    else if (auto p_melee = Cast<ACoreMeleeWeapon>(pWeapon))
             weaponType = (int)p_melee->CurrentWeaponType;

    else if (auto p_throwable = Cast<ACoreThrowableWeapon>(pWeapon))
             weaponType = (int)p_throwable->CurrentWeaponType;

    return weaponType;
}

void AWeaponManager::Drop(ECurrentWeaponType WeaponType)
{
    auto playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    if (!playerCharacter)
        return;

    ResetAfterDetaching(GetWeaponByIndex(WeaponType), FTransform(playerCharacter->GetActorRotation(), playerCharacter->GetActorLocation() + FVector(0.f, 75.f, 20.f)));
    SetNull(WeaponType);
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
    ACoreWeapon*          p_firstGunUI  = Cast<ACoreWeapon>(pArrActor[(int)ECurrentWeaponType::FIRST]);
    ACoreWeapon*          p_secondGunUI = Cast<ACoreWeapon>(pArrActor[(int)ECurrentWeaponType::SECOND]);
    ACoreWeapon*          p_pistolUI    = Cast<ACoreWeapon>(pArrActor[(int)ECurrentWeaponType::PISTOL]);
    ACoreMeleeWeapon*     p_meleeUI     = Cast<ACoreMeleeWeapon>(pArrActor[(int)ECurrentWeaponType::MELEE]);
    ACoreThrowableWeapon* p_throwableUI = Cast<ACoreThrowableWeapon>(pArrActor[(int)ECurrentWeaponType::THROWABLE]);

    // 첫번째 무기
    if (pFirstGun &&
        p_firstGunUI)
    {
        auto skeletalMeshComp = pFirstGun->SkeletalMeshComp;
        auto skeletalMesh = (pFirstGun->WeaponType != p_firstGunUI->WeaponType) ? skeletalMeshComp->SkeletalMesh : nullptr;
        skeletalMeshComp->SetSkeletalMesh(skeletalMesh);
    }
    // 두번째 무기
    if (pSecondGun &&
        p_secondGunUI)
    {
        auto skeletalMeshComp = p_secondGunUI->SkeletalMeshComp;
        auto skeletalMesh = (pSecondGun->WeaponType != p_secondGunUI->WeaponType) ? skeletalMeshComp->SkeletalMesh : nullptr;
        skeletalMeshComp->SetSkeletalMesh(skeletalMesh);
    }
    // 세번째 무기
    if (pPistol &&
        p_pistolUI)
    {
        auto skeletalMeshComp = p_pistolUI->SkeletalMeshComp;
        auto skeletalMesh = (pPistol->WeaponType != p_pistolUI->WeaponType) ? skeletalMeshComp->SkeletalMesh : nullptr;
        skeletalMeshComp->SetSkeletalMesh(skeletalMesh);
    }
    // 네번째 무기
    if (pMelee &&
        p_meleeUI)
    {
        auto staticMeshComp = p_meleeUI->StaticMeshComp;
        auto staticMesh = (pMelee->CurrentWeaponType != p_meleeUI->CurrentWeaponType) ? staticMeshComp->GetStaticMesh() : nullptr;
        staticMeshComp->SetStaticMesh(staticMesh);
    }
    // 투척무기 일 시
    if (pThrowable &&
        p_throwableUI)
    {
        auto staticMeshComp = p_throwableUI->StaticMeshComp;
        auto staticMesh = (pThrowable->CurrentWeaponType != p_throwableUI->CurrentWeaponType) ? staticMeshComp->GetStaticMesh() : nullptr;
        staticMeshComp->SetStaticMesh(staticMesh);
    }
}

bool AWeaponManager::IsDuplicated(ABaseInteraction* pWeapon, ECurrentWeaponType WeaponType)
{
    auto groupType = pWeapon->ObjectGroupType;
    auto p_gun     = Cast<ACoreWeapon>(pWeapon);
    auto p_melee   = Cast<ACoreMeleeWeapon>(pWeapon);
    auto p_grenade = Cast<ACoreThrowableWeapon>(pWeapon);

    switch (WeaponType)
    {
    // 총기 
    case ECurrentWeaponType::FIRST: 
    case ECurrentWeaponType::SECOND: 
         return (p_gun == pFirstGun || p_gun == pSecondGun || groupType == "Handgun");

    case ECurrentWeaponType::PISTOL:    return (p_gun     == pPistol     || groupType != "Handgun");
    case ECurrentWeaponType::MELEE:     return (p_melee   == pMelee      || groupType != "Melee");
    case ECurrentWeaponType::THROWABLE: return (p_grenade == pThrowable  || groupType != "Throwable");
    case ECurrentWeaponType::NONE:      return (groupType == "Explosive" || groupType == "Melee");
    }
    return true;
}