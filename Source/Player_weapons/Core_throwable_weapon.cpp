// Fill out your copyright notice in the Description page of Project Settings.


#include "Core_throwable_weapon.h"

ACore_throwable_weapon::ACore_throwable_weapon()
{
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACore_throwable_weapon::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ACore_throwable_weapon::Tick(float _delta_time)
{
    Super::Tick(_delta_time);
}