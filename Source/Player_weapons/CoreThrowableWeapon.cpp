#include "CoreThrowableWeapon.h"
#include "TimerManager.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "PUBG_UE4/DataTableManager.h"
#include "PUBG_UE4/SoundManager.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
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
    this->InitProjectileMovementComp();
}

ACoreThrowableWeapon::ACoreThrowableWeapon(EThrowableWeaponType Type) : ACoreThrowableWeapon()
{
    WeaponType      = Type;
    WeaponData      = ADataTableManager::ArrOtherWeaponData[(int)Type];
    ObjectType      = WeaponData.Type;
    ObjectGroupType = WeaponData.GroupType;

    this->InitMesh();
    this->InitParticleSystem();
}

void ACoreThrowableWeapon::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

    /*if (ProjectileMovementComp)
        ProjectileMovementComp->Deactivate();*/
}

UFUNCTION() void ACoreThrowableWeapon::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
    {
        if (mpCustomGameInstance)
            mpCustomGameInstance->DeleSetPlayerOtherState.ExecuteIfBound(EPlayerOtherState::BURNED);
    }
}

UFUNCTION() void ACoreThrowableWeapon::EndOverlap(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
    {
        if (mpCustomGameInstance)
        {
            mpCustomGameInstance->DeleSetPlayerOtherState.ExecuteIfBound(EPlayerOtherState::NONE);
            mpCustomGameInstance->DeleKillUI_Anim.ExecuteIfBound();
        }
    }
}

void ACoreThrowableWeapon::BeginDestroy()
{
    Super::BeginDestroy();
    GrenadeEndPos = FVector::ZeroVector;

    if (mpCustomGameInstance)
        mpCustomGameInstance->DeleKillUI_Anim.ExecuteIfBound();
}

void ACoreThrowableWeapon::BeginPlay()
{
    Super::BeginPlay();

    if (auto p_world = GetWorld())
        mpCustomGameInstance = Cast<UCustomGameInstance>(p_world->GetGameInstance());
}
    
void ACoreThrowableWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACoreThrowableWeapon::BindExplosionFunc()
{
    switch (WeaponType)
    {
    case EThrowableWeaponType::ILLUMINATION:

        mCallBack.BindLambda([this]()
            {
                float distance  = this->GetDistanceTo(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
                float startTime = (distance >= 300.f) ? 2.5f : 0.f;
                float waitTime  = (startTime == 0.f)   ? 5.f : 2.5f;

                if (mpCustomGameInstance)
                    mpCustomGameInstance->DeleRunEffectAnim.ExecuteIfBound(startTime, waitTime, EPlayerStateAnimType::BLINDED);
            });
        break;

    case EThrowableWeaponType::STICK:
    case EThrowableWeaponType::FRAGMENTATION1:
    case EThrowableWeaponType::FRAGMENTATION2:
    case EThrowableWeaponType::CLAYMORE:

        mCallBack.BindLambda([this]()
            {
                if (mpCustomGameInstance)
                    mpCustomGameInstance->DeleDealPlayerDmg.ExecuteIfBound(WeaponData.Damage);                
            });
        break;
    }
}

void ACoreThrowableWeapon::InitParticleSystem(FString Path)
{
    const FString explosionPath = "ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/";
    FString particlePath = "";

    switch (WeaponType)
    {
    case EThrowableWeaponType::FRAGMENTATION1:
    case EThrowableWeaponType::FRAGMENTATION2:
    case EThrowableWeaponType::CLAYMORE:
        particlePath = explosionPath + "P_Explosion_Big_A.P_Explosion_Big_A'"; break;

    case EThrowableWeaponType::ILLUMINATION:
    case EThrowableWeaponType::STICK:
        particlePath = "ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_explosion.P_ky_explosion'"; break;

    case EThrowableWeaponType::GRAY_SMOKE: particlePath = explosionPath + "P_Explosion_Smoke.P_Explosion_Smoke'"; break;
    case EThrowableWeaponType::RED_SMOKE:  particlePath = explosionPath + "P_Explosion_Smoke.P_Explosion_Smoke'"; break;
    case EThrowableWeaponType::MOLOTOV:    particlePath = explosionPath + "P_Molotov.P_Molotov'"; break;
    }
    // 파티클 설정
    ConstructorHelpers::FObjectFinder<UParticleSystem> PARTICLE(*particlePath);

    if (PARTICLE.Succeeded())
        Particle = PARTICLE.Object;
}

void ACoreThrowableWeapon::InitProjectileMovementComp()
{
    if (WeaponType == EThrowableWeaponType::CLAYMORE)
        return;

    ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
    ProjectileMovementComp->SetUpdatedComponent(StaticMeshComp);
    ProjectileMovementComp->InitialSpeed  = 0.f;
    ProjectileMovementComp->MaxSpeed      = 0.f;
    ProjectileMovementComp->bShouldBounce = false;
    ProjectileMovementComp->bAutoActivate = false;
    ProjectileMovementComp->bInitialVelocityInLocalSpace = false;
    ProjectileMovementComp->Bounciness = 0.5f;
    ProjectileMovementComp->Friction = 0.5f;
    ProjectileMovementComp->BounceVelocityStopSimulatingThreshold = 5.f;
    ProjectileMovementComp->Velocity = FVector::ZeroVector;
}

void ACoreThrowableWeapon::InitMesh()
{
    Super::InitStaticMesh(WeaponData.MeshPath);

    if (StaticMeshComp)
    {
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

void ACoreThrowableWeapon::InitSphereComp()
{
    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
    SphereComp->SetupAttachment(GrenadeParticleComp);

    // 충돌체 관련 설정
    SphereComp->SetGenerateOverlapEvents(false);
    SphereComp->SetSphereRadius(300.f);
    SphereComp->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
    SphereComp->SetCollisionProfileName("OverlapOnlyPawn");
    SphereComp->BodyInstance.bLockTranslation = true;
    SphereComp->BodyInstance.bLockRotation = true;

    // 델리게이트 바인딩
    SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACoreThrowableWeapon::BeginOverlap);
    SphereComp->OnComponentEndOverlap.AddDynamic(this, &ACoreThrowableWeapon::EndOverlap);
    SphereComp->Deactivate();
}

bool ACoreThrowableWeapon::IsPlayerInRadius()
{
    using TypeQueryByte = TEnumAsByte<EObjectTypeQuery>;

    // 범위 안에 들어가는지 확인 (원형 충돌감지)
    FVector startPos = GetActorLocation(),
            endPos = startPos + FVector(0.f, 0.f, 0.1f);
    TArray<TypeQueryByte> arrObjectTypeQuery;
    TypeQueryByte         objectTypeQuery = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
    arrObjectTypeQuery.Add(objectTypeQuery);
    TArray<AActor*> arrActorsToIgnore;
    FHitResult radiusHitResult;
    
    if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), startPos, endPos, WeaponData.ExplosionRadius, arrObjectTypeQuery, false, arrActorsToIgnore, EDrawDebugTrace::None, radiusHitResult, true))
    {
        auto       character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        FHitResult lineHitResult;
        
        if (GetWorld()->LineTraceSingleByChannel(lineHitResult, startPos, character->GetActorLocation(), ECollisionChannel::ECC_Camera))
            return (lineHitResult.GetActor() == character);
    }
    return false;
}

