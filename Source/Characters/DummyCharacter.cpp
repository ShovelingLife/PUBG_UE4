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
    InitMeshComp();
    InitAnimInstance();
    InitRenderTarget();
}

void ADummyCharacter::BeginPlay()
{
    Super::BeginPlay();
    mArrActorToShow.Add(this);
    InitWeaponUI();
    DummySkeletalMeshComp->SetOwnerNoSee(true);
    SceneCaptureComp->ShowOnlyActors = mArrActorToShow;
}

void ADummyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    SetActorRelativeLocation(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());    
    UpdateWeapon();
}

void ADummyCharacter::InitMeshComp()
{
    // 메쉬 초기화
    DummySkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
    this->SetRootComponent(DummySkeletalMeshComp);

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/Characters/UE4_Mannequin/Mesh/SK_Mannequin"));
    
    if (SK_MANNEQUIN.Succeeded())
        DummySkeletalMeshComp->SetSkeletalMesh(SK_MANNEQUIN.Object);

    DummySkeletalMeshComp->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
    DummySkeletalMeshComp->bHiddenInGame = true;
}

void ADummyCharacter::InitAnimInstance()
{
    // 애니메이션 초기화
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
    auto p_world = GetWorld();

    if (!p_world)
        return;

    auto tmpThrowable = p_world->SpawnActor<ACoreThrowableWeapon>(ACoreThrowableWeapon::StaticClass());

    TArray<TPair<ABaseInteraction*, FString>> arrWeapons
    {
        TPair<ABaseInteraction*, FString>(p_world->SpawnActor<ACoreWeapon>(ACoreWeapon::StaticClass()), "FirstGunSock"),
        TPair<ABaseInteraction*, FString>(p_world->SpawnActor<ACoreWeapon>(ACoreWeapon::StaticClass()), "SecondGunSock"),
        TPair<ABaseInteraction*, FString>(p_world->SpawnActor<ACoreWeapon>(ACoreWeapon::StaticClass()), "HandGunSock"),
        TPair<ABaseInteraction*, FString>(p_world->SpawnActor<ACoreMeleeWeapon>(ACoreMeleeWeapon::StaticClass()), "MeleeSock"),
        TPair<ABaseInteraction*, FString>(tmpThrowable, tmpThrowable->WeaponData.Type + "Sock")
    };
    for (auto item : arrWeapons)
    {
        if (auto p_weapon = item.Key)
        {
            p_weapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale, *item.Value);
            Cast<ABaseInteraction>(p_weapon)->SetForDummyCharacter();
            mArrActorToShow.Add(p_weapon);
        }
    }
}

void ADummyCharacter::UpdateWeapon()
{
    auto p_player = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (p_player)
    {
        if (auto p_weaponManager = p_player->GetWeaponManager())
        {
            TArray<ABaseInteraction*> arr_tmpWeapon
            {
                p_weaponManager->pFirstGun,
                p_weaponManager->pSecondGun,
                p_weaponManager->pPistol,
                p_weaponManager->pMelee,
                p_weaponManager->pThrowable
            };
#define WEAPON_COUNT 5

            // 렌더 타깃으로부터 렌더링이 될 액터 개수는 6개, 더미 캐릭터 제외 1번부터
            for (int i = 0; i < WEAPON_COUNT; i++)
            {
                if (auto p_weapon = arr_tmpWeapon[i])
                    Cast<ABaseInteraction>(mArrActorToShow[i + 1])->UpdateMesh(p_weapon->GetRootComponent());                
            }
        }
    }
}