#include "DummyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SkeletalMeshComponent.h"
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
    TArray<AActor*> arr_actor;
    arr_actor.Add(this);
    SceneCaptureComp->ShowOnlyActors = arr_actor;
    SkeletalMeshComp->SetOwnerNoSee(true);
}

void ADummyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    SetActorLocation(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());
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