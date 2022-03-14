﻿#include "WeaponManager.h"
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
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"

AWeaponManager::AWeaponManager()
{
	PrimaryActorTick.bCanEverTick = true;
    SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene_comp"));
    RootComponent = SceneComp;
}

void AWeaponManager::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponManager::Tick(float _delta_time)
{
	Super::Tick(_delta_time);
    UpdateCurrentWeaponArr();

    // 총기일 때마다 확인
    if (bArrWeaponEquipped[3] &&
        bArrWeaponEquipped[4])
    {
        CheckIfReloading(_delta_time);
        CheckContinouslyShooting(_delta_time);
    }
}

void AWeaponManager::Check_for_equipped_weapon()
{
    //
}

void AWeaponManager::UpdateCurrentWeaponArr()
{
    TArray< ABaseInteraction*> ArrCurrentWeapon
    {
        pFirstGun,
        pSecondGun,
        pPistol,
        pMelee,
        pThrowable
    };
    for (int i = 0; i < ArrCurrentWeapon.Num(); i++)
         bArrWeaponEquipped[i] = (ArrCurrentWeapon[i] != nullptr);
}

void AWeaponManager::PlaySound(EWeaponSoundType _SoundType)
{
    // 오디오 컴포넌트 가져오기    
    UAudioComponent* p_AudioComp = nullptr;
    int              weapon_index = 0;

    // 첫번째 무기
    if (bArrWeaponEquipped[0])
    {
        p_AudioComp  = pFirstGun->AudioComp;
        weapon_index = (int)pFirstGun->WeaponType;
    }
    // 두번째 무기
    else if (bArrWeaponEquipped[1])
    {
        p_AudioComp  = pSecondGun->AudioComp;
        weapon_index = (int)pSecondGun->WeaponType;
    }
    // 세번째 무기
    else if (bArrWeaponEquipped[2])
    {
        p_AudioComp  = pPistol->AudioComp;
        weapon_index = (int)pPistol->WeaponType;
    }
    // 네번째 무기
    else if (bArrWeaponEquipped[3])
    {
        p_AudioComp  = pMelee->AudioComp;
        weapon_index = (int)pMelee->WeaponType;
    }
    // 다섯번째 무기
    else if (bArrWeaponEquipped[4])
    {
        p_AudioComp  = pThrowable->AudioComp;
        weapon_index = (int)pThrowable->WeaponType;
    }
    /*if (_sound_type != e_weapon_sound_type::SHOT)
        AGlobal::Get_sound_manager()->Play_weapon_sound(p_audio_comp, _sound_type);

    else
        AGlobal::Get_sound_manager()->Play_weapon_sound(p_audio_comp, _sound_type, weapon_index);*/
}

ECurrentWeaponType AWeaponManager::GetWeaponIndex(FString _Direction, int _StartIndex)
{
    // 위에서 아래
    if (_Direction == "down")
    {
        for (int i = _StartIndex - 1; i > -1; i--)
        {
            if (bArrWeaponEquipped[i])
                return (ECurrentWeaponType)(i + 1);
        }
    }
    // 아래에서 위
    else
    {
        for (int i = _StartIndex - 1; i < 5; i++)
        {
            if (bArrWeaponEquipped[i])
                return (ECurrentWeaponType)(i + 1);
        }
    }
    return ECurrentWeaponType::NONE;
}

