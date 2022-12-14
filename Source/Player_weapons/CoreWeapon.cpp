#include "CoreWeapon.h"
#include "Farmable_items/CoreAttachment.h"
#include "Farmable_items/CoreBarrel.h"
#include "Farmable_items/CoreForend.h"
#include "Farmable_items/CoreGrip.h"
#include "Farmable_items/CoreSight.h"
#include "Farmable_items/CoreStock.h"
#include "GunRecoilComponent.h"
#include "InputCoreTypes.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Curves/CurveVector.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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

            if (mCurrentShootTime > mNextShootTime)
            {
                FireBullet();
                mCurrentShootTime = 0.f;
            }
        }
    }
    else
        mCurrentShootTime = 0.f;

    if (!bShooting &&
        GunRecoilComponent)
        GunRecoilComponent->bFiring = true;
}

void ACoreWeapon::ResetSettings()
{
    WeaponData.MaxBulletCount = 0;
    bInInventory = false;
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
            if (!GetWorld()->GetTimerManager().IsTimerActive(mBurstTimerHandle))
                GetWorld()->GetTimerManager().SetTimer(mBurstTimerHandle, this, &ACoreWeapon::FireBullet, mNextShootTime, true);
        }
        else
            FireBullet();
    }
}

void ACoreWeapon::Init(EGunType Type)
{
    ItemIdx = (int)Type;
    WeaponData = ADataTableManager::GetWeaponData(ItemIdx);
    WeaponData.BulletCount = WeaponData.BulletLimitCount;
    WeaponType = Type;
    ObjectType = WeaponData.Type;
    ObjectGroupType = WeaponData.GroupType;
    mNextShootTime = (ObjectGroupType == "ShotGun") ? 0.25f : 0.1f;

    // 총기 반동 초기화
    GunRecoilComponent    = CreateDefaultSubobject<UGunRecoilComponent>(TEXT("GunRecoilComp"));
    FString curvePath     = "/Game/Data/GunRecoilCurve/RecoilCurve" + ObjectType;
    auto recoilCurveFloat = ConstructorHelpers::FObjectFinder<UCurveVector>(*curvePath);

    if (recoilCurveFloat.Succeeded())
        GunRecoilComponent->RecoilCurve = recoilCurveFloat.Object;

    //Super::ChangeCollisionSettings();
    Super::InitSkeletalMesh(WeaponData.MeshPath);
    Super::InitParticleSystem("/Game/VFX/FXVarietyPack/Particles/P_ky_shotShockwave.P_ky_shotShockwave");
    InitBullet();
    UpdateParticleSystem();

    if (ColliderComp)
        ColliderComp->DestroyComponent();

    // 메쉬 컴포넌트 재설정
    SkeletalMeshComp->SetCollisionProfileName("Object");
    SkeletalMeshComp->SetSimulatePhysics(true);
    SkeletalMeshComp->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Yes;
}

void ACoreWeapon::InitBullet()
{
    ConstructorHelpers::FClassFinder<ACoreBullet> BP_BULLET(*WeaponData.BulletBP_path);

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

    if (ShootType == BURST)
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
    GunRecoilComponent->RecoilStart();
    PlaySound(EWeaponSoundType::SHOT);

    if (auto p_customGameInst = UCustomGameInstance::GetInst())
        p_customGameInst->DeleDeleteInventoryItem.ExecuteIfBound(WeaponData.BulletType);

    // ------- 레이캐스트 범위 구하기 -------
    auto cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
    auto cameraPos     = cameraManager->GetCameraLocation();
    auto cameraDir     = UKismetMathLibrary::GetForwardVector(cameraManager->GetCameraRotation()) * 50000.f;

    // 레이트레이스 충돌체 감지
    FHitResult hitResult;
    FCollisionQueryParams collisionQueryParams;
    collisionQueryParams.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, cameraPos, cameraPos + cameraDir, ECollisionChannel::ECC_Visibility, collisionQueryParams);
    //DrawDebugLine(GetWorld(), cameraPos, cameraPos + cameraDir, FColor::Red, false, 1.f);
    auto selectedVec = UKismetMathLibrary::SelectVector(hitResult.ImpactPoint, hitResult.TraceEnd, bHit);
    auto rot = UKismetMathLibrary::FindLookAtRotation(cameraPos, selectedVec);
    auto p_bullet = GetWorld()->SpawnActor<ACoreBullet>(BP_Bullet, UKismetMathLibrary::MakeTransform(cameraPos,rot,FVector(1.f)));
}

void ACoreWeapon::ResetBurstCount()
{
    // 점사 타이머 초기화
    GetWorld()->GetTimerManager().ClearTimer(mBurstTimerHandle);
    mBurstTimerHandle.Invalidate();
    mBurstCount = bShooting = 0;
}

EGunShootType ACoreWeapon::GetNextShootType() const
{
    EGunShootType shootType = SINGLE;

    if (ShootType != GetMaxShootType())
    {
        switch (ShootType)
        {
        case SINGLE: shootType = BURST;       break;
        case BURST:  shootType = CONSECUTIVE; break;
        }
    }
    return shootType;
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

void ACoreWeapon::Reload()
{
    PlaySound(EWeaponSoundType::RELOAD);
    // 총알 있을 시 재장전 
    int curBulletCount = WeaponData.BulletCount, maxBulletCount = WeaponData.MaxBulletCount;
    int result = (curBulletCount > 0) ? (maxBulletCount - curBulletCount) : maxBulletCount;
    WeaponData.MaxBulletCount -= result;
    WeaponData.BulletCount += result;
    mbReloading = true;
}

void ACoreWeapon::ChangeShootMode()
{
    if (GetWorld()->GetTimerManager().IsTimerActive(mBurstTimerHandle))
        ResetBurstCount();

    bShooting = false;

    // 격발 방식 변경
    ShootType = GetNextShootType();

    // 팝업 UI 설정
    if (auto p_customGameInst = UCustomGameInstance::GetInst())
        p_customGameInst->DeleSetFadingTxt.ExecuteIfBound(GetShootTypeStr());
}

FString ACoreWeapon::GetShootTypeStr() const
{
    // 조정간을 어디까지 적용 가능한지 확인
    TMap<EGunShootType, FString> mapGunShootType
    {
        { SINGLE,      "Single" },
        { BURST,       "Burst" },
        { CONSECUTIVE, "Consecutive" }
    };
    return (ShootType == MAX) ? "Fail" : "Current type : " + mapGunShootType[ShootType];
}

UTexture* ACoreWeapon::GetAttachmentTex(int Idx) const
{
    auto attachment = TArray<ACoreAttachment*>{ CurrentBarrel, CurrentForend, CurrentGrip, CurrentSight, CurrentStock } [Idx];
    return attachment ? attachment->CurrentItemTex : nullptr;
}
