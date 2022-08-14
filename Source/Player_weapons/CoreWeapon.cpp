#include "CoreWeapon.h"
#include "InputCoreTypes.h"
#include "TimerManager.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "PUBG_UE4/DataTableManager.h"
#include "PUBG_UE4/SoundManager.h"

ACoreWeapon::ACoreWeapon()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACoreWeapon::BeginPlay()
{
    Super::BeginPlay();
    Super::SetCollisionSettingsForObjects();
}

void ACoreWeapon::NotifyActorBeginOverlap(AActor* CollidedActor)
{
    Super::NotifyActorBeginOverlap(CollidedActor);
}

void ACoreWeapon::NotifyActorEndOverlap(AActor* ColliderActor)
{
    Super::NotifyActorEndOverlap(ColliderActor);
}

void ACoreWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    CheckReloading(DeltaTime);

    // 현재 인벤토리에 있을 때만 총알 개수 갱신
    if (bInInventory)
    {
        if (auto p_customGameInst = UCustomGameInstance::GetInst())
        {
            auto deleGetBulletCount = p_customGameInst->DeleGetBulletCount;

            if (deleGetBulletCount.IsBound())
                WeaponData.BulletCount = deleGetBulletCount.Execute(WeaponData.BulletType);
        }        
    }
    // 좌클릭 시 쿨타임 적용
    if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetInputKeyTimeDown(EKeys::LeftMouseButton) > 0.f)
    {
        // 쿨타임 적용
        if(ShootType == CONSECUTIVE)
        {
            mCurrentShootTime += DeltaTime;

            if (mCurrentShootTime > 0.25f)
            {
                FireBullet();
                mCurrentShootTime = 0.f;
            }
        }
    }
    else
        mCurrentShootTime = 0.f;
}

void ACoreWeapon::ClickEvent()
{
    // 총알 부족한지 확인
    if (WeaponData.BulletCount == 0)
    {
        // 총알이 하나라도 존재할 시
        if (WeaponData.MaxBulletCount > 0)
            Reload();

        else
            PlaySound(EWeaponSoundType::EMPTY_AMMO);
    }
    // 총알이 1개 이상이므로 발사
    else
    {
        bShooting = true;

        if (ShootType == BURST)
        {
            // 총알 발사가 진행 중일 시 반환        
            if (!GetWorld()->GetTimerManager().IsTimerActive(mTimerHandle))
                GetWorld()->GetTimerManager().SetTimer(mTimerHandle, this, &ACoreWeapon::FireBullet, 0.25f, true);
        }
        else
            FireBullet();
    }
}

void ACoreWeapon::Init(EGunType Type)
{
    WeaponData = ADataTableManager::GetWeaponData((int)Type);
    WeaponData.BulletCount = WeaponData.BulletLimitCount;
    WeaponType = Type;
    ObjectType = WeaponData.Type;
    ObjectGroupType = WeaponData.GroupType;

    Super::SetCollisionSettingsForObjects();
    Super::InitSkeletalMesh(WeaponData.MeshPath);
    Super::InitParticleSystem("/Game/VFX/FXVarietyPack/Particles/P_ky_shotShockwave.P_ky_shotShockwave");
    InitBullet();
    UpdateParticleSystem();
}

void ACoreWeapon::InitBullet()
{
    ConstructorHelpers::FClassFinder<ACoreBullet> BP_BULLET(*(WeaponData.BulletBP_path));

    if (BP_BULLET.Succeeded())
        BP_Bullet = BP_BULLET.Class;
}

void ACoreWeapon::UpdateParticleSystem()
{
    if (!ParticleComp)
        return;

    ParticleComp->SetWorldScale3D(FVector(0.05f));
    ParticleComp->SetWorldLocation(SkeletalMeshComp->GetSocketLocation(TEXT("MuzzleSock")));
    ParticleComp->AddWorldRotation(FQuat::MakeFromEuler(FVector(0.f, 90.f, 0.f)));
}