void AWeaponManager::Attach(ABaseInteraction* _pWeapon, FString _SocketName, bool _bShouldCheck)
{
    if (!_pWeapon)
        return;

    _pWeapon->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
    auto playerMesh = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetMesh();

    // 소켓 기반 무기 종류 판별 후 다운캐스팅
    if (_SocketName == "HandGunSock")
    {
        pPistol           = Cast<ACoreWeapon>(_pWeapon);
        CurrentWeaponType = ECurrentWeaponType::PISTOL;
    }
    else if (_SocketName == "FirstGunSock")
    {
        pFirstGun         = Cast<ACoreWeapon>(_pWeapon);
        CurrentWeaponType = ECurrentWeaponType::FIRST;
    }
    else if (_SocketName == "SecondGunSock")
    {
        // 중복 무기 방지
        if (_bShouldCheck)
        {
            if (pFirstGun == Cast<ACoreWeapon>(_pWeapon))
                return;
        }
        pSecondGun        = Cast<ACoreWeapon>(_pWeapon);
        CurrentWeaponType = ECurrentWeaponType::SECOND;
    }
    // 투척류/근접무기
    else
    {
        if (_pWeapon->IsA<ACoreThrowableWeapon>())
        {
            pThrowable = Cast<ACoreThrowableWeapon>(_pWeapon);

            if (pThrowable)
            {
                if      (auto p_capsuleColliderComp = pThrowable->GrenadeColliderComp)
                         p_capsuleColliderComp->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);

                else if (auto p_boxColliderComp = pThrowable->ColliderComp)
                         p_boxColliderComp->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
            }
            CurrentWeaponType = ECurrentWeaponType::THROWABLE;
        }
        else if (_pWeapon->IsA<ACoreMeleeWeapon>())
        {
            pMelee            = Cast<ACoreMeleeWeapon>(_pWeapon);
            CurrentWeaponType = ECurrentWeaponType::MELEE;
        }
    }

    USkeletalMeshComponent* skeletalMeshComp = _pWeapon->SkeletalMeshComp;
    UStaticMeshComponent*   staticMeshComp   = _pWeapon->StaticMeshComp;

    // 무기 메시를 플레이어 메시에 부착
    if      (skeletalMeshComp)
             skeletalMeshComp->AttachToComponent(playerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, *_SocketName);

    else if (staticMeshComp)
             staticMeshComp->AttachToComponent(playerMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, *_SocketName);
}

void AWeaponManager::ResetAfterDetaching(ABaseInteraction* _pWeapon, FTransform _NewPos)
{
    if (!_pWeapon)
        return;

    USkeletalMeshComponent* skeletalMeshComp = _pWeapon->SkeletalMeshComp;
    UStaticMeshComponent*   staticMeshComp   = _pWeapon->StaticMeshComp;
    
    // 컴포넌트를 탈착 > 현재 루트 컴포넌트에 부착 > 트랜스폼 초기화
    if (skeletalMeshComp)
    {
        skeletalMeshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        skeletalMeshComp->AttachToComponent(_pWeapon->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        skeletalMeshComp->ResetRelativeTransform();
    }
    else if (staticMeshComp)
    {
        staticMeshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        staticMeshComp->AttachToComponent(_pWeapon->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        staticMeshComp->ResetRelativeTransform();
    }
    // 현재 무기를 탈착 후 월드에 소환
    _pWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    _pWeapon->SetActorTransform(_NewPos);
}

void AWeaponManager::CheckForGrenadePath()
{
    if (!pThrowable)
        return;
    //UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation().Pitch / 360.f;
    // 투척류 예측 경로 데이터 설정
    FVector outVelocity = FVector::ZeroVector;
    auto    controllerRotation = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation();
    FVector targetPos = FVector(GrenadePathPredictPos.X, GrenadePathPredictPos.Y + controllerRotation.Pitch, GrenadePathPredictPos.Z + controllerRotation.Yaw);
    GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, controllerRotation.ToString());
    GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Blue, targetPos.ToString());
    
    // UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetActorForwardVector().Z * 0.5f
    if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, outVelocity, GrenadePathPredictPos, targetPos, GetWorld()->GetGravityZ()))
    {
        FPredictProjectilePathParams predictParams(2.f, GrenadePathPredictPos, outVelocity, 5.f);   
        predictParams.DrawDebugTime = 3.f; //디버그 라인 보여지는 시간 (초)     
        predictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;  // DrawDebugTime 을 지정하면 EDrawDebugTrace::Type::ForDuration 필요.
        predictParams.OverrideGravityZ = GetWorld()->GetGravityZ();
        FPredictProjectilePathResult result;
        UGameplayStatics::PredictProjectilePath(this, predictParams, result);

        // 수류탄 투척
        if (auto colliderComp = pThrowable->GrenadeColliderComp)
        {
            colliderComp->SetSimulatePhysics(true);
            colliderComp->AddImpulse(outVelocity);
        }
    }
}

