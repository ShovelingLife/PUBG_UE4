#include "CoreThrowableWeapon.h"
#include "PUBG_UE4/DataTableManager.h"
#include "PUBG_UE4/SoundManager.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundBase.h"
#include "Materials/Material.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include <Runtime/Engine/Public/DrawDebugHelpers.h>

ACoreThrowableWeapon::ACoreThrowableWeapon()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACoreThrowableWeapon::BeginPlay()
{
    Super::BeginPlay();
}

void ACoreThrowableWeapon::Tick(float _DeltaTime)
{
    Super::Tick(_DeltaTime);
}

void ACoreThrowableWeapon::Init(EThrowableWeaponType _WeaponType)
{
    WeaponData   = ADataTableManager::ArrOtherWeaponData[(int)_WeaponType];
    WeaponType   = _WeaponType;
    ObjectType = WeaponData.Type;
    
    ABaseInteraction::InitAudio();
    Update_collider();
    InitMesh();
    InitParticleSystem();
}

void ACoreThrowableWeapon::InitMesh()
{
    ABaseInteraction::InitStaticMesh(WeaponData.MeshPath);
    StaticMeshComp->SetRelativeRotation(FRotator::ZeroRotator);
    StaticMeshComp->SetRelativeLocation(FVector::ZeroVector);
}

void ACoreThrowableWeapon::Update_collider()
{
    ColliderComp->AddLocalOffset(WeaponData.ColliderPos);
    ColliderComp->SetBoxExtent(WeaponData.ColliderSize);
    ColliderComp->AddRelativeLocation(FVector(0.f, 0.f, 8.f));
}

void ACoreThrowableWeapon::InitParticleSystem()
{
    // 총기 관련
    ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
    ParticleComp->bAutoActivate = false;
    ParticleComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}