#include "DummyCharacter.h"
#include "CustomPlayer.h"
#include "Player_weapons/WeaponManager.h"
#include "Player_weapons/CoreWeapon.h"
#include "Player_weapons/CoreMeleeWeapon.h"
#include "Player_weapons/CoreThrowableWeapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ADummyCharacter::ADummyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
    InitMeshComp();
    InitAnimInstance();
    InitRenderTarget();
}

void ADummyCharacter::BeginPlay()
{
    Super::BeginPlay();
    mArrActorToShow.Add(this);
    InitWeaponUI();
    mpWeaponManager = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetWeaponManager();
    SkeletalMeshComp->SetOwnerNoSee(true);
}

void ADummyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    SceneCaptureComp->ShowOnlyActors = mArrActorToShow;
    SetActorRelativeLocation(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());    
    UpdateCharacterWeaponUI();
}

void ADummyCharacter::InitMeshComp()
{
    // 메쉬 초기화
    SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
    SkeletalMeshComp->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/Characters/UE4_Mannequin/Mesh/SK_Mannequin"));
    
    if (SK_MANNEQUIN.Succeeded())
        SkeletalMeshComp->SetSkeletalMesh(SK_MANNEQUIN.Object);

    SkeletalMeshComp->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
    SkeletalMeshComp->bHiddenInGame = true;
}

void ADummyCharacter::InitAnimInstance()
{
    // 애니메이션 초기화
    static ConstructorHelpers::FClassFinder<UAnimInstance> BP_ANIM(TEXT("/Game/Blueprints/Animations/BP_DummyPlayerAnimInstance"));

    if (BP_ANIM.Succeeded())
        SkeletalMeshComp->SetAnimInstanceClass(BP_ANIM.Class);
}

void ADummyCharacter::InitRenderTarget()
{
    SceneCaptureComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComp"));
    SceneCaptureComp->SetupAttachment(RootComponent);
}

void ADummyCharacter::InitWeaponUI()
{
    // 1번째 무기
    {
        // 월드에 소환
        auto tmpFirstGun = GetWorld()->SpawnActor<ACoreWeapon>(ACoreWeapon::StaticClass());
        auto tmpFirstGunMeshComp = tmpFirstGun->SkeletalMeshComp;
        tmpFirstGun->DestroyComponentsForUI("Skeletal");

        // 메시 초기화
        tmpFirstGunMeshComp->SetupAttachment(tmpFirstGun->GetRootComponent());
        tmpFirstGunMeshComp->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, "FirstGunSock");

        // 엑터 설정 후 추가
        tmpFirstGun->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
        mArrActorToShow.Add(tmpFirstGun);
    }
    // 2번째 무기
    {
        auto tmpSecondGun = GetWorld()->SpawnActor<ACoreWeapon>(ACoreWeapon::StaticClass());
        auto tmpSecondGunMeshComp = tmpSecondGun->SkeletalMeshComp;
        tmpSecondGun->DestroyComponentsForUI("Skeletal");
        tmpSecondGunMeshComp->SetupAttachment(tmpSecondGun->GetRootComponent());
        tmpSecondGunMeshComp->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, "SecondGunSock");
        tmpSecondGun->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
        mArrActorToShow.Add(tmpSecondGun);
    }
    // 3번째 무기
    {
        auto tmpPistol = GetWorld()->SpawnActor<ACoreWeapon>(ACoreWeapon::StaticClass());
        tmpPistol->DestroyComponentsForUI("Skeletal");
        tmpPistol->SkeletalMeshComp->SetupAttachment(tmpPistol->GetRootComponent());

        tmpPistol->SkeletalMeshComp->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, "HandGunSock");
        tmpPistol->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
        mArrActorToShow.Add(tmpPistol);
    }
    // 4번째 무기
    {
        auto tmpMelee = GetWorld()->SpawnActor<ACoreMeleeWeapon>(ACoreMeleeWeapon::StaticClass());
        auto tmpMeleeMeshComp = tmpMelee->StaticMeshComp;
        tmpMelee->DestroyComponentsForUI("Static");
        tmpMeleeMeshComp->SetupAttachment(tmpMelee->GetRootComponent());
        tmpMeleeMeshComp->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, "MeleeSock");
        tmpMelee->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
        mArrActorToShow.Add(tmpMelee);
    }
    // 5번째 무기
    {
        auto tmpThrowable = GetWorld()->SpawnActor<ACoreThrowableWeapon>(ACoreThrowableWeapon::StaticClass());
        auto tmpThrowableMeshComp = tmpThrowable->StaticMeshComp;
        tmpThrowable->DestroyComponentsForUI("Static");
        tmpThrowableMeshComp->SetupAttachment(tmpThrowable->GetRootComponent());
        tmpThrowableMeshComp->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, "ThrowableSock");
        tmpThrowable->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
        mArrActorToShow.Add(tmpThrowable);
    }
}