void AWeaponManager::Equip(AActor* _pWeapon, bool _bShouldCheck)
{
    ABaseInteraction* p_collidedWeapon = Cast<ABaseInteraction>(_pWeapon);

    if (!p_collidedWeapon)
        return;
    
    p_collidedWeapon->bPlayerNear = false;

    // 총기 종류
    if (_pWeapon->IsA<ACoreWeapon>())
    {
        // 권총일 시 무조건 3번 슬롯
        if (Cast<ACoreWeapon>(_pWeapon)->WeaponData.GroupType == "HandGun")
        {
            if (!pPistol)
                Attach(p_collidedWeapon, "HandGunSock", _bShouldCheck);

            else
                SwapWorld(pPistol, _pWeapon, "HandGunSock");
        }
        // 기타 총기 1,2번 슬롯
        else
        {
            if (!pFirstGun) // 첫번째 무기가 없을 시
                Attach(p_collidedWeapon, "FirstGunSock", _bShouldCheck);

            else
            {
                if (!pSecondGun) // 두번째 무기가 없을 시
                    Attach(p_collidedWeapon, "SecondGunSock", _bShouldCheck);

                else
                {
                    // 첫번째 무기 장착중
                    if      (CurrentWeaponType == ECurrentWeaponType::FIRST)
                             SwapWorld(pFirstGun, _pWeapon, "FirstGunSock");

                    // 두째 무기 장착중
                    else if (CurrentWeaponType == ECurrentWeaponType::SECOND)
                             SwapWorld(pSecondGun, _pWeapon, "SecondGunSock");
                }
            }
        }
    }
    // 근접 종류
    else if (_pWeapon->IsA<ACoreMeleeWeapon>())
             Attach(p_collidedWeapon, "EquippedWeaponPosSock", _bShouldCheck);

    // 투척류
    else if (_pWeapon->IsA<ACoreThrowableWeapon>())
    {
        // 수류탄을 장착중이지 않을 때만
        if (!pThrowable)
            Attach(p_collidedWeapon, p_collidedWeapon->ObjectType + "Socket", _bShouldCheck);

        else
        {
            if (auto p_customGameInst = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
            {
                p_customGameInst->DeleSetItemOntoInventory.ExecuteIfBound(p_collidedWeapon);
                p_collidedWeapon->Destroy();
            }
        }
    }
}

void AWeaponManager::Shoot()
{
    ABaseInteraction* p_weapon = GetWeaponByIndex(CurrentWeaponType);

    if (!p_weapon)
        return;

    if (p_weapon->IsA<ACoreWeapon>())
    {
        if (mbReloading)
            return;

        auto p_gun = Cast<ACoreWeapon>(p_weapon);
        auto weaponData = p_gun->WeaponData;

        // 총알 부족
        if (weaponData.CurrentBulletCount == 0)
        {
            if (weaponData.CurrentBulletCount == 0)
                PlaySound(EWeaponSoundType::EMPTY_AMMO);

            else
                Reload();

            return;
        }
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
        CheckForGrenadePath();
}

void AWeaponManager::Reload()
{
    ABaseInteraction* p_weapon = GetWeaponByIndex(CurrentWeaponType);    

    if (auto p_gun = Cast<ACoreWeapon>(p_weapon))
    {
        int result       = 0;
        mbReloading   = true;
        auto weaponData = p_gun->WeaponData;

        // 허용 총알 개수가 똑같을 시
        if      (weaponData.CurrentBulletCount == weaponData.MaxBulletCount)
                 return;

        // 중간에 장전
        else if (weaponData.CurrentBulletCount > 0)
                 result = weaponData.MaxBulletCount - weaponData.CurrentBulletCount;

        // 전체 총알 소모 후 장전
        else
                 result = p_gun->WeaponData.MaxBulletCount;

        weaponData.MaxBulletCount     -= result;
        weaponData.CurrentBulletCount += result;
        PlaySound(EWeaponSoundType::RELOAD);
    }
}

bool AWeaponManager::ScrollSelect(FString _Pos)
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
    if (_Pos == "Down")
    {
        // 마지막 원소 0 도달 시
        if (currentIndex == (int)ECurrentWeaponType::FIRST)
            CurrentWeaponType = GetWeaponIndex("down", (int)ECurrentWeaponType::THROWABLE);

        else
        {
            // 현재 위치에서 탐색
            finalIndex = GetWeaponIndex("down", currentIndex - 1);

            // 발견하지 못함
            if (finalIndex == ECurrentWeaponType::NONE)
                CurrentWeaponType = GetWeaponIndex("down", (int)ECurrentWeaponType::THROWABLE);

            else
                CurrentWeaponType = finalIndex;
        }
    }
    // 위로 스크롤
    else
    {
        // 현재 마지막 원소에 접근 할 시
        if (currentIndex == (int)ECurrentWeaponType::THROWABLE)
        {
            if (pFirstGun)
                CurrentWeaponType = ECurrentWeaponType::FIRST;

            else
                CurrentWeaponType = GetWeaponIndex("up", 1);
        }
        else
        {
            // 현재 위치에서 탐색
            finalIndex = GetWeaponIndex("up", currentIndex + 1);

            // 발견하지 못함
            if (finalIndex == ECurrentWeaponType::NONE)
                CurrentWeaponType = GetWeaponIndex("up", 1);

            else
                CurrentWeaponType = finalIndex;
        }
    }
    return true;
}

void AWeaponManager::SwapWorld(ABaseInteraction* _CurrentWeapon, AActor* _NewWeapon, FString _SocketName)
{
    ResetAfterDetaching(_CurrentWeapon, _NewWeapon->GetActorTransform());
    Attach(Cast<ABaseInteraction>(_NewWeapon), _SocketName);
}

bool AWeaponManager::Swap(ABaseInteraction* _pOldWeapon, ABaseInteraction* _pNewWeapon, ECurrentWeaponType _WeaponType)
{
    auto    p_oldWeapon = Cast<ACoreWeapon>(_pOldWeapon);
    auto    p_newWeapon = Cast<ACoreWeapon>(_pNewWeapon);
    FString weaponType  = "";

    if (p_oldWeapon)
        weaponType = p_oldWeapon->WeaponData.GroupType;

    switch (_WeaponType)
    {
    case ECurrentWeaponType::FIRST:
    case ECurrentWeaponType::SECOND:

        if (weaponType == "HandGun" ||
            weaponType == "Melee" ||
            weaponType == "Explosive")
            return false;

        // 첫번째 총과 두번째 총 교체
        if (p_oldWeapon == pFirstGun &&
            p_newWeapon == pSecondGun)
        {
            Attach(p_oldWeapon, "SecondGunSock", false);
            Attach(p_newWeapon, "FirstGunSock", false);
        }
        // 두번째 총과 첫번째 총 교체
        else if (p_newWeapon == pFirstGun &&
                 p_oldWeapon == pSecondGun)
        {
            Attach(p_newWeapon, "SecondGunSock", false);
            Attach(p_oldWeapon, "FirstGunSock", false);
        }
        // 인벤토리 총과 첫번째 총 교체
        else
        {
            FString sockName = (_WeaponType == ECurrentWeaponType::FIRST) ? "FirstGunSock" : "SecondGunSock";
            SwapWorld(p_newWeapon, p_oldWeapon, sockName);
        }
        break;

    case ECurrentWeaponType::PISTOL:

        if (weaponType != "HandGun")
            return false;

        if (p_oldWeapon != pPistol)
            SwapWorld(p_newWeapon, p_oldWeapon, "HandGunSock");

        break;

    case ECurrentWeaponType::MELEE:

        if (weaponType != "Melee")
            return false;

        break;

    case ECurrentWeaponType::THROWABLE:

        if (weaponType != "Explosive")
            return false;

        break;
    }
    return true;
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

        if      ((int)p_gun->ShootType < maxGunShootType)
                 p_gun->ShootType = (EGunShootType)++currentGunShootType;

        // 초기화
        else if ((int)p_gun->ShootType == maxGunShootType)
                 p_gun->ShootType = EGunShootType::SINGLE;
    }
}

