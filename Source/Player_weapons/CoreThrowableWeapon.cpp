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
#include "PhysicsEngine/RadialForceComponent.h"
#include "Sound/SoundBase.h"
#include <Runtime/Engine/Public/DrawDebugHelpers.h>

ACoreThrowableWeapon::ACoreThrowableWeapon()
{
    this->InitProjectileMovementComp();
    InitRadialForce();   
}

ACoreThrowableWeapon::ACoreThrowableWeapon(EThrowableWeaponType Type) : ACoreThrowableWeapon()
{
    ItemIdx = (int)Type;
    WeaponType = Type;
    WeaponData = ADataTableManager::GetOtherWeaponData(ItemIdx);
    ObjectType      = WeaponData.Type;
    ObjectGroupType = WeaponData.GroupType;

    this->InitMesh();
    this->InitParticleSystem();
}

void ACoreThrowableWeapon::BeginPlay()
{
    Super::BeginPlay();
}
    
void ACoreThrowableWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACoreThrowableWeapon::InitParticleSystem(FString Path)
{
    const FString explosionPath = "ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/";
    FString particlePath = "";

    switch (WeaponType)
    {
    case GRENADE:
    case CLAYMORE:
        particlePath = explosionPath + "P_Explosion_Big_A.P_Explosion_Big_A'"; break;

    case ILLUMINATION:
        particlePath = "ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_explosion.P_ky_explosion'"; break;

    case SMOKE:   particlePath = explosionPath + "P_Explosion_Smoke.P_Explosion_Smoke'"; break;
    case MOLOTOV: particlePath = explosionPath + "P_Molotov.P_Molotov'"; break;
    }
    // 파티클 설정
    ConstructorHelpers::FObjectFinder<UParticleSystem> PARTICLE(*particlePath);

    if (PARTICLE.Succeeded())
        Particle = PARTICLE.Object;
}

void ACoreThrowableWeapon::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

    /*if (ProjectileMovementComp)
        ProjectileMovementComp->Deactivate();*/
}

void ACoreThrowableWeapon::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 플레이어가 접촉 시 화상 효과를 줌 (오직 Molotov만)
    if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
    {
        if (auto p_customGameInst = UCustomGameInstance::GetInst())
            p_customGameInst->DeleSetPlayerOtherState.ExecuteIfBound(EPlayerOtherState::BURNED);
    }
}

void ACoreThrowableWeapon::EndOverlap(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // 
    if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
    {
        if (auto p_customGameInst = UCustomGameInstance::GetInst())
        {
            p_customGameInst->DeleSetPlayerOtherState.ExecuteIfBound(EPlayerOtherState::NONE);
            p_customGameInst->DeleKillUI_Anim.ExecuteIfBound();
        }
    }
}

void ACoreThrowableWeapon::BeginDestroy()
{
    Super::BeginDestroy();
    GrenadeEndPos = FVector::ZeroVector;

    /*if (auto gameInst = UCustomGameInstance::GetInst())
        gameInst->DeleKillUI_Anim.ExecuteIfBound();*/
}

void ACoreThrowableWeapon::ClickEvent()
{
    if (auto p_customGameInst = UCustomGameInstance::GetInst())
        p_customGameInst->DelePredictGrenadePath.ExecuteIfBound();
}

