#include "CoreMeleeWeapon.h"
#include "PUBG_UE4/DataTableManager.h"
#include "PUBG_UE4/SoundManager.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundBase.h"
#include "Materials/Material.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include <Runtime/Engine/Public/DrawDebugHelpers.h>

ACoreMeleeWeapon::ACoreMeleeWeapon()
{
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACoreMeleeWeapon::BeginPlay()
{
    Super::BeginPlay();
    ABaseInteraction::SetCollisionSettingsForObjects();
}

void ACoreMeleeWeapon::Init(EMeleeWeaponType _WeaponType)
{
    WeaponData = ADataTableManager::ArrOtherWeaponData[(int)_WeaponType];
    WeaponType = _WeaponType;
    ObjectType = WeaponData.Type;    

    UpdateCollider();
    InitMesh();

    if (SceneComp)
        SceneComp->DestroyComponent();
}

void ACoreMeleeWeapon::InitMesh()
{
    ABaseInteraction::InitStaticMesh(WeaponData.MeshPath);
    StaticMeshComp->SetRelativeRotation(FRotator::ZeroRotator);
    StaticMeshComp->SetRelativeLocation(FVector::ZeroVector);
}

void ACoreMeleeWeapon::UpdateCollider()
{
    //ColliderComp->AddLocalOffset(WeaponData.ColliderPos);
    ColliderComp->SetBoxExtent(FVector(WeaponData.ColliderSize));
    ColliderComp->AddRelativeLocation(FVector(0.f, 0.f, 8.f));
}