void AWeaponManager::ChangeAimPose(int _type)
{
    //
}

void AWeaponManager::CheckIfReloading(float _TranscurredReloadTime)
{
    // 재장전 중
    if (mbReloading)
    {
        mCurrentReloadTime += _TranscurredReloadTime;

        if (mCurrentReloadTime > mkReloadTime)
        {
            mCurrentReloadTime = 0.f;
            mbReloading        = false;
        }
    }
}

void AWeaponManager::CheckContinouslyShooting(float _TranscurredShootTime)
{
    if (bShooting)
    {
        // 총기일 경우 쿨타임 적용
        mCurrentShootTime += _TranscurredShootTime;

        if (mCurrentShootTime > mShootTime)
        {
            Shoot();
            mCurrentShootTime = 0.f;
            bShooting = mbChangedShootType;
        }
    }
}

bool AWeaponManager::IsWeaponAvailable(ECurrentWeaponType _WeaponType)
{
    auto p_weapon = GetWeaponByIndex(_WeaponType);

    if (p_weapon)
    {
        CurrentWeaponType = _WeaponType;
        return true;
    }
    else
        return false;
}

int AWeaponManager::GetMaxBulletCount(ECurrentWeaponType _WeaponType)
{
    ABaseInteraction* p_weapon = GetWeaponByIndex(_WeaponType);

    if (!p_weapon)
        return 0;

    if (p_weapon->IsA<ACoreWeapon>())
        return Cast<ACoreWeapon>(p_weapon)->WeaponData.CurrentMaxBulletCount;

    else
        return 1;
}