void ACoreThrowableWeapon::Setup(ACoreThrowableWeapon* OtherWeapon)
{
    if (!OtherWeapon)
        return;

    DestroyComponentsForUI();
    /*StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SetStaticMesh(OtherWeapon->StaticMeshComp->GetStaticMesh());*/
    
    // 데이터 설정
    WeaponData = OtherWeapon->WeaponData;
    WeaponType = OtherWeapon->WeaponType;
    
    // 이펙트 정보 설정
    Particle = OtherWeapon->Particle;
    BindExplosionFunc();
}

void ACoreThrowableWeapon::Throw(FVector Velocity)
{
    if (!StaticMeshComp ||
        !ProjectileMovementComp)
        return;

    // 메쉬 설정
    this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    this->SetActorScale3D(FVector(WeaponData.MeshSize));
    StaticMeshComp->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
    StaticMeshComp->SetCollisionProfileName("Explosive");

    // 발사체 컴포넌트 설정
    Velocity.Z = 0.1f;
    ProjectileMovementComp->bSimulationEnabled = true;
    ProjectileMovementComp->Activate();
    ProjectileMovementComp->Velocity = Velocity;
    ProjectileMovementComp->bShouldBounce = true;

    GetWorld()->GetTimerManager().SetTimer(mWaitHandle, FTimerDelegate::CreateLambda([&]()
        {
            auto meshLocation = StaticMeshComp->GetComponentLocation();

            // 사운드 재생
            if (mpCustomGameInstance)
            {
                auto p_soundManager = mpCustomGameInstance->pSoundManager;

                if (p_soundManager &&
                    !mbPlayed)
                {
                    UGameplayStatics::PlaySoundAtLocation(GetWorld(), p_soundManager->GetExplosiveSoundBase((int)WeaponType), meshLocation);
                    mbPlayed = true;
                }
            }
            // 효과 재생
            if (IsPlayerInRadius())
                mCallBack.ExecuteIfBound();

            // 이펙트 재생 
            if (WeaponType == EThrowableWeaponType::GRAY_SMOKE ||
                WeaponType == EThrowableWeaponType::RED_SMOKE  ||
                WeaponType == EThrowableWeaponType::MOLOTOV)
            {
                StaticMeshComp->SetVisibility(false);

                if (!GrenadeParticleComp)
                {
                    GrenadeParticleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, meshLocation, FRotator::ZeroRotator, false);
                    GrenadeParticleComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
                }
                else
                {
                    if (SphereComp)
                    {
                        SphereComp->SetGenerateOverlapEvents(true);
                    }
                }
            }
            else
            {
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, meshLocation);
                Destroy();
            }
        }), (WeaponType == EThrowableWeaponType::MOLOTOV) ? 0.5f : 3.5f, false);
}