void ADummyCharacter::UpdateCharacterWeaponUI()
{
    enum class EWeaponIndexType
    {
        FIRST_GUN = 1,
        SECOND_GUN,
        PISTOL,
        MELEE,
        THROWABLE
    };
    // 첫번째 총
    {
        ACoreWeapon* p_tmpFirstGun = Cast<ACoreWeapon>(mArrActorToShow[(int)EWeaponIndexType::FIRST_GUN]);

        if (auto p_FirstGun = mpWeaponManager->pFirstGun)
        {
            if (p_tmpFirstGun &&
                p_tmpFirstGun->WeaponType != p_FirstGun->WeaponType)
            {
                auto weaponSkeletalMeshComp = p_tmpFirstGun->SkeletalMeshComp;
                weaponSkeletalMeshComp->SetSkeletalMesh(p_FirstGun->SkeletalMeshComp->SkeletalMesh);
                weaponSkeletalMeshComp->SetVisibility(true);
            }
        }
        else
        {
            if (p_tmpFirstGun)
                p_tmpFirstGun->SkeletalMeshComp->SetVisibility(false);
        }
    }
    // 두번째 총
    {
        ACoreWeapon* p_tmpSecondGun = Cast<ACoreWeapon>(mArrActorToShow[(int)EWeaponIndexType::SECOND_GUN]);

        if (auto p_SecondGun = mpWeaponManager->pSecondGun)
        {
            if (p_tmpSecondGun &&
                p_tmpSecondGun->WeaponType != p_SecondGun->WeaponType)
            {
                auto weaponSkeletalMeshComp = p_tmpSecondGun->SkeletalMeshComp;
                weaponSkeletalMeshComp->SetSkeletalMesh(p_SecondGun->SkeletalMeshComp->SkeletalMesh);
                weaponSkeletalMeshComp->SetVisibility(true);
            }
        }
        else
        {
            if (p_tmpSecondGun)
                p_tmpSecondGun->SkeletalMeshComp->SetVisibility(false);
        }
    }
    // 세번째 총
    {
        ACoreWeapon* p_tmpPistol = Cast<ACoreWeapon>(mArrActorToShow[(int)EWeaponIndexType::PISTOL]);

        if (auto p_Pistol = mpWeaponManager->pPistol)
        {
            if (p_tmpPistol &&
                p_tmpPistol->WeaponType != p_Pistol->WeaponType)
            {
                auto weaponSkeletalMeshComp = p_tmpPistol->SkeletalMeshComp;
                weaponSkeletalMeshComp->SetSkeletalMesh(p_Pistol->SkeletalMeshComp->SkeletalMesh);
                weaponSkeletalMeshComp->SetVisibility(true);
            }
        }
        else
        {
            if (p_tmpPistol)
                p_tmpPistol->SkeletalMeshComp->SetVisibility(false);
        }
    }
    // 근접 무기
    {
        ACoreMeleeWeapon* p_tmpMelee = Cast<ACoreMeleeWeapon>(mArrActorToShow[(int)EWeaponIndexType::MELEE]);

        if (auto p_Melee = mpWeaponManager->pMelee)
        {
            if (p_tmpMelee &&
                p_tmpMelee->WeaponType != p_Melee->WeaponType)
            {
                auto weaponStaticMeshComp = p_tmpMelee->StaticMeshComp;
                weaponStaticMeshComp->SetStaticMesh(p_Melee->StaticMeshComp->GetStaticMesh());
                weaponStaticMeshComp->SetVisibility(true);
            }
        }
        else
        {
            if (p_tmpMelee)
                p_tmpMelee->SkeletalMeshComp->SetVisibility(false);
        }
    }
    // 투척류 무기
    {
        ACoreThrowableWeapon* p_tmpThrowable = Cast<ACoreThrowableWeapon>(mArrActorToShow[(int)EWeaponIndexType::THROWABLE]);

        if (auto p_Throwable = mpWeaponManager->pThrowable)
        {
            if (p_tmpThrowable &&
                p_tmpThrowable->WeaponType != p_Throwable->WeaponType)
            {
                auto weaponStaticMeshComp = p_tmpThrowable->StaticMeshComp;
                weaponStaticMeshComp->SetStaticMesh(p_Throwable->StaticMeshComp->GetStaticMesh());
                weaponStaticMeshComp->SetVisibility(true);
            }
        }
        else
        {
            if (p_tmpThrowable)
                p_tmpThrowable->SkeletalMeshComp->SetVisibility(false);
        }
    }
}