ABaseInteraction* AWeaponManager::GetWeaponByIndex(ECurrentWeaponType _WeaponType)
{
    ABaseInteraction* p_weapon = nullptr;

    switch (_WeaponType)
    {
    case ECurrentWeaponType::FIRST:      p_weapon = pFirstGun;  break;
    case ECurrentWeaponType::SECOND:     p_weapon = pSecondGun; break;
    case ECurrentWeaponType::PISTOL:     p_weapon = pPistol;    break;
    case ECurrentWeaponType::MELEE:      p_weapon = pMelee;     break;
    case ECurrentWeaponType::THROWABLE:  p_weapon = pThrowable; break;
    }
    return p_weapon;
}

ECurrentWeaponType AWeaponManager::GetWeaponIndex(ABaseInteraction* _pWeapon)
{
    if      (_pWeapon == pFirstGun)
             return ECurrentWeaponType::FIRST;

    else if (_pWeapon == pSecondGun)
             return ECurrentWeaponType::SECOND;

    else if (_pWeapon == pPistol)
             return ECurrentWeaponType::PISTOL;

    else if (_pWeapon == pMelee)
             return ECurrentWeaponType::THROWABLE;

    else if (_pWeapon == pThrowable)
             return ECurrentWeaponType::THROWABLE;

    else
             return ECurrentWeaponType::NONE;
}

int AWeaponManager::GetWeaponType(ABaseInteraction* _pWeapon)
{
    // 총기일시
    if      (auto p_gun = Cast<ACoreWeapon>(_pWeapon))
             return (int)p_gun->WeaponType;

    else if (auto p_melee = Cast<ACoreMeleeWeapon>(_pWeapon))
             return (int)p_melee->WeaponType;

    else if (auto p_throwable = Cast<ACoreThrowableWeapon>(_pWeapon))
             return (int)p_throwable->WeaponType;

    else
        return -1;
}

void AWeaponManager::Drop(ECurrentWeaponType _WeaponType)
{
    auto p_weapon = GetWeaponByIndex(_WeaponType);

    if (!p_weapon)
        return;

    FVector  finalLocation  = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
    FRotator playerRotation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorRotation();
    finalLocation.Y        += 75.f;
    finalLocation.Z         = 20.f;
    FTransform new_location = FTransform(playerRotation, finalLocation);
    ResetAfterDetaching(p_weapon, new_location);
    SetNull(_WeaponType);
}

void AWeaponManager::SetNull(ECurrentWeaponType _WeaponType)
{
    // 현재 보유 중인 무기랑 비교
    switch (_WeaponType)
    {
    case ECurrentWeaponType::FIRST:     pFirstGun  = nullptr; break;
    case ECurrentWeaponType::SECOND:    pSecondGun = nullptr; break;
    case ECurrentWeaponType::PISTOL:    pPistol     = nullptr; break;
    case ECurrentWeaponType::MELEE:     pMelee      = nullptr; break;
    case ECurrentWeaponType::THROWABLE: pThrowable  = nullptr; break;
    }
}

