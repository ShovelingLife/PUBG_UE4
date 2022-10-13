#include "DummyCharacter.h"
#include "CustomPlayer.h"
#include "Player_weapons/WeaponManager.h"
#include "Player_weapons/CoreWeapon.h"
#include "Player_weapons/CoreMeleeWeapon.h"
#include "Player_weapons/CoreThrowableWeapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ADummyCharacter::ADummyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    this->SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComp")));
    InitMeshComp();
    InitAnimInstance();
    InitRenderTarget();
}

void ADummyCharacter::BeginPlay()
{
    Super::BeginPlay();
    this->SetOwner(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    mArrActorToShow.Add(this);
    DummySkeletalMeshComp->SetOwnerNoSee(true);
    SceneCaptureComp->ShowOnlyActors = mArrActorToShow;
    this->SetOwner(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    InitWeaponUI();
}

void ADummyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    SetActorRelativeLocation(GetOwner()->GetActorLocation());    
    UpdateWeapon();
}

void ADummyCharacter::InitMeshComp()
{
    // 메쉬 초기화
    DummySkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
    DummySkeletalMeshComp->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/2_Meshes/UE4_Mannequin/Mesh/SK_Mannequin"));
    
    if (SK_MANNEQUIN.Succeeded())
        DummySkeletalMeshComp->SetSkeletalMesh(SK_MANNEQUIN.Object);

    DummySkeletalMeshComp->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
    DummySkeletalMeshComp->bHiddenInGame = true;
}

void ADummyCharacter::InitAnimInstance()
{
    // 애니메이션 초기화
    static ConstructorHelpers::FClassFinder<UAnimInstance> BP_ANIM(TEXT("/Game/1_Blueprints/Animations/BP_DummyPlayerAnimInstance"));

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
    auto p_world = GetWorld();

    TArray<TPair<ABaseInteraction*, FString>> arrWeapons
    {
        TPair<ABaseInteraction*, FString>(p_world->SpawnActor<ACoreWeapon>(ACoreWeapon::StaticClass()), "FirstGunSock"),
        TPair<ABaseInteraction*, FString>(p_world->SpawnActor<ACoreWeapon>(ACoreWeapon::StaticClass()), "SecondGunSock"),
        TPair<ABaseInteraction*, FString>(p_world->SpawnActor<ACoreWeapon>(ACoreWeapon::StaticClass()), "HandGunSock"),
        TPair<ABaseInteraction*, FString>(p_world->SpawnActor<ACoreMeleeWeapon>(ACoreMeleeWeapon::StaticClass()), "MeleeSock"),
        TPair<ABaseInteraction*, FString>(p_world->SpawnActor<ACoreThrowableWeapon>(ACoreThrowableWeapon::StaticClass()), "")
    };
    for (auto& [p_weapon, socketName] : arrWeapons)
    {
        p_weapon->SetOwner(this);
        p_weapon->SetForDummyCharacter();
        p_weapon->AttachToComponent(DummySkeletalMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, *socketName);
        mArrActorToShow.Add(p_weapon);
    }
}

void ADummyCharacter::UpdateWeapon()
{
    return;
    // ACustomPlayer 다운 캐스팅
    if (auto p_player = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
    {
        if (auto p_weaponManager = p_player->GetWeaponManager())
            p_weaponManager->SetMeshToPlayerUI(mArrActorToShow);
    }
}