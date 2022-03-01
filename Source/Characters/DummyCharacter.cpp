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
        auto tmpFirstGun = GetWorld()->SpawnActor<ACoreWeapon>(ACoreWeapon::StaticClass());
        tmpFirstGun->DestroyComponentsForUI("Skeletal");
        tmpFirstGun->SkeletalMeshComp->SetVisibility(true);
        tmpFirstGun->SkeletalMeshComp->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, "FirstGunSock");
        tmpFirstGun->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
        mArrActorToShow.Add(tmpFirstGun);
    }
    // 2번째 무기
    {
        auto tmpSecondGun = GetWorld()->SpawnActor<ACoreWeapon>(ACoreWeapon::StaticClass());
        tmpSecondGun->DestroyComponentsForUI("Skeletal");
        tmpSecondGun->SkeletalMeshComp->SetVisibility(true);
        tmpSecondGun->SkeletalMeshComp->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, "SecondGunSock");
        tmpSecondGun->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
        mArrActorToShow.Add(tmpSecondGun);
    }
    // 3번째 무기
    {
        auto tmpPistol = GetWorld()->SpawnActor<ACoreWeapon>(ACoreWeapon::StaticClass());
        tmpPistol->DestroyComponentsForUI("Skeletal");
        tmpPistol->SkeletalMeshComp->SetVisibility(true);
        tmpPistol->SkeletalMeshComp->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, "HandGunSock");
        tmpPistol->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
        mArrActorToShow.Add(tmpPistol);
    }
    // 4번째 무기
    {
        auto tmpMelee = GetWorld()->SpawnActor<ACoreMeleeWeapon>(ACoreMeleeWeapon::StaticClass());
        tmpMelee->DestroyComponentsForUI("Static");
        tmpMelee->StaticMeshComp->SetVisibility(true);
        tmpMelee->StaticMeshComp->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, "MeleeSock");
        tmpMelee->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
        mArrActorToShow.Add(tmpMelee);
    }
    // 5번째 무기
    {
        auto tmpThrowable = GetWorld()->SpawnActor<ACoreThrowableWeapon>(ACoreThrowableWeapon::StaticClass());
        tmpThrowable->DestroyComponentsForUI("Static");
        tmpThrowable->StaticMeshComp->SetVisibility(true);
        tmpThrowable->StaticMeshComp->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, "ThrowableSock");
        tmpThrowable->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
        mArrActorToShow.Add(tmpThrowable);
    }
}

void ADummyCharacter::UpdateCharacterWeaponUI()
{
    // 데이터 갱신
    if (mpWeaponManager)
        mpWeaponManager->SetMeshToPlayerUI(mArrActorToShow);
} 