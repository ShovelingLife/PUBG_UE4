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

void ACoreMeleeWeapon::BeginPlay()
{
    Super::BeginPlay();
    ABaseInteraction::SetCollisionSettingsForObjects();
}

void ACoreMeleeWeapon::Init(EMeleeWeaponType WeaponType)
{
    WeaponData = ADataTableManager::ArrOtherWeaponData[(int)WeaponType];    
    this->CurrentWeaponType = WeaponType;
    ObjectType = WeaponData.Type;    

    InitMesh();
}

void ACoreMeleeWeapon::InitMesh()
{
    ABaseInteraction::InitStaticMesh(WeaponData.MeshPath);
    StaticMeshComp->SetRelativeRotation(FRotator::ZeroRotator);
    StaticMeshComp->SetRelativeLocation(FVector::ZeroVector);
}