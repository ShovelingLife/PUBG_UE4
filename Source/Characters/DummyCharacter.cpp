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
    DummySkeletalMeshComp->SetOwnerNoSee(true);
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
    // �޽� �ʱ�ȭ
    DummySkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
    DummySkeletalMeshComp->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/Characters/UE4_Mannequin/Mesh/SK_Mannequin"));
    
    if (SK_MANNEQUIN.Succeeded())
        DummySkeletalMeshComp->SetSkeletalMesh(SK_MANNEQUIN.Object);

    DummySkeletalMeshComp->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
    DummySkeletalMeshComp->bHiddenInGame = true;
}

void ADummyCharacter::InitAnimInstance()
{
    // �ִϸ��̼� �ʱ�ȭ
    static ConstructorHelpers::FClassFinder<UAnimInstance> BP_ANIM(TEXT("/Game/Blueprints/Animations/BP_DummyPlayerAnimInstance"));

    if (BP_ANIM.Succeeded())
        DummySkeletalMeshComp->SetAnimInstanceClass(BP_ANIM.Class);
}

void ADummyCharacter::InitRenderTarget()
{
    SceneCaptureComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComp"));
    SceneCaptureComp->SetupAttachment(RootComponent);
}

void ADummyCharacter::InitWeaponUI()
{
    // 1��° ����
    if (auto tmpFirstGun = GetWorld()->SpawnActor<ACoreWeapon>(ACoreWeapon::StaticClass()))
    {
        tmpFirstGun->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
        tmpFirstGun->SetForDummyCharacter(this->DummySkeletalMeshComp, "FirstGunSock");
        mArrActorToShow.Add(tmpFirstGun);
    }
    // 2��° ����
    if (auto tmpSecondGun = GetWorld()->SpawnActor<ACoreWeapon>(ACoreWeapon::StaticClass()))
    {
        tmpSecondGun->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
        tmpSecondGun->SetForDummyCharacter(this->DummySkeletalMeshComp, "SecondGunSock");
        mArrActorToShow.Add(tmpSecondGun);
    }
    // 3��° ����
    if (auto tmpPistol = GetWorld()->SpawnActor<ACoreWeapon>(ACoreWeapon::StaticClass()))
    {
        tmpPistol->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
        tmpPistol->SetForDummyCharacter(this->DummySkeletalMeshComp, "HandGunSock");
        mArrActorToShow.Add(tmpPistol);
    }
    // 4��° ����
    if (auto tmpMelee = GetWorld()->SpawnActor<ACoreMeleeWeapon>(ACoreMeleeWeapon::StaticClass()))
    {
        tmpMelee->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
        tmpMelee->SetForDummyCharacter(this->DummySkeletalMeshComp, "MeleeSock");
        mArrActorToShow.Add(tmpMelee);
    }
    // 5��° ����
    if (auto tmpThrowable = GetWorld()->SpawnActor<ACoreThrowableWeapon>(ACoreThrowableWeapon::StaticClass()))
    {
        tmpThrowable->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
        tmpThrowable->SetForDummyCharacter(this->DummySkeletalMeshComp, tmpThrowable->WeaponData.Type + "Sock");
        mArrActorToShow.Add(tmpThrowable);
    }
}

void ADummyCharacter::UpdateCharacterWeaponUI()
{
    // ������ ����
    if (mpWeaponManager)
        mpWeaponManager->SetMeshToPlayerUI(mArrActorToShow, DummySkeletalMeshComp);
} 