void AWeaponManager::SetMeshToPlayerUI(TArray<AActor*> _pArrActor, USkeletalMeshComponent* _pSkeletalMeshComp)
{
    ACoreWeapon*          p_firstGunUI  = Cast<ACoreWeapon>(_pArrActor[(int)ECurrentWeaponType::FIRST]);
    ACoreWeapon*          p_secondGunUI = Cast<ACoreWeapon>(_pArrActor[(int)ECurrentWeaponType::SECOND]);
    ACoreWeapon*          p_pistolUI    = Cast<ACoreWeapon>(_pArrActor[(int)ECurrentWeaponType::PISTOL]);
    ACoreMeleeWeapon*     p_meleeUI     = Cast<ACoreMeleeWeapon>(_pArrActor[(int)ECurrentWeaponType::MELEE]);
    ACoreThrowableWeapon* p_throwableUI = Cast<ACoreThrowableWeapon>(_pArrActor[(int)ECurrentWeaponType::THROWABLE]);

    // 첫번째 무기
    {
        if (pFirstGun    &&
            p_firstGunUI &&
            pFirstGun->WeaponType != p_firstGunUI->WeaponType)
            p_firstGunUI->SkeletalMeshComp->SetSkeletalMesh(pFirstGun->SkeletalMeshComp->SkeletalMesh);

        else
            p_firstGunUI->SkeletalMeshComp->SetSkeletalMesh(nullptr);
    }
    // 두번째 무기
    {
        if (pSecondGun    &&
            p_secondGunUI &&
            pSecondGun->WeaponType != p_secondGunUI->WeaponType)
            p_secondGunUI->SkeletalMeshComp->SetSkeletalMesh(pSecondGun->SkeletalMeshComp->SkeletalMesh);

        else
            p_secondGunUI->SkeletalMeshComp->SetSkeletalMesh(nullptr);
    }
    // 세번째 무기
    {
        if (pPistol    &&
            p_pistolUI &&
            pPistol->WeaponType != p_pistolUI->WeaponType)
            p_pistolUI->SkeletalMeshComp->SetSkeletalMesh(pPistol->SkeletalMeshComp->SkeletalMesh);

        else
            p_pistolUI->SkeletalMeshComp->SetSkeletalMesh(nullptr);
    }
    // 네번째 무기
    {
        if (pMelee    &&
            p_meleeUI &&
            pMelee->WeaponType != p_meleeUI->WeaponType)
            p_meleeUI->StaticMeshComp->SetStaticMesh(pMelee->StaticMeshComp->GetStaticMesh());

        else
            p_meleeUI->StaticMeshComp->SetStaticMesh(nullptr);
    }
    // 투척무기 일 시
    {
        if (pThrowable    &&
            p_throwableUI &&
            pThrowable->WeaponType != p_throwableUI->WeaponType)
        {
            auto staticMeshComp = p_throwableUI->StaticMeshComp;

            if (staticMeshComp)
            {
                staticMeshComp->SetStaticMesh(pThrowable->StaticMeshComp->GetStaticMesh());
                staticMeshComp->AttachToComponent(_pSkeletalMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, *(pThrowable->WeaponData.Type + "Socket"));
            }
        }
        else
            p_throwableUI->StaticMeshComp->SetStaticMesh(nullptr);
    }
}

bool AWeaponManager::IsDuplicated(ABaseInteraction* _pWeapon, ECurrentWeaponType _WeaponType)
{
    auto objectGroupType = _pWeapon->ObjectGroupType;
    auto p_gun           = Cast<ACoreWeapon>(_pWeapon);
    auto p_melee         = Cast<ACoreMeleeWeapon>(_pWeapon);
    auto p_grenade       = Cast<ACoreThrowableWeapon>(_pWeapon);

    switch (_WeaponType)
    {
    // 총기 
    case ECurrentWeaponType::FIRST: 
    case ECurrentWeaponType::SECOND: 
         return (p_gun == pFirstGun || p_gun == pSecondGun || objectGroupType == "Handgun");

    case ECurrentWeaponType::PISTOL:    return (p_gun           == pPistol     || objectGroupType != "Handgun");
    case ECurrentWeaponType::MELEE:     return (p_melee         == pMelee      || objectGroupType != "Melee");
    case ECurrentWeaponType::THROWABLE: return (p_grenade       == pThrowable  || objectGroupType != "Throwable");
    case ECurrentWeaponType::NONE:      return (objectGroupType == "Explosive" || objectGroupType == "Melee");
    }
    return true;
}