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

ACoreThrowableWeapon::ACoreThrowableWeapon(EThrowableWeaponType WeaponType) : ACoreThrowableWeapon()
{
    this->CurrentWeaponType = WeaponType;
    WeaponData = ADataTableManager::ArrOtherWeaponData[(int)WeaponType];
    ObjectType = WeaponData.Type;
    ObjectGroupType = WeaponData.GroupType;

    this->InitMesh();
    this->InitParticleSystem();
    Super::AttachComponents();
}

void ACoreThrowableWeapon::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

    /*if (ProjectileMovementComp)
        ProjectileMovementComp->Deactivate();*/
}

UFUNCTION() void ACoreThrowableWeapon::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, "Overlap Begin");

    if (auto p_world = GetWorld())
    {
        if (OtherActor == UGameplayStatics::GetPlayerCharacter(p_world, 0))
        {
            if (auto p_customGameInst = Cast<UCustomGameInstance>(p_world->GetGameInstance()))
                p_customGameInst->DeleSetPlayerOtherState.ExecuteIfBound(EPlayerOtherState::BURNED);
        }
    }
}

UFUNCTION() void ACoreThrowableWeapon::EndOverlap(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Cyan, "Overlap End");

    if (auto p_world = GetWorld())
    {
        if (OtherActor == UGameplayStatics::GetPlayerCharacter(p_world, 0))
        {
            if (auto p_customGameInst = Cast<UCustomGameInstance>(p_world->GetGameInstance()))
            {
                p_customGameInst->DeleSetPlayerOtherState.ExecuteIfBound(EPlayerOtherState::NONE);
                p_customGameInst->DeleKillUI_Anim.ExecuteIfBound();
            }
        }
    }
}

void ACoreThrowableWeapon::BeginDestroy()
{
    Super::BeginDestroy();
    GrenadeEndPos = FVector::ZeroVector;

    if(auto p_world = GetWorld())
    {
        if (auto p_customGameInst = Cast<UCustomGameInstance>(p_world->GetGameInstance()))
            p_customGameInst->DeleKillUI_Anim.ExecuteIfBound();
    }
}

void ACoreThrowableWeapon::BeginPlay()
{
    Super::BeginPlay();
}
    
void ACoreThrowableWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    /*if (GrenadeParticleComp)
    {
        static float currentTime = 0.f;

        currentTime += DeltaTime;

        if (currentTime >= 3.5f)
        {
            GetWorld()->GetTimerManager().ClearTimer(mWaitHandle);
            this->Destroy();
        }
    }*/
}

void ACoreThrowableWeapon::BindFunc()
{
    switch (CurrentWeaponType)
    {
    case EThrowableWeaponType::ILLUMINATION:

        mCallBack.BindLambda([&]()
            {
                float distance = this->GetDistanceTo(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
                float startTime = (distance >= 300) ? 2.5f : 0.f;
                float waitTime = (startTime == 0.f) ? 5.f : 2.5f;

                if (UCustomGameInstance * p_customGameInst = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance()))
                    p_customGameInst->DeleRunEffectAnim.ExecuteIfBound(startTime, waitTime, EPlayerStateAnimType::BLINDED);
            });

        break;

    case EThrowableWeaponType::STICK:
    case EThrowableWeaponType::FRAGMENTATION1:
    case EThrowableWeaponType::FRAGMENTATION2:
    case EThrowableWeaponType::CLAYMORE:

        mCallBack.BindLambda([&]()
            {
                if (UCustomGameInstance* p_customGameInst = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance()))
                    p_customGameInst->DeleDealPlayerDmg.ExecuteIfBound(WeaponData.Damage);                
            });
        break;
    }
}

void ACoreThrowableWeapon::InitParticleSystem(FString Path)
{
    FString particlePath = "";

    switch (CurrentWeaponType)
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
        particlePath = "ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Smoke.P_Explosion_Smoke'";
        break;
    
    case EThrowableWeaponType::RED_SMOKE:
        particlePath = "ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Smoke.P_Explosion_Smoke'";
        break;

    case EThrowableWeaponType::MOLOTOV:
        particlePath = "ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Molotov.P_Molotov'";
        break;
    }
    // 파티클 설정
    ConstructorHelpers::FObjectFinder<UParticleSystem> PARTICLE(*particlePath);

    if (PARTICLE.Succeeded())
        Particle = PARTICLE.Object;
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
    ProjectileMovementComp->bInitialVelocityInLocalSpace = false;
    ProjectileMovementComp->Bounciness = 0.5f;
    ProjectileMovementComp->Friction = 0.5f;
    ProjectileMovementComp->BounceVelocityStopSimulatingThreshold = 5.f;
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
            return (lineHitResult.Actor == character);
    }
    return false;
}

void ACoreThrowableWeapon::Setup(ACoreThrowableWeapon* OtherWeapon)
{
    DestroyComponentsForUI();

    // 메시 설정
    if (SkeletalMeshComp)
    {
        SkeletalMeshComp->DestroyComponent();
        SkeletalMeshComp = nullptr;
    }
    SetRootComponent(StaticMeshComp);
    StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComp->SetStaticMesh(OtherWeapon->StaticMeshComp->GetStaticMesh());
    
    // 데이터 설정
    WeaponData = OtherWeapon->WeaponData;
    CurrentWeaponType = OtherWeapon->CurrentWeaponType;
    
    // 이펙트 정보 설정
    Particle = OtherWeapon->Particle;
    BindFunc();
}

void ACoreThrowableWeapon::Throw(FVector Velocity)
{
    if (!StaticMeshComp ||
        !ProjectileMovementComp)
        return;

    // 메쉬 설정
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    SetActorScale3D(FVector(WeaponData.MeshSize));
    StaticMeshComp->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
    StaticMeshComp->SetCollisionProfileName("Explosive");

    // 발사체 컴포넌트 설정
    ProjectileMovementComp->Activate();
    ProjectileMovementComp->Velocity = Velocity;
    ProjectileMovementComp->bShouldBounce = true;
    return;

    GetWorld()->GetTimerManager().SetTimer(mWaitHandle, FTimerDelegate::CreateLambda([&]()
        {
            auto location = StaticMeshComp->GetComponentLocation();

            // 사운드 재생
            if (auto p_world = GetWorld())
            {
                if(UCustomGameInstance* p_customGameInst = Cast<UCustomGameInstance>(p_world->GetGameInstance()))
                {
                    if (auto p_soundManager = p_customGameInst->pSoundManager)
                    {
                        if (!mbPlayed)
                        {
                            UGameplayStatics::PlaySoundAtLocation(GetWorld(), p_soundManager->GetExplosiveSoundBase((int)CurrentWeaponType), location);
                            mbPlayed = true;
                        }
                    }
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

                if (!GrenadeParticleComp)
                {
                    GrenadeParticleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, location, FRotator::ZeroRotator, false);
                    GrenadeParticleComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
                }
                if (SphereComp &&
                    GrenadeParticleComp)
                {
                    SphereComp->SetGenerateOverlapEvents(true);
                    //SphereComp->SetWorldLocationAndRotation(FVector(300.f, GrenadeEndPos.Y, GrenadeEndPos.Z), FRotator::ZeroRotator);
                }
            }
            else
            {
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, location);
                Destroy();
            }
        }), (CurrentWeaponType == EThrowableWeaponType::MOLOTOV) ? 0.5f : 3.5f, false);
}