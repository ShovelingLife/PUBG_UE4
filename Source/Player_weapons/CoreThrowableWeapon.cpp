#include "CoreThrowableWeapon.h"
#include "TimerManager.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "PUBG_UE4/DataTableManager.h"
#include "PUBG_UE4/SoundManager.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include <Runtime/Engine/Public/DrawDebugHelpers.h>

ACoreThrowableWeapon::ACoreThrowableWeapon()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACoreThrowableWeapon::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

    /*if (ProjectileMovementComp)
        ProjectileMovementComp->Deactivate();*/
}

void ACoreThrowableWeapon::BeginPlay()
{
    Super::BeginPlay();
}
    
void ACoreThrowableWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (GrenadeParticleComp)
    {
        static float currentTime = 0.f;

        currentTime += DeltaTime;

        if (currentTime >= 3.5f)
            this->Destroy();
    }
}

void ACoreThrowableWeapon::Init(EThrowableWeaponType WeaponType)
{
    WeaponData = ADataTableManager::ArrOtherWeaponData[(int)WeaponType];
    this->CurrentWeaponType = WeaponType;
    ObjectType = WeaponData.Type;
    ObjectGroupType = WeaponData.GroupType;

    InitMesh();
    InitProjectileMovementComp();
    Super::AttachComponents();

    FString particlePath = "";

    switch (WeaponType)
    {
    case EThrowableWeaponType::FRAGMENTATION1:
    case EThrowableWeaponType::FRAGMENTATION2:
    case EThrowableWeaponType::CLAYMORE:
        particlePath = "ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_A.P_Explosion_Big_A'";
        break;

    case EThrowableWeaponType::ILLUMINATION:
    case EThrowableWeaponType::STICK:
        particlePath = "ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_explosion.P_ky_explosion'";
        break;

    case EThrowableWeaponType::GRAY_SMOKE:
    case EThrowableWeaponType::RED_SMOKE:
        particlePath = "ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Smoke.P_Explosion_Smoke'";
        break;

    case EThrowableWeaponType::MOLOTOV:
        particlePath = "ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Molotov.P_Molotov'";
        break;
    }
    Super::InitParticleSystem(particlePath);
    
    if (SceneComp)
        SceneComp->DestroyComponent();
}

void ACoreThrowableWeapon::InitProjectileMovementComp()
{
    if (CurrentWeaponType == EThrowableWeaponType::CLAYMORE)
        return;

    ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
    ProjectileMovementComp->InitialSpeed  = 0.f;
    ProjectileMovementComp->MaxSpeed      = 0.f;
    ProjectileMovementComp->bShouldBounce = false;
    ProjectileMovementComp->bAutoActivate = false;
    ProjectileMovementComp->Velocity = FVector(0.1f, 0.f, 0.f);
}

void ACoreThrowableWeapon::InitMesh()
{
    ABaseInteraction::InitStaticMesh(WeaponData.MeshPath);

    if (StaticMeshComp)
    {
        RootComponent->DestroyComponent();
        RootComponent = StaticMeshComp;

        // 위치 관련
        StaticMeshComp->SetRelativeLocation(WeaponData.MeshPos);
        StaticMeshComp->SetRelativeRotation(FRotator::MakeFromEuler(FVector(WeaponData.MeshRotationX, 0.f, 0.f)));
        StaticMeshComp->SetRelativeScale3D(FVector(WeaponData.MeshSize));

        // 강체 관련
        StaticMeshComp->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
        StaticMeshComp->SetSimulatePhysics(false);
        StaticMeshComp->SetCollisionProfileName("Explosive");
        StaticMeshComp->SetNotifyRigidBodyCollision(true);
    }
}

bool ACoreThrowableWeapon::IsPlayerInRadius()
{
    // 범위 안에 들어가는지 확인 (원형 충돌감지)
    FVector startPos = GetActorLocation();
    FVector endPos   = startPos + FVector(0.f, 0.f, 0.1f);
    TArray < TEnumAsByte<EObjectTypeQuery>> arrObjectTypeQuery;
    TEnumAsByte<EObjectTypeQuery> ObjectTypeQuery = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
    arrObjectTypeQuery.Add(ObjectTypeQuery);
    TArray<AActor*> arrActorsToIgnore;
    FHitResult radiusHitResult;
    
    if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), startPos, endPos, WeaponData.ExplosionRadius, arrObjectTypeQuery, false, arrActorsToIgnore, EDrawDebugTrace::None, radiusHitResult, true))
    {
        auto character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        FHitResult lineHitResult;
        FVector lineEndPos = character->GetActorLocation();
        
        if (GetWorld()->LineTraceSingleByChannel(lineHitResult, startPos, lineEndPos, ECollisionChannel::ECC_Camera))
            return (lineHitResult.Actor == character);
    }
    return false;
}

void ACoreThrowableWeapon::Throw(FVector Velocity)
{
    if (!StaticMeshComp ||
        !ProjectileMovementComp)
        return;

    ProjectileMovementComp->Velocity = Velocity;
    ProjectileMovementComp->Activate();
    this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    this->SetActorScale3D(FVector(WeaponData.MeshSize));

    FTimerHandle waitHandle;
    auto p_world = GetWorld();

    if (!p_world)
        return;

    p_world->GetTimerManager().SetTimer(waitHandle, FTimerDelegate::CreateLambda([&]()
        {
            auto location = GetActorLocation();
            static bool b_once = false;

            // 사운드 재생
            if (auto p_world = GetWorld())
            {
                if (auto p_subGameInst = Cast<UCustomGameInstance>(p_world->GetGameInstance()))
                {
                    if (auto p_soundManager = p_subGameInst->pSoundManager)
                        UGameplayStatics::PlaySoundAtLocation(GetWorld(), p_soundManager->GetExplosiveSoundBase((int)CurrentWeaponType), location);
                }
            }
            // 효과 재생
            if (IsPlayerInRadius())
                mCallBack.ExecuteIfBound();

            // 이펙트 재생 
            if (CurrentWeaponType == EThrowableWeaponType::GRAY_SMOKE ||
                CurrentWeaponType == EThrowableWeaponType::RED_SMOKE  ||
                CurrentWeaponType == EThrowableWeaponType::MOLOTOV)
            {
                StaticMeshComp->SetVisibility(false);
                GrenadeParticleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, location, FRotator::ZeroRotator, false);
            }
            else
            {
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, location);
                this->Destroy();
            }
        }), 3.f, false);
}