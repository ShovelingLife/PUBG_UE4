﻿#include "CoreMeleeWeapon.h"
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
    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
}

void ACoreMeleeWeapon::BeginPlay()
{
    Super::BeginPlay();
    ABaseInteraction::SetCollisionSettingsForObjects();
}

void ACoreMeleeWeapon::Init(EMeleeWeaponType Type)
{
    WeaponData = ADataTableManager::ArrOtherWeaponData[(int)Type];    
    this->WeaponType = Type;
    ObjectType = WeaponData.Type;    

    InitMesh();
}

void ACoreMeleeWeapon::InitMesh()
{
    ABaseInteraction::InitStaticMesh(WeaponData.MeshPath);
    StaticMeshComp->SetRelativeRotation(FRotator::ZeroRotator);
    StaticMeshComp->SetRelativeLocation(FVector::ZeroVector);
}