void ACoreThrowableWeapon::InitProjectileMovementComp()
{
    if (WeaponType == CLAYMORE)
        return;

    ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
    ProjectileMovementComp->SetUpdatedComponent(SkeletalMeshComp);
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
    Super::InitSkeletalMesh(WeaponData.MeshPath);

    if (SkeletalMeshComp)
    {
        // 강체 관련
        SkeletalMeshComp->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
        SkeletalMeshComp->SetSimulatePhysics(false);
        SkeletalMeshComp->SetCollisionProfileName("Explosive");
        SkeletalMeshComp->SetNotifyRigidBodyCollision(true);
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

void ACoreThrowableWeapon::InitRadialForce()
{
    RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
    this->SetRootComponent(SkeletalMeshComp);
    RadialForceComp->SetupAttachment(RootComponent);
    RadialForceComp->bImpulseVelChange = true;
    RadialForceComp->ImpulseStrength = 350.f;
    //RadialForceComp->ForceStrength = 1500.f;
    RadialForceComp->Radius = 150.f;
    RadialForceComp->RemoveObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
    RadialForceComp->Deactivate();
}

void ACoreThrowableWeapon::BindExplosionFunc()
{
    switch (WeaponType)
    {
    case ILLUMINATION:

        mExplosionEvent.BindLambda([this]()
            {
                float distance  = this->GetDistanceTo(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
                float startTime = (distance >= 300.f) ? 2.5f : 0.f;
                float waitTime  = (startTime == 0.f)  ? 5.0f : 2.5f;

                if (auto p_customGameInst = UCustomGameInstance::GetInst())
                    p_customGameInst->DeleRunEffectAnim.ExecuteIfBound(startTime, waitTime, EPlayerStateAnimType::BLINDED);
            });
        break;
    
    case GRENADE:
    case CLAYMORE:

        mExplosionEvent.BindLambda([this]()
            {
                if (auto p_customGameInst = UCustomGameInstance::GetInst())
                    p_customGameInst->DeleDealPlayerDmg.ExecuteIfBound(WeaponData.Damage);
            });
        break;
    }
}

bool ACoreThrowableWeapon::IsPlayerInRadius()
{
    // 충돌 감지할 오브젝트 설정
    TArray<TEnumAsByte<EObjectTypeQuery>> arrObjectTypeQuery;
    arrObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
    TArray<AActor*> arrActorsToIgnore;
    FHitResult radiusHitResult;
    FVector startPos = GetActorLocation();

    // 범위 안에 들어가는지 확인 (원형 충돌감지)
    return UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), startPos, startPos + 0.1f, WeaponData.ExplosionRadius, arrObjectTypeQuery, false, arrActorsToIgnore, EDrawDebugTrace::None, radiusHitResult, true);
}

void ACoreThrowableWeapon::Setup(ACoreThrowableWeapon* OtherWeapon)
{
    if (!OtherWeapon)
        return;

    // 데이터 설정
    WeaponData = OtherWeapon->WeaponData;
    WeaponType = OtherWeapon->WeaponType;

    DestroyComponentsForUI();

    // 메쉬 설정
    SetSkeletalMesh(OtherWeapon->GetSkeletalMesh());
    SkeletalMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SetRootComponent(SkeletalMeshComp);
    
    // 이펙트 정보 설정
    Particle = OtherWeapon->Particle;
    BindExplosionFunc();
}

void ACoreThrowableWeapon::Throw(FVector Velocity)
{
    if (!SkeletalMeshComp ||
        !ProjectileMovementComp)
        return;

    bThrowed = true;

    // 메쉬 설정
    this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    SkeletalMeshComp->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
    SkeletalMeshComp->SetCollisionProfileName("Explosive");

    // 발사체 컴포넌트 설정
    Velocity.Z = 0.1f;
    ProjectileMovementComp->Activate();
    ProjectileMovementComp->bSimulationEnabled = true;
    ProjectileMovementComp->bShouldBounce = true;
    ProjectileMovementComp->Velocity = Velocity;

    GetWorld()->GetTimerManager().SetTimer(mWaitHandle, FTimerDelegate::CreateLambda([&]()
        {
            auto meshLocation = SkeletalMeshComp->GetComponentLocation();

            // 사운드 재생
            if (auto p_customGameInst = UCustomGameInstance::GetInst())
            {
                auto p_soundManager = p_customGameInst->pSoundManager;

                if (p_soundManager &&
                    !mbPlayed)
                {
                    UGameplayStatics::PlaySoundAtLocation(GetWorld(), p_soundManager->GetExplosiveSoundBase((int)WeaponType), meshLocation);
                    mbPlayed = true;
                }
            }
            // 효과 재생
            if (IsPlayerInRadius())
                mExplosionEvent.ExecuteIfBound();

            // 이펙트 재생 
            if (WeaponData.Effect)
            {
                SkeletalMeshComp->SetVisibility(false);

                if (!GrenadeParticleComp)
                {
                    GrenadeParticleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, meshLocation, FRotator::ZeroRotator, false);
                    GrenadeParticleComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
                }
                else
                {
                    if (SphereComp)
                        SphereComp->SetGenerateOverlapEvents(true);
                }
            }
            else
            {
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, meshLocation);
                RadialForceComp->Activate();
                RadialForceComp->FireImpulse();
                Destroy();
            }   
        }), (WeaponType == MOLOTOV) ? 0.5f : 3.5f, false);
}

void ACoreThrowableWeapon::DestroyComponentsForUI()
{
    if (SphereComp)
        SphereComp->DestroyComponent();

    if (ProjectileMovementComp)
        ProjectileMovementComp->DestroyComponent();

    if (GrenadeParticleComp)
        GrenadeParticleComp->DestroyComponent();

    if (RadialForceComp)
        RadialForceComp->DestroyComponent();

    SetForDummyCharacter();
}