void ACoreWeapon::PlaySound(EWeaponSoundType SoundType)
{
    if (auto p_customGameInst = UCustomGameInstance::GetInst())
    {
        if (auto p_soundManager = p_customGameInst->pSoundManager)
            p_soundManager->PlayGunSound(GetActorLocation(), SoundType, (int)WeaponType);
    }
}

void ACoreWeapon::CheckReloading(float DeltaTime)
{
    // 재장전 중
    if (mbReloading)
    {
        mCurrentReloadTime += DeltaTime;

        if (mCurrentReloadTime > mkReloadTime)
        {
            mCurrentReloadTime = 0.f;
            mbReloading = false;
        }
    }
}

void ACoreWeapon::FireBullet()
{
    if (!bShooting)
        return;

    if(ShootType == BURST)
    {
        if (mBurstCount > 2)
        {
            ResetBurstCount();
            return;
        }
        mBurstCount++;
    }
    if (WeaponData.BulletCount == 0)
    {
        PlaySound(EWeaponSoundType::EMPTY_AMMO);
        ResetBurstCount();
        return;
    }
    // 사운드 적용 및 총알 1개 차감
    if (auto p_customGameInst = UCustomGameInstance::GetInst())
        p_customGameInst->DeleDeleteInventoryItem.ExecuteIfBound(WeaponData.BulletType);

    // ------- 레이캐스트 범위 구하기 -------

    FVector startPos = SkeletalMeshComp->GetSocketLocation("Barrel");
    auto cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

    // 게임 뷰포트 가운데 지점 구하기    
    FVector2D vPortCenterPos = GEngine->GameViewport->Viewport->GetSizeXY() / 2.f;
    FVector deprojPos, deprojDir;
    UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(GetWorld(), 0), vPortCenterPos, deprojPos, deprojDir);
    FVector rayStartPos = cameraManager->GetCameraLocation();
    FVector rayEndPos = deprojPos + (deprojDir * 5000.f);

    PlaySound(EWeaponSoundType::SHOT);
}

void ACoreWeapon::ResetBurstCount()
{
    // 점사 타이머 초기화
    GetWorld()->GetTimerManager().ClearTimer(mTimerHandle);
    mTimerHandle.Invalidate();
    mBurstCount = 0;
}

void ACoreWeapon::Reload()
{
    PlaySound(EWeaponSoundType::RELOAD);
    int result = (WeaponData.BulletCount > 0) ? (WeaponData.MaxBulletCount - WeaponData.BulletCount) : WeaponData.MaxBulletCount;
    WeaponData.MaxBulletCount -= result;
    WeaponData.BulletCount += result;
    mbReloading = true;
}

void ACoreWeapon::ChangeShootMode()
{
    if (GetWorld()->GetTimerManager().IsTimerActive(mTimerHandle))
        ResetBurstCount();

    bShooting = false;

    // 격발 방식 변경
    ShootType = GetNextShootType();

    // 팝업 UI 설정
    if (auto p_customGameInst = UCustomGameInstance::GetInst())
        p_customGameInst->DeleSetShootTypeNotificationTxt.ExecuteIfBound(GetShootTypeStr());
}

EGunShootType ACoreWeapon::GetNextShootType() const
{
    if (ShootType != GetMaxShootType())
    {
        switch (ShootType)
        {
        case SINGLE: return BURST;
        case BURST:  return CONSECUTIVE;
        }
    }
    return SINGLE;
}

EGunShootType ACoreWeapon::GetMaxShootType() const
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
    return mapGunShootType[WeaponData.GroupType];
}

FString ACoreWeapon::GetShootTypeStr() const
{
    if (ShootType == MAX)
        return "Fail";

    TMap<EGunShootType, FString> mapGunShootType
    {
        { SINGLE,      "Single" },
        { BURST,       "Burst" },
        { CONSECUTIVE, "Consecutive" }
    };
    return "Current type : " + mapGunShootType[ShootType];
}